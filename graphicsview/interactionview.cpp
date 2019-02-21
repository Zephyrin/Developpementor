#include "interactionview.hpp"
#include "ui_interactionview.h"

#include <QSettings>

#include <QFileDialog>
#include <QtSvg/QSvgGenerator>

InteractionView::InteractionView(const BuildGraphics &build
                                 , const QString saveConfigName
                                 , const bool showUpdate
                                 , const bool showTimer
                                 , QWidget *parent)
  :
    QWidget(parent)
  , ui(new Ui::InteractionView)
  , view(NULL)
  , scene(NULL)
  , timerId(0)
  , build(build)
  , saveConfigName(saveConfigName)
{
  ui->setupUi(this);
  view = new GraphicsView(saveConfigName, this);
  scene = new QGraphicsScene(view);
  view->setScene(scene);
  view->setRenderHints(QPainter::Antialiasing
                       | QPainter::SmoothPixmapTransform);
  view->readSettings();
  this->layout()->addWidget(view);
  ui->cOptions->setVisible(false);
  ui->cRefresh->setVisible(showTimer);
  ui->lMilliSecond->setVisible(showTimer);
  ui->lRefreshSpeed->setVisible(showTimer);
  ui->sRefreshSpeed->setVisible(showTimer);
  ui->bUpdate->setVisible(showUpdate);
  readSettings();
}

InteractionView::~InteractionView()
{
  delete ui;
}

void InteractionView::visibilityChange(const bool visible)
{
  on_cRefresh_clicked(visible);
}

void InteractionView::simulationStop()
{
}

void InteractionView::on_cOptions_clicked(bool checked)
{
  ui->cOptions->setVisible(!checked);
  ui->optionsBox->setVisible(checked);
  ui->optionsBox->setChecked(checked);
}

void InteractionView::on_optionsBox_clicked(bool checked)
{
  ui->optionsBox->setVisible(checked);
  ui->cOptions->setVisible(!checked);
  ui->cOptions->setChecked(checked);
}

void InteractionView::on_cRefresh_clicked(bool checked)
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

void InteractionView::on_sRefreshSpeed_editingFinished()
{
  if(timerId)
    {
      killTimer(timerId);
      timerId = startTimer(ui->sRefreshSpeed->value());
    }
}

void InteractionView::on_bExportSvg_clicked()
{
  QString filename = QFileDialog::getSaveFileName(NULL,
                                                  tr("Save SVG"),
                                                  defaultFolder,
                                                  tr("Svg (*.svg)"));
  if(!filename.isEmpty())
    {
      if(!filename.endsWith(".svg", Qt::CaseInsensitive))
        filename += ".svg";
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

void InteractionView::on_bUpdate_clicked()
{
  userUpdate();
}

void InteractionView::on_bClearView_clicked()
{
  userClear();
}

void InteractionView::timerEvent(QTimerEvent *)
{
}

void InteractionView::userUpdate()
{

}

void InteractionView::userClear()
{
}

void InteractionView::readSettings()
{
  QSettings settings("Tweak", "ZErnest");
  settings.beginGroup("GraphicsView");
  settings.beginGroup(saveConfigName);
  ui->sRefreshSpeed->setValue(settings.value("refreshSpeed", "60").toInt());
  defaultFolder = settings.value("defaultFolder", ".").toString();
  ui->cRefresh->setChecked(settings.value("refreshEnable", false).toBool());
  on_cRefresh_clicked(ui->cRefresh->isChecked());
  ui->optionsBox->setChecked(settings.value("optionsShow", true).toBool());
  on_optionsBox_clicked(ui->optionsBox->isChecked());
  settings.endGroup();
  settings.endGroup();
}

void InteractionView::writeSettings()
{
  QSettings settings("Tweak", "ZErnest");
  settings.beginGroup("GraphicsView");
  settings.beginGroup(saveConfigName);
  settings.setValue("refreshSpeed", ui->sRefreshSpeed->value());
  settings.setValue("defaultFolder", defaultFolder);
  settings.setValue("refreshEnable", ui->cRefresh->isChecked());
  settings.setValue("optionsShow", ui->optionsBox->isChecked());
  settings.endGroup();
  settings.endGroup();
  view->writeSettings();
}

