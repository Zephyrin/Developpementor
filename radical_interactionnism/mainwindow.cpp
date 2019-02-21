#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <interactionfluxgraphics.hpp>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
  :
    QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , createEnvironment(build, style())
{
  ui->setupUi(this);
  ui->actionStart->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
  ui->actionPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
  ui->actionStop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));

  ui->actionStep->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
  ui->clearLog->setIcon(style()->standardIcon(
                          QStyle::SP_DialogResetButton));
  ui->actionStop->setEnabled(false);
  ui->actionPause->setEnabled(false);
  ui->toolBar->addAction(ui->actionStart);
  ui->toolBar->addAction(ui->actionStep);
  ui->toolBar->addAction(ui->actionPause);
  ui->toolBar->addAction(ui->actionStop);
  ui->toolBar->addWidget(createEnvironment.getSelector());
  simulation = new Simulation();
  simulation->moveToThread(&workerThread);
  workerThread.start();
  interactionIntendedDisplay = new InteractionGraphics(
        build
        , "Intended Interaction"
        , "IntendedGraphics"
        , simulation
        , SIGNAL(intendedInteraction(const Interaction*))
        , ui->actionInteraction
        , this);
  interactionEnactedDisplay = new InteractionGraphics(
        build
        , "Enacted Interaction"
        , "EnactedGraphics"
        , simulation
        , SIGNAL(enactedInteraction(const Interaction*))
        , ui->actionEnacted_Interaction
        , this);
  usageTableGraphics = new UsageTableInteractionsGraphics(
        build
        , "Usage Table"
        , "UsageTableInteraction"
        , ui->actionUsage_Table
        , simulation
        , this);
  phenomenasGraphics = new PhenomenasGraphics(build
                                              , "Phenomenas"
                                              , "PhenomenasGraphics"
                                              , simulation
                                              , ui->actionPhenomenas
                                              , this);
  interactionsFlux = new InteractionFluxGraphics(build
                                                 , "Flux"
                                                 , "InteractionFluxGraphics"
                                                 , simulation
                                                 , ui->actionInteractions_Time
                                                 , this);

  addDockWidget(Qt::BottomDockWidgetArea, phenomenasGraphics);
  addDockWidget(Qt::BottomDockWidgetArea, interactionIntendedDisplay);
  addDockWidget(Qt::BottomDockWidgetArea, interactionEnactedDisplay);
  addDockWidget(Qt::BottomDockWidgetArea, usageTableGraphics);
  addDockWidget(Qt::BottomDockWidgetArea, interactionsFlux);

  textConsole = new TextEditSearch();
  ui->dockWidgetContents_2->layout()->addWidget(textConsole);

  connect(simulation, SIGNAL(failedToOpenFile(const QString &)),
          this, SLOT(failedToOpenFile(const QString &)));
  connect(simulation, SIGNAL(simulationDone()),
          this, SLOT(simulationDone()));
  connect(simulation, SIGNAL(toConsoleSignal(const QString &)),
          this, SLOT(toConsole(const QString &)));
  connect(simulation, SIGNAL(logSignal(const QString &)),
          this, SLOT(logSlot(const QString &)));
  connect(simulation, SIGNAL(simulationPause()),
          this, SLOT(simulationPause()));
  connect(simulation, SIGNAL(startSimulationSignal()),
          this, SLOT(simulationStart()));
  readSettings();
}

MainWindow::~MainWindow()
{
  simulation->stop();
  workerThread.quit();
  workerThread.wait();
  workerThread.deleteLater();
  delete simulation;
  delete ui;
}

void MainWindow::on_actionPause_triggered()
{
  ui->actionStop->setEnabled(true);
  ui->actionStart->setEnabled(true);
  ui->actionStep->setEnabled(true);
  simulation->paused();
}

void MainWindow::on_actionStop_triggered()
{
  ui->actionStart->setEnabled(true);
  ui->actionStop->setEnabled(false);
  simulation->stop();
}

void MainWindow::on_actionQuit_triggered()
{
  close();
}

void MainWindow::on_actionConfig_triggered(bool checked)
{
  ui->dockWidgetConfig->setVisible(checked);
}

void MainWindow::on_actionStart_triggered()
{
  if(simulation->isRunning())
    simulation->resume();
  else
    {
      createSimulation();
    }
  if(simulation->isRunning())
    {
      ui->actionPause->setEnabled(true);
      ui->actionStart->setEnabled(false);
      ui->actionStop->setEnabled(false);
    }
}

