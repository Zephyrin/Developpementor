#ifndef TEXTEDITSEARCH_H
#define TEXTEDITSEARCH_H

#include <QWidget>
#include <QTextCursor>

namespace Ui {
  class TextEditSearch;
}

class TextEditSearch : public QWidget
{
  Q_OBJECT

public:
  explicit TextEditSearch(QWidget *parent = 0);
  ~TextEditSearch();

  void appendPlainText(const QString &text);
private slots:
  void on_clear_clicked();

  void on_actionSearch_triggered();

  void on_actionValidate_triggered();

  void on_actionCloseSearch_triggered();

  void on_next_clicked();

  void on_previous_clicked();

  void on_maxCountBlock_valueChanged(int arg1);

protected:
  void find(bool backward);

private:
  Ui::TextEditSearch *ui;
  bool isFirstTime;
};

#endif // TEXTEDITSEARCH_H
