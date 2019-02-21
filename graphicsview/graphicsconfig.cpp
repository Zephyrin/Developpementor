#include "graphicsconfig.hpp"
#include "ui_graphicsconfig.h"
#include <QSettings>
#include <QFileDialog>

GraphicsConfig::GraphicsConfig(BuildGraphics &build, QWidget *parent)
  :
    QWidget(parent)
  , ui(new Ui::GraphicsConfig)
  , buildGraphics(build)
{
  ui->setupUi(this);
  readSettings();
}

GraphicsConfig::~GraphicsConfig()
{
  delete ui;
}

void GraphicsConfig::readSettings()
{
  QSettings settings("Tweak", "ZErnest");
  settings.beginGroup("GraphicsConfig");
  defaultFolder = settings.value("DefaultFolder", ".").toString();
  sConfigFile = settings.value("fileNameConfig", ":/graphics.conf")
      .toString();
  if(sConfigFile != ":/graphics.conf")
    {
      QFileInfo f(sConfigFile);
      ui->lFileName->setText(f.baseName());
    }
  else
    ui->lFileName->setText(sConfigFile);
  settings.endGroup();
  buildGraphics.readSettings(sConfigFile);
}

void GraphicsConfig::writeConfig()
{
  QSettings settings("Tweak", "ZErnest");
  settings.beginGroup("GraphicsConfig");
  settings.setValue("DefaultFolder", defaultFolder);
  settings.setValue("fileNameConfig", sConfigFile);
  settings.endGroup();
}

void GraphicsConfig::on_bDefault_clicked()
{
  sConfigFile = ":/graphics.conf";
  ui->lFileName->setText(sConfigFile);
}

void GraphicsConfig::on_bSelectFile_clicked()
{
  QString filename = QFileDialog::getOpenFileName(NULL,
                                                  tr("Graphics configuration"),
                                                  defaultFolder,
                                                  tr("Configuration (*.conf)"));
  if(!filename.isEmpty())
    {
      defaultFolder = QFileInfo(filename).absolutePath();
      QFileInfo f(filename);
      sConfigFile = filename;
      ui->lFileName->setText(f.baseName());
      buildGraphics.readSettings(filename);
    }
}