void MainWindow::on_actionStep_triggered()
{
  if(simulation->isRunning())
    simulation->step();
  else
    {
      createSimulation();
      simulation->step();
    }
  if(simulation->isRunning())
    {
      ui->actionStart->setEnabled(true);
      ui->actionStop->setEnabled(true);
      ui->actionPause->setEnabled(false);
    }
}

void MainWindow::createSimulation()
{
  if(createEnvironment.getAssociatedWidget() != NULL)
    {
      ui->centralLayout->removeWidget(
            createEnvironment.getAssociatedWidget());
      createEnvironment.saveAndDeleteAssociateWidget();
//      createEnvironment.getAssociatedWidget()->deleteLater();
    }
  if(ui->ktbs->isChecked())
    {
      simulation->start(ui->ktbsServer->text(),
                        ui->ktbsPort->value(),
                        ui->ktbsBase->text(),
                        ui->ktbsModel->text(),
                        ui->ktbsTrace->text(),
                        createEnvironment.createEnvironment(),
                        ui->display->isChecked() ?
                          ui->toConsole->isChecked()
                        : false,
                        ui->display->isChecked() ? ui->toFile->isChecked()
                                                 : false,
                        ui->display->isChecked() ?
                          ui->toFile->isChecked() ?
                            ui->filename->text() : ""
                          : "",
                        ui->speedSpinBox->value(),
                        ui->numberOfLoop->value(),
                        ui->maxInteractions->value(),
                        ui->boredAt->value(),
                        ui->qAgent->currentIndex(),
                        ui->cUsePhenomenas->isChecked()
                        , ui->sExcitementThreshold->value());
    }
  else
    {
      simulation->start(createEnvironment.createEnvironment(),
                        ui->display->isChecked() ?
                          ui->toConsole->isChecked()
                        : false,
                        ui->display->isChecked() ? ui->toFile->isChecked()
                                                 : false,
                        ui->display->isChecked() ?
                          ui->toFile->isChecked() ?
                            ui->filename->text() : ""
                          : "",
                        ui->speedSpinBox->value(),
                        ui->numberOfLoop->value(),
                        ui->maxInteractions->value(),
                        ui->boredAt->value(),
                        ui->qAgent->currentIndex(),
                        ui->cUsePhenomenas->isChecked()
                        , ui->sExcitementThreshold->value());
    }
  if(createEnvironment.getAssociatedWidget() != NULL)
    ui->centralLayout->addWidget(createEnvironment.getAssociatedWidget());
}

void MainWindow::on_actionConsole_triggered(bool checked)
{
  ui->dockWidgetConsole->setVisible(checked);
}

void MainWindow::on_dockWidgetConfig_visibilityChanged(bool visible)
{
    ui->actionConfig->setChecked(visible);
}

void MainWindow::on_dockWidgetConsole_visibilityChanged(bool visible)
{
  ui->actionConsole->setChecked(visible);
}

void MainWindow::failedToOpenFile(const QString &filename)
{
  QMessageBox::warning(this, tr("Error"), tr("Failed to open file %1 in "
                                             "write access\n"
                                             "Simulation did not start")
                       .arg(filename),
                       QMessageBox::Ok);
}

void MainWindow::simulationDone()
{
  ui->actionPause->setEnabled(false);
  ui->actionStart->setEnabled(true);
  ui->actionStop->setEnabled(false);
//  interactionsFlux->simulationStop();
}

void MainWindow::simulationStart()
{
}

void MainWindow::simulationPause()
{
  ui->actionPause->setEnabled(false);
  ui->actionStart->setEnabled(true);
  ui->actionStop->setEnabled(true);
}

void MainWindow::toConsole(const QString &log)
{
  textConsole->appendPlainText(log);
}

void MainWindow::on_dockWidgetLog_visibilityChanged(bool visible)
{
    ui->actionLog->setChecked(visible);
}

void MainWindow::on_actionLog_toggled(bool checked)
{
  ui->dockWidgetLog->setVisible(checked);
}

void MainWindow::logSlot(const QString &message)
{
  ui->logPlainText->appendPlainText(QDateTime::currentDateTime()
                                    .toString("yyyy-MM-ddT HH-mm-sst : ")
                                    + message);
}

void MainWindow::on_speedSpinBox_valueChanged(int speed)
{
    simulation->setSpeed(speed);
}

void MainWindow::on_clearLog_clicked()
{
    ui->logPlainText->clear();
}

void MainWindow::on_cActivated_clicked(bool checked)
{
   Trace::setCActivated(checked);
}

void MainWindow::on_cIntended_clicked(bool checked)
{
    Trace::setCIntended(checked);
}

void MainWindow::on_cProposed_clicked(bool checked)
{
    Trace::setCProposed(checked);
}

