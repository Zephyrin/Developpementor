#ifndef ENVIRONMENTSMALLLOOPWIDGET_H
#define ENVIRONMENTSMALLLOOPWIDGET_H

#include <environmentsmallloop.h>

//#include "graphicsviewdeprecated.h"
#include "graphicsview.hpp"
#include <QWidget>
#include <QGraphicsRectItem>
#include <QGraphicsScene>

namespace Ui {
  class EnvironmentSmallLoopWidget;
}

class EnvironmentSmallLoopWidget : public QWidget
{
  Q_OBJECT

public:
  explicit EnvironmentSmallLoopWidget(EnvironmentSmallLoop *loop,
      QWidget *parent = 0);
  ~EnvironmentSmallLoopWidget();

signals:
  void changeItem(const int pos);

public slots:
  void moveSlot(const unsigned int position, const int result);
  void turnSlot(const int angle);
  void feelSlot(const int result, const int dir);

protected slots:
  void mouseClickOnScene(const QPointF &point);
  void init();

protected:
  void clearLast();
private:
  Ui::EnvironmentSmallLoopWidget *ui;
  std::size_t X;
  std::size_t Y;
  QVector<QGraphicsRectItem *> boards;
  GraphicsView *view;
  QGraphicsScene scene;
  QGraphicsPolygonItem *agent;
  QGraphicsRectItem *cLeft;
  QGraphicsRectItem *cRight;
  QGraphicsRectItem *cBehind;
  QGraphicsItemGroup *group;
  QGraphicsRectItem * lastFeel;
  EnvironmentSmallLoop *loop;
};

#endif // ENVIRONMENTSMALLLOOPWIDGET_H
