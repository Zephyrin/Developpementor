#include "createenvironment.h"

#include "environmentstring.h"
#include "environmentstring2.h"
#include "environment4.h"
#include "environment1.h"
#include "environment2.hpp"
#include "environmentsmallloop.h"
#include <environmentstringwidget.h>
#include <environmentsmallloopwidget.h>
#include <QSettings>
#include <QFileDialog>
#include <QByteArray>

CreateEnvironment::CreateEnvironment(BuildGraphics &build, QStyle *style)
  :
    combo(NULL),
    env(NULL),
    widget(NULL),
    fileNameSmallLoop(":/config/smallLoop.env"),
    fileNameString(":/config/stringMotivationWithout0.env")
  , build(build)
{
  combo = new QComboBox();
  combo->addItem("e1r1");
  combo->addItem("e1r1e1r2e2r1e2r2");
  combo->addItem("e1r1e1r1e1r2e2r1e2r1e2r2");
  combo->addItem("Sring");
  combo->addItem("String with equal");
  combo->addItem("String equal is down");
  combo->addItem("Loop");
  QObject::connect(combo, SIGNAL(activated(int)),
                   this, SLOT(updateDisplay(int)));
  selector = new QWidget();
  QBoxLayout *box = new QBoxLayout(QBoxLayout::LeftToRight);
  selector->setLayout(box);
  box->addWidget(combo);
  labelSmallLoop = new QLabel(fileNameSmallLoop);
  labelString = new QLabel(fileNameString);
  fileSelector = new QPushButton("...");
  QObject::connect(fileSelector, SIGNAL(clicked()),
                   this, SLOT(fileSelectorSlot()));
  defaultFile = new QPushButton(tr("default"));
  defaultFile->setIcon(style->standardIcon(QStyle::SP_DialogResetButton));
  QObject::connect(defaultFile, SIGNAL(clicked()),
                   this, SLOT(fileDefault()));

  graphicsConfig = new GraphicsConfig(build, NULL);
  box->addWidget(labelSmallLoop);
  box->addWidget(labelString);
  box->addWidget(fileSelector);
  box->addWidget(defaultFile);

  box->addWidget(graphicsConfig);

  readSettings();
}

CreateEnvironment::~CreateEnvironment()
{
  selector->deleteLater();
  graphicsConfig->deleteLater();
}

QWidget *CreateEnvironment::getSelector()
{
  return selector;
}

Environment *CreateEnvironment::createEnvironment()
{
  switch(combo->currentIndex())
    {
    case ENV1:
      env = new Environment1();
      widget = NULL;
      break;
    case ENV2:
      env = new Environment2();
      widget = NULL;
      break;
    case ENV4:
      env = new Environment4();
      widget = NULL;
      break;
    case ENVSTRING:
      {
        EnvironmentString *string = new EnvironmentString(fileNameString);
        widget = new EnvironmentStringWidget(build, string);
        env = string;
      }
      break;
    case ENVSTRING2:
      {
        EnvironmentString2 *string = new EnvironmentString2(fileNameString);
        widget = new EnvironmentStringWidget(build, string);
        env = string;
      }
      break;
    case ENVINVSTRING:
      {
        EnvironmentString *string = new EnvironmentString(fileNameString, true);
        widget = new EnvironmentStringWidget(build, string);
        env = string;
      }
      break;
    case ENVLOOP:
      {
        EnvironmentSmallLoop *loop =
            new EnvironmentSmallLoop(fileNameSmallLoop);
        widget = new EnvironmentSmallLoopWidget(loop);
        env = loop;
      }
      break;
    default:
      env = NULL;
      widget = NULL;
      break;
    }
  return env;
}

QWidget *CreateEnvironment::getAssociatedWidget()
{
  return widget;
}

void CreateEnvironment::writeSettings()
{
  QSettings settings("Tweak", "ZErnest");
  settings.beginGroup("Environments");
  settings.setValue("DefaultEnvironment", combo->currentIndex());
  settings.setValue("fileNameString", fileNameString);
  settings.setValue("fileNameSmallLoop", fileNameSmallLoop);
  settings.setValue("DefaultFolder", defaultFolder);
  settings.endGroup();
  graphicsConfig->writeConfig();
}

void CreateEnvironment::saveAndDeleteAssociateWidget()
{
  EnvironmentStringWidget *w = NULL;
  if((w = dynamic_cast<EnvironmentStringWidget *>(widget)) != NULL)
    w->writeSettings();
  widget->deleteLater();
}

void CreateEnvironment::readSettings()
{
  QSettings settings("Tweak", "ZErnest");
  settings.beginGroup("Environments");
  combo->setCurrentIndex(settings.value("DefaultEnvironment", ENVLOOP).toInt());
  fileNameString = settings.value("fileNameString", ":/config/string.env")
      .toString();
  fileNameSmallLoop = settings.value("fileNameSmallLoop"
                                     , ":/config/smallLoop.env")
      .toString();
  defaultFolder = settings.value("DefaultFolder", ".").toString();
  QFileInfo f(fileNameSmallLoop);
  labelSmallLoop->setText(f.baseName());
  QFileInfo f1(fileNameString);
  labelString->setText(f1.baseName());

  settings.endGroup();

  updateDisplay(combo->currentIndex());
}

void CreateEnvironment::updateDisplay(int index)
{
  fileSelector->setVisible(false);
  labelSmallLoop->setVisible(false);
  labelString->setVisible(false);
  defaultFile->setVisible(false);
  switch(index)
    {
    case ENVSTRING:
    case ENVSTRING2:
      fileSelector->setVisible(true);
      labelString->setVisible(true);
      defaultFile->setVisible(true);
      break;

    case ENVLOOP:
      fileSelector->setVisible(true);
      labelSmallLoop->setVisible(true);
      defaultFile->setVisible(true);
      break;
    default:
      break;
    }
}

void CreateEnvironment::fileSelectorSlot()
{
  QString filename = QFileDialog::getOpenFileName(NULL,
                                                  tr("Open environment"),
                                                  defaultFolder,
                                                  tr("Environment (*.env)"));

  if(!filename.isEmpty())
    {
      defaultFolder = QFileInfo(filename).absolutePath();
      QFileInfo f(filename);
      switch(combo->currentIndex())
        {
        case ENVSTRING:
        case ENVSTRING2:
          fileNameString = filename;
          labelString->setText(f.baseName());
          break;
        case ENVLOOP:
          fileNameSmallLoop = filename;
          labelSmallLoop->setText(f.baseName());
          break;
        default:
          break;
        }
    }
}

void CreateEnvironment::fileDefault()
{
  switch(combo->currentIndex())
    {
    case ENVSTRING:
    case ENVSTRING2:
    case ENVINVSTRING:
      fileNameString = ":/config/string.env";
      labelString->setText(fileNameString);
      break;
    case ENVLOOP:
      fileNameSmallLoop = ":/config/smallLoop.env";
      labelSmallLoop->setText(fileNameSmallLoop);
      break;
    default:
      break;
    }
}

