#include "texteditsearch.h"
#include "ui_texteditsearch.h"

TextEditSearch::TextEditSearch(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::TextEditSearch),
  isFirstTime(true)
{
  ui->setupUi(this);
  ui->previous->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
  ui->next->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
  ui->clear->setIcon(style()->standardIcon(
                       QStyle::SP_DialogResetButton));
  ui->searchwidget->setVisible(false);
  ui->plainTextEdit->addAction(ui->actionSearch);
  ui->plainTextEdit->addAction(ui->actionCloseSearch);
  ui->search->addAction(ui->actionValidate);
  ui->search->addAction(ui->actionCloseSearch);
  ui->plainTextEdit->setMaximumBlockCount(ui->maxCountBlock->value());
}

TextEditSearch::~TextEditSearch()
{
  delete ui;
}

void TextEditSearch::appendPlainText(const QString &text)
{
  if(isFirstTime == false)
    {
      ui->plainTextEdit->document()->undo();
      isFirstTime = true;
    }
  ui->plainTextEdit->appendPlainText(text);
}

void TextEditSearch::on_clear_clicked()
{
  ui->plainTextEdit->clear();
}

void TextEditSearch::on_actionSearch_triggered()
{
  if(!ui->searchwidget->isVisible())
    ui->searchwidget->setVisible(true);
  ui->search->setFocus();
}

void TextEditSearch::on_actionValidate_triggered()
{
  find(false);
}

void TextEditSearch::on_actionCloseSearch_triggered()
{
  if(ui->searchwidget->isVisible())
    ui->searchwidget->setVisible(false);
  if(isFirstTime == false)
    {
      ui->plainTextEdit->document()->undo();
      isFirstTime = true;
    }
}

void TextEditSearch::on_next_clicked()
{
  find(false);
}

void TextEditSearch::find(bool backward)
{
  QString searchString = ui->search->text();
  QTextDocument *document = ui->plainTextEdit->document();
  if (isFirstTime == false)
    {
      document->undo();
      isFirstTime = true;
    }

  if (!searchString.isEmpty())
    {
      QTextCursor cursor(document);
      QTextCursor highlightCursor(ui->plainTextEdit->textCursor());
      if(!backward && highlightCursor.atEnd())
        {
          highlightCursor.setPosition(0);
        }
      else if(backward && highlightCursor.atStart())
        highlightCursor.movePosition(QTextCursor::End,
                                     QTextCursor::MoveAnchor,
                                     0);
      cursor.beginEditBlock();
      QTextCharFormat plainFormat(highlightCursor.charFormat());
      QTextCharFormat colorFormat = plainFormat;
      colorFormat.setForeground(Qt::red);
      colorFormat.setBackground(Qt::yellow);
      if (!highlightCursor.isNull() ) {
          if(backward)
            highlightCursor = document->find(searchString, highlightCursor,
                                             QTextDocument::FindWholeWords
                                             | QTextDocument::FindBackward);
          else
            highlightCursor = document->find(searchString, highlightCursor,
                                             QTextDocument::FindWholeWords);

          if (!highlightCursor.isNull()) {
//              ui->plainTextEdit->setCursor(highlightCursor);
              highlightCursor.movePosition(QTextCursor::WordRight,
                                           QTextCursor::KeepAnchor);
              highlightCursor.mergeCharFormat(colorFormat);
              ui->plainTextEdit->moveCursor(QTextCursor::Start);
              ui->plainTextEdit->setTextCursor(highlightCursor);
//              ui->plainTextEdit->textCursor().movePosition(QTextCursor::Start,
//                                              QTextCursor::MoveAnchor,
//                                              highlightCursor.position());
              isFirstTime = false;
            }
          else
            {
              QTextCursor cursor = ui->plainTextEdit->textCursor();
              if(backward)
                cursor.movePosition(QTextCursor::End,
                                    QTextCursor::MoveAnchor,
                                    0);
              else
                cursor.movePosition(QTextCursor::Start,
                                    QTextCursor::MoveAnchor,
                                    0);
              ui->plainTextEdit->setTextCursor(cursor);
            }
        }

      cursor.endEditBlock();
    }
}

void TextEditSearch::on_previous_clicked()
{
  find(true);
}

void TextEditSearch::on_maxCountBlock_valueChanged(int blockCount)
{
    ui->plainTextEdit->setMaximumBlockCount(blockCount);
}
