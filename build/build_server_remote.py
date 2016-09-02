#!/usr/bin/env python
import pika
import json
import sys
import shlex
from base import system_info
import config
import build

def print_usage():
    print("Usage:\n"
        "[optional] argv[1] platform\n"
        "[optional] argv[2] architecture\n")

class ProgressSaver(build.ProgressSaverBase):
    def __init__(self, progress_min, progress_max, cb):
        build.ProgressSaverBase.__init__(self)
        self.progress_max_ = progress_max
        self.progress_min_ = progress_min
        self.cb_ = cb
        self.progress_ = 0.0

    def process(self, line):
        diff = self.progress_max_ - self.progress_min_
        perc = self.progress_min_ + diff * (self.progress_ / 100.0)
        self.cb_(line, perc)

class BuildRpcServer(object):
    EXCHANGE = 'direct_logs'
    EXCHANGE_TYPE = 'direct'
    QUEUE ='build'

    def __init__(self, platform, arch_bit):
        self.connection_ = None
        self.channel_ = None
        self.closing_ = False
        self.consumer_tag_ = None
        self.routing_key_ = system_info.gen_routing_key(platform, str(arch_bit))
        self.buid_ = build.BuildRequest(platform, arch_bit)
        print("Build server for %s inited!" % platform)

    def connect(self):
        credentials = pika.PlainCredentials(config.USER_NAME, config.PASSWORD)
        params = pika.ConnectionParameters(host = config.REMOTE_HOST, credentials = credentials)
        return pika.SelectConnection(params, self.on_connection_open, stop_ioloop_on_close = False)

    def reconnect(self):
        self.connection_.ioloop.stop()

        if not self.closing_:
            self.connection_ = self.connect()
            self.connection_.ioloop.start()

    def on_connection_open(self, unused_connection):
        self.add_on_connection_close_callback()
        self.open_channel()

    def add_on_connection_close_callback(self):
        self.connection_.add_on_close_callback(self.on_connection_closed)

    def open_channel(self):
        self.connection_.channel(on_open_callback=self.on_channel_open)

    def on_channel_open(self, channel):
        self.channel_ = channel
        self.add_on_channel_close_callback()
        self.setup_exchange(self.EXCHANGE)

    def add_on_channel_close_callback(self):
        self.channel_.add_on_close_callback(self.on_channel_closed)

    def setup_exchange(self, exchange_name):
        self.channel_.exchange_declare(self.on_exchange_declareok, exchange_name, self.EXCHANGE_TYPE)

    def on_exchange_declareok(self, unused_frame):
        self.setup_queue(self.QUEUE)

    def setup_queue(self, queue_name):
        self.channel_.queue_declare(self.on_queue_declareok, queue_name)

    def on_queue_declareok(self, method_frame):
        self.channel_.queue_bind(self.on_bindok, self.QUEUE, self.EXCHANGE, self.routing_key_)

    def on_bindok(self, unused_frame):
         self.start_consuming()

    def start_consuming(self):
        self.add_on_cancel_callback()
        print("Awaiting RPC build requests")
        self.consumer_tag_ = self.channel_.basic_consume(self.on_request, self.routing_key_)

    def on_consumer_cancelled(self, method_frame):
        if self.channel_:
            self.channel_.close()

    def add_on_cancel_callback(self):
        self.channel_.add_on_cancel_callback(self.on_consumer_cancelled)

    def on_channel_closed(self, channel, reply_code, reply_text):
        print("on_channel_closed reply_code: {0}, reply_text: {1}".format(reply_code, reply_text))
        self.connection_.close()

    def on_connection_closed(self, connection, reply_code, reply_text):
        print("on_connection_closed reply_code: {0}, reply_text: {1}".format(reply_code, reply_text))
        self.channel_ = None
        if self.closing_:
            self.connection_.ioloop.stop()
        else:
            self.connection_.add_timeout(5, self.reconnect)

    def run(self):
        self.connection_ = self.connect()
        self.connection_.ioloop.start()

    def on_line_ready(self, line, progress, routing_key, op_id):
        self.send_status(routing_key, op_id, progress, line)
        print line

    def build_package(self, op_id, branding_options, package_types, destination, routing_key):
        platform = self.buid_.platform()
        arch = platform.arch()

        platform_and_arch_str = '{0}_{1}'.format(platform.name(), arch.name())
        dir_name = 'build_{0}_for_{1}'.format(platform_and_arch_str, op_id)

        def store(cb, routing_key, op_id):
            def closure(line, progress):
                return cb(line, progress, routing_key, op_id)
            return closure

        store = store(self.on_line_ready, routing_key, op_id)

        self.send_status(routing_key, op_id, 20, 'Building package')
        saver = ProgressSaver(21, 79, store)
        filenames = self.buid_.build('..', branding_options, dir_name, package_types, saver)
        filename = filenames[0]
        self.send_status(routing_key, op_id, 80, 'Loading package to server')
        try:
            result = config.post_install_step(filename, destination)
        except Exception as ex:
            raise ex

        return result

    def send_status(self, routing_key, op_id, progress, status):
        json_to_send = {'progress': progress, 'status' : status}
        properties = pika.BasicProperties(content_type = 'application/json', correlation_id = op_id, headers = {'type' : 'status'})
        if self.channel_:
            self.channel_.basic_publish(exchange='',
                         routing_key=routing_key,
                         properties = properties,
                         body=json.dumps(json_to_send))

    def send_responce(self, routing_key, op_id, body):
        properties = pika.BasicProperties(
                                            content_type = 'application/json',
                                            correlation_id = op_id,
                                            headers = {'type' : 'responce'}
                                         )
        if self.channel_:
            self.channel_.basic_publish(exchange='',
                         routing_key=routing_key,
                         properties = properties,
                         body=body)

    def on_request(self, ch, method, props, body):
        data = json.loads(body)
        #self.acknowledge_message(method.delivery_tag)
        #return

        branding_variables = data.get('branding_variables')
        platform = data.get('platform')
        arch = data.get('arch')
        package_type = data.get('package_type')
        destination = data.get('destination')
        op_id = props.correlation_id
        package_types = []
        package_types.append(package_type)

        self.send_status(props.reply_to, op_id, 0, 'Prepare to build package')
        print('Build started for: {0}, platform: {1}_{2}'.format(op_id, platform, arch))
        try:
            response = self.build_package(op_id, shlex.split(branding_variables), package_types, destination, props.reply_to)
            print('Build finished for: {0}, platform: {1}_{2}, responce: {3}'.format(op_id, platform, arch, response))
            json_to_send = {'body' : response}
        except build.BuildError as ex:
            print('Build finished for: {0}, platform: {1}_{2}, exception: {3}'.format(op_id, platform, arch, str(ex)))
            json_to_send = {'error': str(ex)}
        except Exception as ex:
            print('Build finished for: {0}, platform: {1}_{2}, exception: {3}'.format(op_id, platform, arch, str(ex)))
            json_to_send = {'error': str(ex)}
        
        self.send_status(props.reply_to, op_id, 100, 'Completed')
        self.send_responce(props.reply_to, op_id, json.dumps(json_to_send))
        self.acknowledge_message(method.delivery_tag)

    def acknowledge_message(self, delivery_tag):
        if self.channel_:
            self.channel_.basic_ack(delivery_tag)

if __name__ == "__main__":
    argc = len(sys.argv)

    if argc > 1:
        platform_str = sys.argv[1]
    else:
        platform_str = system_info.get_os()

    if argc > 2:
        arch_str = sys.argv[2]
    else:
        arch_str = system_info.get_arch_bit()
        
    server = BuildRpcServer(platform_str, int(arch_str))
    server.run()