void MainWindow::on_cEnacted_clicked(bool checked)
{
    Trace::setCEnacted(checked);
}

void MainWindow::on_cLearned_clicked(bool checked)
{
    Trace::setCLearned(checked);
}

void MainWindow::on_cReinforced_clicked(bool checked)
{
  Trace::setCReinforced(checked);
}

void MainWindow::on_cPhenomenas_clicked(bool checked)
{
  Trace::setCPhenomenas(checked);
}

//void MainWindow::on_dockWidgetInteraction_visibilityChanged(bool visible)
//{
//    ui->actionInteraction->setChecked(visible);
//    intendedInteractionVisibilityChanged(visible);
//}

//void MainWindow::on_actionInteraction_toggled(bool checked)
//{
////  ui->dockWidgetInteraction->setVisible(checked);
//}

//void MainWindow::intendedInteractionVisibilityChanged(bool visible)
//{
//  if(visible)
//    {
//      connect(simulation, SIGNAL(intendedInteraction(const Interaction*)),
//              (InteractionGraphics *)interactionIntendedDisplay,
//              SLOT(showInteraction(const Interaction*)));
//    }
//  else
//    {
//      simulation->disconnect(interactionIntendedDisplay);
//    }
//}

//void MainWindow::enactedInteractionVisibilityChanged(bool visible)
//{
//  if(visible)
//    {
//      connect(simulation, SIGNAL(enactedInteraction(const Interaction*)),
//              interactionEnactedDisplay,
//              SLOT(showInteraction(const Interaction*)));
//    }
//  else
//    {
//      simulation->disconnect(interactionEnactedDisplay);
//    }
//}

//void MainWindow::on_dockWidgetInteractionsTime_visibilityChanged(bool visible)
//{
//  if(visible)
//    {
//      connect(simulation, SIGNAL(intendedEnactedSignal(const Interaction*,
//                                                       const Interaction*
//                                                       , const Noema *
//                                                       , const int
//                                                       , const int
//                                                       , const Noema *)),
//              interactionsFlux,
//              SLOT(addInteractions(const Interaction *,
//                                   const Interaction *
//                                   , const Noema *
//                                   , const int
//                                   , const int
//                                   , const Noema *)));
//    }
//  else
//    {
//      simulation->disconnect(interactionsFlux);
//    }
//  ui->actionInteractions_Time->setChecked(visible);
//}

void MainWindow::on_toConsole_clicked(bool checked)
{
  if(checked)
    {
      connect(simulation, SIGNAL(toConsoleSignal(const QString &)),
              this, SLOT(toConsole(const QString &)));
    }
  else
    simulation->disconnect(SIGNAL(toConsoleSignal(const QString &)));
}

void MainWindow::on_qAgent_currentIndexChanged(int index)
{
    switch(index)
      {
      case 0:
        ui->cUsePhenomenas->setEnabled(true);
        ui->sExcitementThreshold->setEnabled(false);
        break;
      case 1:
        ui->cUsePhenomenas->setEnabled(false);
        ui->sExcitementThreshold->setEnabled(true);
        break;
      default:
        ui->cUsePhenomenas->setEnabled(false);
        ui->sExcitementThreshold->setEnabled(false);
        break;
      }
}

void MainWindow::closeEvent(QCloseEvent *)
{

  writeSettings();
}

