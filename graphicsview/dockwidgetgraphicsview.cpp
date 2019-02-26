#include "dockwidgetgraphicsview.hpp"
#include "ui_dockwidgetgraphicsview.h"
#include <QFileDialog>
#include <QCheckBox>
#include <QSettings>
#include <QtSvg/QSvgGenerator>
#include <QDebug>
#include <QAction>

DockWidgetGraphicsView::DockWidgetGraphicsView(const BuildGraphics &build
                                               , const QString &title
                                               , const QString configName
                                               , QAction *menuAction
                                               , const bool showUpdate
                                               , const bool showTimer
                                               , QWidget *parent)
  :
    QDockWidget(parent)
  , ui(new Ui::DockWidgetGraphicsView)
  , view(NULL)
  , timerId(0)
  , build(build)
  , configName(configName)
  , scene(NULL)
  , menuAction(menuAction)
{
  ui->setupUi(this);
  view = new GraphicsView(configName, this);
  scene = new QGraphicsScene(view);
  setObjectName(title);
  view->setScene(scene);
  view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  view->setCacheMode(QGraphicsView::CacheNone);
  view->setRenderHints(QPainter::Antialiasing
                       | QPainter::SmoothPixmapTransform);
  ui->dockWidgetContents->layout()->addWidget(view);
  ui->cRefresh->setVisible(showTimer);
  ui->lMilliSecond->setVisible(showTimer);
  ui->lRefreshSpeed->setVisible(showTimer);
  ui->sRefreshSpeed->setVisible(showTimer);
  ui->bUpdate->setVisible(showUpdate);
  if(menuAction)
    {
  optionsCheck = new QCheckBox(title
                               + " ("
                               + menuAction->shortcut().toString()
                               + ")", this);
  connect(menuAction, SIGNAL(toggled(bool))
          , this, SLOT(setVisible(bool)));
  connect(this, SIGNAL(visibilityChanged(bool))
          , menuAction, SLOT(setChecked(bool)));
  }
  else
      optionsCheck = new QCheckBox(title, this);
  setTitleBarWidget(optionsCheck);
  connect(optionsCheck, SIGNAL(clicked(bool))
          , this, SLOT(onOptionsClicked(bool)));
  connect(this, SIGNAL(visibilityChanged(bool))
          , this, SLOT(visibilityChange(bool)));
  readSettings();
}

DockWidgetGraphicsView::~DockWidgetGraphicsView()
{
  delete ui;
}
void DockWidgetGraphicsView::visibilityChange(const bool visible)
{
  if(visible)
    on_cRefresh_clicked(ui->cRefresh->isChecked());
  else
    on_cRefresh_clicked(false);
}

void DockWidgetGraphicsView::simulationStop()
{
}

void DockWidgetGraphicsView::onOptionsClicked(bool checked)
{
  ui->optionsBox->setVisible(checked);
}

void DockWidgetGraphicsView::on_cRefresh_clicked(bool checked)
{
  if(checked)
    {
      if(!timerId)
        timerId = startTimer(ui->sRefreshSpeed->value());
      ui->bClearView->setEnabled(false);
      ui->bUpdate->setEnabled(false);
    }
  else
    {
      if(timerId)
        {
          killTimer(timerId);
          timerId = 0;
        }
      ui->bClearView->setEnabled(true);
      ui->bUpdate->setEnabled(true);
    }
}

void DockWidgetGraphicsView::on_sRefreshSpeed_editingFinished()
{
  if(timerId)
    {
      killTimer(timerId);
      timerId = startTimer(ui->sRefreshSpeed->value());
    }
}

void DockWidgetGraphicsView::on_bExportSvg_clicked()
{
  QString filename = QFileDialog::getSaveFileName(NULL,
                                                  tr("Save SVG"),
                                                  defaultFolder,
                                                  tr("Svg (*.svg)"));
  if(!filename.isEmpty())
    {
      if(!filename.endsWith(".svg", Qt::CaseInsensitive))
        filename += ".svg";
      defaultFolder = QFileInfo(filename).absolutePath();
      QSvgGenerator gen;
      gen.setFileName(filename);
      gen.setSize(scene->itemsBoundingRect().size().toSize());
      gen.setViewBox(scene->itemsBoundingRect());
      QPainter painter;
      painter.begin(&gen);
      painter.setRenderHint(QPainter::Antialiasing);
      scene->render(&painter);
      painter.end();
    }
}

void DockWidgetGraphicsView::on_bUpdate_clicked()
{
  userUpdate();
}

void DockWidgetGraphicsView::on_bClearView_clicked()
{
  userClear();
}

void DockWidgetGraphicsView::timerEvent(QTimerEvent *)
{
}

void DockWidgetGraphicsView::userUpdate()
{

}

void DockWidgetGraphicsView::userClear()
{
}

void DockWidgetGraphicsView::readSettings()
{
  QSettings settings("Tweak", "ZErnest");
  settings.beginGroup("GraphicsView");
  settings.beginGroup(configName);
  ui->sRefreshSpeed->setValue(settings.value("refreshSpeed", "60").toInt());
  defaultFolder = settings.value("defaultFolder", ".").toString();
  ui->cRefresh->setChecked(settings.value("refreshEnable", false).toBool());
  on_cRefresh_clicked(ui->cRefresh->isChecked());
  bool optionsIsVisible = settings.value("optionsShow", true).toBool();
  optionsCheck->setChecked(optionsIsVisible);
  onOptionsClicked(optionsIsVisible);
  settings.endGroup();
  settings.endGroup();
  view->readSettings();
}

void DockWidgetGraphicsView::writeSettings()
{
  QSettings settings("Tweak", "ZErnest");
  settings.beginGroup("GraphicsView");
  settings.beginGroup(configName);
  settings.setValue("refreshSpeed", ui->sRefreshSpeed->value());
  settings.setValue("defaultFolder", defaultFolder);
  settings.setValue("refreshEnable", ui->cRefresh->isChecked());
  settings.setValue("optionsShow", optionsCheck->isChecked());
  settings.endGroup();
  settings.endGroup();
  view->writeSettings();
}

void DockWidgetGraphicsView::on_bCenterOnView_clicked()
{
    view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
    view->centerOn(scene->itemsBoundingRect().center());
}
