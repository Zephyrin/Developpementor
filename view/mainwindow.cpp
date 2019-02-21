#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <interactionfluxgraphics.hpp>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
  :
    QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  this->setCentralWidget(NULL);
  interactionsFlux = new InteractionFluxGraphics(build
                                                 , "Flux"
                                                 , "InteractionFluxGraphics"
                                                 , NULL
                                                 , NULL
                                                 , this);

  addDockWidget(Qt::BottomDockWidgetArea, interactionsFlux);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
  close();
}

void MainWindow::closeEvent(QCloseEvent *)
{
}


