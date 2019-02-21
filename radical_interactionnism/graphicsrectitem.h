#ifndef GRAPHICSRECTITEM_H
#define GRAPHICSRECTITEM_H

#include <QGraphicsItemGroup>

class GraphicsRectItem : public QGraphicsItemGroup
{
public:
  GraphicsRectItem(QGraphicsItem *item);
  GraphicsRectItem(QList<QGraphicsItem *> &listItemsToHide);
  virtual ~GraphicsRectItem();
protected:

  void mousePressEvent(QGraphicsSceneMouseEvent *);
private:
  QGraphicsItem *item;
  QList<QGraphicsItem *> listItemsToHide;
};

#endif // GRAPHICSRECTITEM_H
