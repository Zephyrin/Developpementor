#ifndef WIDGETGRAPHICSVIEW_HPP
#define WIDGETGRAPHICSVIEW_HPP
#include "graphicsview.hpp"
#include "buildgraphics.hpp"

#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsScene>

class QCheckBox;
namespace Ui {
  class WidgetGraphicsView;
}

class WidgetGraphicsView : public QWidget
{
  Q_OBJECT

public:
  explicit WidgetGraphicsView(const BuildGraphics &build
                              , const QString &configName
                              , QWidget *parent = 0);
  virtual ~WidgetGraphicsView();
  void writeSettings();

private slots:
  void on_reset_clicked();

  void on_resize_clicked();

  void on_exportAsSvg_clicked();

  void on_autoResize_toggled(bool checked);

  void on_optionsVisible_clicked(bool checked);

protected:
  void readSettings();

  GraphicsView *view;
  const BuildGraphics &build;
  Ui::WidgetGraphicsView *ui;
  void on_resetBoard_clicked();
private:
  QString defaultFolder;
  QString configName;
  QGraphicsScene *scene;
  bool resizeEventEnable;
  QCheckBox *optionsCheck;

  // QWidget interface
protected:
  void resizeEvent(QResizeEvent *);
protected slots:
  void fitInView();
};

#endif // WIDGETGRAPHICSVIEW_HPP
