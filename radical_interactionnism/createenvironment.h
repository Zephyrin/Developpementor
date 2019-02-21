#ifndef CREATEENVIRONMENT_H
#define CREATEENVIRONMENT_H

#include "environment.hpp"
//#include "buildgrahpicsitem.h"

#include <graphicsconfig.hpp>
#include <QComboBox>

#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>

class CreateEnvironment : public QObject
{
  Q_OBJECT
public:
  CreateEnvironment(BuildGraphics &build, QStyle *style);
  ~CreateEnvironment();

  QWidget *getSelector();

  Environment *createEnvironment();
  QWidget *getAssociatedWidget();
  void writeSettings();
  void saveAndDeleteAssociateWidget();
protected:
  void readSettings();
private slots:
  void updateDisplay(int);
  void fileSelectorSlot();
  void fileDefault();

private:
  QWidget *selector;
  QComboBox *combo;
  Environment *env;
  QWidget *widget;
  QString fileNameSmallLoop;
  QString fileNameString;
  QLabel *labelSmallLoop;
  QLabel *labelString;
  QPushButton *fileSelector;
  QPushButton *defaultFile;
  QString defaultFolder;
//  BuildGrahpicsItem buildGraphicsItem;
  GraphicsConfig *graphicsConfig;
  const BuildGraphics &build;

  enum ENV
  {
    ENV1 = 0,
    ENV2 = 1,
    ENV4 = 2,
    ENVSTRING = 3,
    ENVSTRING2 = 4,
    ENVINVSTRING = 5,
    ENVLOOP = 6
  };
};

#endif // CREATEENVIRONMENT_H
