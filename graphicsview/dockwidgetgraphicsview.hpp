#ifndef DOCKWIDGETGRAPHICSVIEW_HPP
#define DOCKWIDGETGRAPHICSVIEW_HPP

#include "graphicsview.hpp"
#include "buildgraphics.hpp"
#include <QDockWidget>

class QCheckBox;
namespace Ui {
  class DockWidgetGraphicsView;
}

class DockWidgetGraphicsView : public QDockWidget
{
  Q_OBJECT

public:
  explicit DockWidgetGraphicsView(const BuildGraphics &build
                                  , const QString &title
                                  , const QString configName
                                  , QAction *menuAction
                                  , const bool showUpdate = true
                                  , const bool showTimer = true
                                  , QWidget *parent = 0);
  virtual ~DockWidgetGraphicsView();

  void writeSettings();

  void setAction(QAction *action);

public slots:
  virtual void simulationStop();
private slots:
  void visibilityChange(const bool visible);

  void onOptionsClicked(bool checked);

  void on_cRefresh_clicked(bool checked);

  void on_sRefreshSpeed_editingFinished();

  void on_bExportSvg_clicked();

  void on_bUpdate_clicked();

  void on_bClearView_clicked();

  void on_bCenterOnView_clicked();
protected:
  void timerEvent(QTimerEvent *);
  virtual void userUpdate();
  virtual void userClear();
  void readSettings();
protected:
  Ui::DockWidgetGraphicsView *ui;
  GraphicsView *view;
  int timerId;
  QString defaultFolder;
  const BuildGraphics &build;
  QString configName;
private:
  QGraphicsScene *scene;
  QCheckBox *optionsCheck;
  QAction *menuAction;
};

#endif // DOCKWIDGETGRAPHICSVIEW_HPP
