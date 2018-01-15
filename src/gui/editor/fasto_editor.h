/*  Copyright (C) 2014-2018 FastoGT. All right reserved.

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

#include <QModelIndex>
#include <QWidget>

class QCheckBox;  // lines 34-34
class QFrame;     // lines 35-35
class QKeyEvent;
class QLineEdit;  // lines 31-31
class QMenu;      // lines 37-37
class QObject;
class QPixmap;
class QPoint;
class QPushButton;  // lines 33-33
class QToolButton;  // lines 32-32
class QsciLexer;    // lines 36-36

namespace fastonosql {
namespace gui {
class FastoScintilla;

class FastoEditor : public QWidget {
  Q_OBJECT
 public:
  enum { HeightFindPanel = 40 };
  explicit FastoEditor(QWidget* parent = Q_NULLPTR);
  virtual ~FastoEditor();

  void registerImage(int id, const QPixmap& im);

  QString text() const;
  QString selectedText() const;

  void setLexer(QsciLexer* lexer);
  QsciLexer* lexer() const;

  bool isReadOnly() const;

 Q_SIGNALS:
  void textChanged();
  void readOnlyChanged();

 public Q_SLOTS:
  void append(const QString& text);
  void setReadOnly(bool ro);
  void setText(const QString& text);
  void clear();

 private Q_SLOTS:
  void goToNextElement();
  void goToPrevElement();

 protected:
  void setShowAutoCompletion(bool showA);
  QMenu* createStandardContextMenu();

  void setCallTipsStyle(int style);
  void sendScintilla(unsigned int msg, unsigned long wParam = 0, long lParam = 0);

  virtual void keyPressEvent(QKeyEvent* e) override;
  virtual bool eventFilter(QObject* object, QEvent* event) override;
  virtual void changeEvent(QEvent* ev) override;

 private:
  void retranslateUi();
  void findElement(bool forward);

  FastoScintilla* scin_;
  QFrame* find_panel_;
  QLineEdit* find_line_;
  QToolButton* close_;
  QPushButton* next_;
  QPushButton* prev_;
  QCheckBox* case_sensitive_;
};

}  // namespace gui
}  // namespace fastonosql
