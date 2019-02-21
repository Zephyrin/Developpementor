#include "widgetgraphicsview.hpp"
#include "ui_widgetgraphicsview.h"
#include <QFileDialog>
#include <QSvgGenerator>
#include <QSettings>
#include <QCheckBox>
#include <QTimer>

WidgetGraphicsView::WidgetGraphicsView(const BuildGraphics &build
                                       , const QString &configName
                                       , QWidget *parent) :
  QWidget(parent)
  , build(build)
  , ui(new Ui::WidgetGraphicsView)
, configName(configName)
{
  ui->setupUi(this);
  view = new GraphicsView(configName, this);
  scene = new QGraphicsScene(view);
  view->setScene(scene);
  view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  view->setCacheMode(QGraphicsView::CacheNone);
  view->setRenderHints(QPainter::Antialiasing
                       | QPainter::SmoothPixmapTransform);
  ui->layout->addWidget(view);
  readSettings();

}

WidgetGraphicsView::~WidgetGraphicsView()
{
  delete ui;
//  view->deleteLater();
}

void WidgetGraphicsView::on_optionsVisible_clicked(bool checked)
{
  ui->hiddenWidget->setVisible(checked);
}

void WidgetGraphicsView::on_reset_clicked()
{
  on_resetBoard_clicked();
}

void WidgetGraphicsView::on_resize_clicked()
{
  view->sceneFitInView();
}

void WidgetGraphicsView::on_exportAsSvg_clicked()
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

void WidgetGraphicsView::on_resetBoard_clicked()
{

}

void WidgetGraphicsView::resizeEvent(QResizeEvent *)
{
  if(resizeEventEnable)
    {
      QTimer::singleShot(1, this, SLOT(fitInView()));

    }
}

void WidgetGraphicsView::fitInView()
{
  view->sceneFitInView();
}

void WidgetGraphicsView::on_autoResize_toggled(bool checked)
{
  ui->resize->setEnabled(!checked);
  resizeEventEnable = checked;
}

void WidgetGraphicsView::readSettings()
{
  QSettings settings("Tweak", "ZErnest");
  settings.beginGroup("GraphicsView");
  settings.beginGroup(configName);
  defaultFolder = settings.value("defaultFolder", ".").toString();
  ui->autoResize->setChecked(settings.value("autoResize", false).toBool());
  bool optionsIsVisible = settings.value("optionsShow", true).toBool();
  on_optionsVisible_clicked(optionsIsVisible);
  ui->optionsVisible->setChecked(optionsIsVisible);
  settings.endGroup();
  settings.endGroup();
  view->readSettings();
}

void WidgetGraphicsView::writeSettings()
{
  QSettings settings("Tweak", "ZErnest");
  settings.beginGroup("GraphicsView");
  settings.beginGroup(configName);
  settings.setValue("defaultFolder", defaultFolder);
  settings.setValue("autoResize", ui->autoResize->isChecked());
  settings.setValue("optionsShow", ui->optionsVisible->isChecked());
  settings.endGroup();
  settings.endGroup();
  view->writeSettings();
}
