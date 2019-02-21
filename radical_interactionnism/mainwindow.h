#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "createenvironment.h"
#include "texteditsearch.h"

#include <simulation.h>
#include <buildgraphics.hpp>
#include <interactionfluxgraphics.hpp>
#include <phenomenasgraphics.hpp>
#include <interactiongraphics.hpp>
#include <usagetableinteractionsgraphics.hpp>
#include <QMainWindow>
#include <QThread>
#include <QSettings>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

protected:
  void writeSettings();

  void readSettings();

  void createSimulation();

private slots:
  void on_actionPause_triggered();

  void on_actionStop_triggered();

  void on_actionQuit_triggered();

  void on_actionConfig_triggered(bool checked);

  void on_actionStart_triggered();

  void on_actionConsole_triggered(bool checked);

  void on_dockWidgetConfig_visibilityChanged(bool visible);

  void on_dockWidgetConsole_visibilityChanged(bool visible);

  void failedToOpenFile(const QString &filename);

  void simulationDone();
  void simulationStart();
  void simulationPause();

  void toConsole(const QString &log);

  void on_dockWidgetLog_visibilityChanged(bool visible);

  void on_actionLog_toggled(bool arg1);

  void logSlot(const QString &message);

  void on_speedSpinBox_valueChanged(int arg1);

  void on_clearLog_clicked();

  void on_cActivated_clicked(bool checked);

  void on_cIntended_clicked(bool checked);

  void on_cProposed_clicked(bool checked);

  void on_cEnacted_clicked(bool checked);

  void on_cLearned_clicked(bool checked);

  void on_cReinforced_clicked(bool checked);

  void on_actionStep_triggered();

//  void on_dockWidgetInteraction_visibilityChanged(bool visible);

//  void on_actionInteraction_toggled(bool checked);

//  void on_dockWidgetEnacted_visibilityChanged(bool visible);

//  void on_dockWidgetInteractions_visibilityChanged(bool visible);

  void on_cPhenomenas_clicked(bool checked);
//  void on_dockWidgetPhenomenas_visibilityChanged(bool visible);
//  void on_actionPhenomenas_toggled(bool checked);

//  void on_actionEnacted_Interaction_toggled(bool checked);

//  void on_actionInteractions_toggled(bool checked);

//  void on_actionInteractions_Time_triggered(bool checked);

//  void on_dockWidgetInteractionsTime_visibilityChanged(bool visible);

  void on_toConsole_clicked(bool checked);

  void on_qAgent_currentIndexChanged(int index);

//  void on_dockWidgetUsageTable_visibilityChanged(bool visible);

//  void on_actionUsage_Table_toggled(bool arg1);

private:
  Ui::MainWindow *ui;
  QThread workerThread;
  Simulation *simulation;
  BuildGraphics build;
  CreateEnvironment createEnvironment;

  InteractionGraphics *interactionIntendedDisplay;
  InteractionGraphics *interactionEnactedDisplay;
  InteractionFluxGraphics *interactionsFlux;
  PhenomenasGraphics *phenomenasGraphics;
  UsageTableInteractionsGraphics *usageTableGraphics;
  TextEditSearch *textConsole;
  // QWidget interface
protected:
  void closeEvent(QCloseEvent *);
  void intendedInteractionVisibilityChanged(bool visible);
  void enactedInteractionVisibilityChanged(bool visible);
};

#endif // MAINWINDOW_H