void MainWindow::writeSettings()
{
  QSettings settings("Tweak", "ZErnest");
  settings.beginGroup("MainWindow");
  settings.setValue("size", size());
  settings.setValue("pos", pos());
  settings.setValue("isMaximized", isMaximized());
  settings.setValue("centralWidget", ui->centralwidget->size());
  settings.setValue("states", this->saveState());
  settings.setValue("geometry", this->saveGeometry());
  settings.endGroup();

  settings.beginGroup("Config");
  settings.beginGroup("Ktbs");
  settings.setValue("isActivate", ui->ktbs->isChecked());
  settings.setValue("base", ui->ktbsBase->text());
  settings.setValue("model", ui->ktbsModel->text());
  settings.setValue("port", ui->ktbsPort->value());
  settings.setValue("trace", ui->ktbsTrace->text());
  settings.setValue("domain", ui->ktbsServer->text());
  settings.endGroup();
  settings.beginGroup("Display");
  settings.setValue("toConsole", ui->toConsole->isChecked());
  settings.setValue("toFile", ui->toFile->isChecked());
  settings.setValue("isActivate", ui->display->isChecked());
  settings.setValue("cActivated", ui->cActivated->isChecked());
  settings.setValue("cProposed", ui->cProposed->isChecked());
  settings.setValue("cLearned", ui->cLearned->isChecked());
  settings.setValue("cIntended", ui->cIntended->isChecked());
  settings.setValue("cEnacted", ui->cEnacted->isChecked());
  settings.setValue("cReinforced", ui->cReinforced->isChecked());
  settings.beginGroup("Phenomena");
  settings.setValue("cPhenomenas", ui->cPhenomenas->isChecked());
  settings.endGroup();
  settings.endGroup();
  settings.beginGroup("Simulation");
  settings.setValue("speed", ui->speedSpinBox->value());
  settings.setValue("numberOfLoop", ui->numberOfLoop->value());
  settings.setValue("maxInteractions", ui->maxInteractions->value());
  settings.setValue("maxSameInteractionBeforeBored", ui->boredAt->value());
  settings.setValue("usePhenomenas", ui->cUsePhenomenas->isChecked());
  settings.setValue("agent", ui->qAgent->currentIndex());
  settings.setValue("excitementThreshold", ui->sExcitementThreshold->value());
  settings.endGroup();
  settings.endGroup();

  createEnvironment.writeSettings();
  interactionIntendedDisplay->writeSettings();
  interactionEnactedDisplay->writeSettings();
  interactionsFlux->writeSettings();
  phenomenasGraphics->writeSettings();
  usageTableGraphics->writeSettings();
}

void MainWindow::readSettings()
{
  QSettings settings("Tweak", "ZErnest");
  settings.beginGroup("MainWindow");
  if(settings.value("isMaximized", true).toBool())
    setWindowState(windowState() | Qt::WindowMaximized);
  else
    {
      resize(settings.value("size", QSize(400, 400)).toSize());
      move(settings.value("pos", QPoint(200, 200)).toPoint());
    }
  this->restoreState(settings.value("states").toByteArray());
  this->restoreGeometry(settings.value("geometry").toByteArray());
  settings.endGroup();
  settings.beginGroup("Config");
  settings.beginGroup("Ktbs");
  ui->ktbs->setChecked(settings.value("isActivate", false).toBool());
  ui->ktbsBase->setText(settings.value("base", "radical_interactionism")
                        .toString());
  ui->ktbsModel->setText(settings.value("model", "model").toString());
  ui->ktbsPort->setValue(settings.value("port", 8001).toInt());
  ui->ktbsTrace->setText(settings.value("trace", "trace").toString());
  ui->ktbsServer->setText(settings.value("domain", "http://localhost")
                          .toString());
  settings.endGroup();
  settings.beginGroup("Display");
  ui->toConsole->setChecked(settings.value("toConsole", true).toBool());
  ui->toFile->setChecked(settings.value("toFile", false).toBool());
  ui->display->setChecked(settings.value("isActivate", true).toBool());
  ui->cActivated->setChecked(settings.value("cActivated", true).toBool());
  Trace::setCActivated(ui->cActivated->isChecked());
  ui->cProposed->setChecked(settings.value("cProposed", true).toBool());
  Trace::setCProposed(ui->cProposed->isChecked());
  ui->cLearned->setChecked(settings.value("cLearned", true).toBool());
  Trace::setCLearned(ui->cLearned->isChecked());
  ui->cIntended->setChecked(settings.value("cIntended", true).toBool());
  Trace::setCIntended(ui->cIntended->isChecked());
  ui->cEnacted->setChecked(settings.value("cEnacted", true).toBool());
  Trace::setCEnacted(ui->cEnacted->isChecked());
  ui->cReinforced->setChecked(settings.value("cReinforced", true).toBool());
  Trace::setCReinforced(ui->cReinforced->isChecked());
  settings.beginGroup("Phenomena");
  ui->cPhenomenas->setChecked(settings.value("cPhenomenas", true).toBool());
  settings.endGroup();
  settings.endGroup();
  settings.beginGroup("Simulation");
  ui->speedSpinBox->setValue(settings.value("speed", 100000).toInt());
  simulation->setSpeed(ui->speedSpinBox->value());
  ui->numberOfLoop->setValue(settings.value("numberOfLoop", 100).toInt());
  ui->maxInteractions->setValue(settings.value("maxInteractions", 0).toInt());
  ui->boredAt->setValue(settings.value("maxSameInteractionBeforeBored", 5)
                        .toInt());
  ui->cUsePhenomenas->setChecked(settings.value("usePhenomenas", true)
                                 .toBool());
  ui->qAgent->setCurrentIndex(settings.value("agent", 0).toInt());
  ui->sExcitementThreshold->setValue(settings.value("excitementThreshold", 3)
                                     .toInt());
  settings.endGroup();
  settings.endGroup();
}
