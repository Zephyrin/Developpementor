#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <buildgraphics.hpp>
#include <interactionfluxgraphics.hpp>
#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void on_actionQuit_triggered();

private:
  Ui::MainWindow *ui;
  BuildGraphics build;

  InteractionFluxGraphics *interactionsFlux;
  // QWidget interface
protected:
  void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
