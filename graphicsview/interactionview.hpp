#ifndef INTERACTIONVIEW_HPP
#define INTERACTIONVIEW_HPP


#include "graphicsview.hpp"
#include "buildgraphics.hpp"
#include <QWidget>

namespace Ui {
  class InteractionView;
}

class InteractionView : public QWidget
{
  Q_OBJECT

public:
  explicit InteractionView(const BuildGraphics &build
                           , const QString saveConfigName
                           , const bool showUpdate = true
                           , const bool showTimer = true
                           , QWidget *parent = 0);
  virtual ~InteractionView();

  void visibilityChange(const bool visible);
  void writeSettings();
public slots:
  virtual void simulationStop();

private slots:
  void on_cOptions_clicked(bool checked);

  void on_optionsBox_clicked(bool checked);

  void on_cRefresh_clicked(bool checked);

  void on_sRefreshSpeed_editingFinished();

  void on_bExportSvg_clicked();

  void on_bUpdate_clicked();

  void on_bClearView_clicked();

protected:
  void timerEvent(QTimerEvent *);
  virtual void userUpdate();
  virtual void userClear();
  void readSettings();
protected:
  Ui::InteractionView *ui;
  GraphicsView *view;
  QGraphicsScene *scene;
  int timerId;
  QString defaultFolder;
  const BuildGraphics &build;
  QString saveConfigName;
};

#endif // INTERACTIONVIEW_HPP
