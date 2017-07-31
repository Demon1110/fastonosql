/*  Copyright (C) 2014-2017 FastoGT. All right reserved.

    This file is part of FastoNoSQL.

    FastoNoSQL is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FastoNoSQL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FastoNoSQL.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <QByteArray>
#include <QTextEdit>

class QEvent;
class QMouseEvent;
class QPaintEvent;
class QPoint;
class QWidget;

namespace fastonosql {
namespace gui {

class FastoHexEdit : public QTextEdit {
  Q_OBJECT
 public:
  typedef QTextEdit base_class;
  explicit FastoHexEdit(QWidget* parent = 0);

  enum DisplayMode { TEXT_MODE, HTML_MODE, HEX_MODE };

  enum { TextMarginXY = 4 };

  QString text() const;

 Q_SIGNALS:
  void readOnlyChanged();

 public Q_SLOTS:
  void setMode(DisplayMode mode);
  void setData(const QByteArray& arr);
  void clear();

 protected:
  virtual void paintEvent(QPaintEvent* event) override;

  virtual void mousePressEvent(QMouseEvent* event) override;
  virtual void mouseMoveEvent(QMouseEvent* event) override;
  virtual void mouseReleaseEvent(QMouseEvent* event) override;
  virtual bool event(QEvent* event) override;

 private:
  static QRect stableRect(const QRect& rect);
  QSize fullSize() const;

  QByteArray data_;
  DisplayMode mode_;

  bool in_selection_state_;

  int charWidth() const;
  int charHeight() const;

  int asciiCharInLine(int wid) const;
  int positionAtPoint(const QPoint& point) const;
};

}  // namespace gui
}  // namespace fastonosql
