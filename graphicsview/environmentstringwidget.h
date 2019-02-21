#ifndef ENVIRONMENTSTRINGWIDGET_H
#define ENVIRONMENTSTRINGWIDGET_H

#include <vector>

#include "graphicsview.hpp"

#include <environmentstring.h>
#include <buildgraphics.hpp>

#include "widgetgraphicsview.hpp"

class EnvironmentStringWidget : public WidgetGraphicsView
{
  Q_OBJECT

public:
  explicit EnvironmentStringWidget(const BuildGraphics &build
                                   , EnvironmentString *string
                                   , QWidget *parent = 0);
  virtual ~EnvironmentStringWidget();

protected slots:
  void stepSlot(const unsigned int nextPos,
                const Interaction *enacted);
  void feelSlot(const unsigned int nextPos, const Interaction *enacted);
  void swapSlot(const unsigned int pos, const unsigned int nextPos,
                const Interaction *enacted);
  void init();

protected:
  void resetColor(const int nextPos);
  void changeColor(const int index, const QColor &color);

  void setAgentPos(const int index);
  void setInteractionEnacted(const Interaction *enacted);
  void on_resetBoard_clicked();
private:
  QVector<QGraphicsItemGroup *> boardGraphics;
  QGraphicsPolygonItem *agent;
  int max;
  EnvironmentString *string;
  QGraphicsItem *interactionEnacted;
};

#endif // ENVIRONMENTSTRINGWIDGET_H
