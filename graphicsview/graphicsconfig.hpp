#ifndef GRAPHICSCONFIG_HPP
#define GRAPHICSCONFIG_HPP

#include "buildgraphics.hpp"

#include <QWidget>

namespace Ui {
  class GraphicsConfig;
}

class GraphicsConfig : public QWidget
{
  Q_OBJECT

public:
  explicit GraphicsConfig(BuildGraphics &build, QWidget *parent = 0);
  ~GraphicsConfig();

  void writeConfig();

protected:
  void readSettings();

private slots:
  void on_bDefault_clicked();

  void on_bSelectFile_clicked();

private:
  Ui::GraphicsConfig *ui;
  QString sConfigFile;
  QString defaultFolder;
  BuildGraphics &buildGraphics;
};

#endif // GRAPHICSCONFIG_HPP
