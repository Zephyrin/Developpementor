#include "graphicsrectitem.h"

#include <QDebug>
GraphicsRectItem::GraphicsRectItem(QGraphicsItem *item)
  :
    QGraphicsItemGroup(),
    item(item)
{

}

GraphicsRectItem::GraphicsRectItem(QList<QGraphicsItem *> &listItemsToHide)
  :
   QGraphicsItemGroup(),
    item(NULL),
    listItemsToHide(listItemsToHide)
{

}

GraphicsRectItem::~GraphicsRectItem()
{

}

void GraphicsRectItem::mousePressEvent(QGraphicsSceneMouseEvent *)
{
  if(item)
    {
  if(item->isVisible())
    item->hide();
  else
    item->show();
    }
  else
    {
      for(QGraphicsItem *item : listItemsToHide)
        {
          if(item->isVisible())
            item->hide();
          else
            item->show();
        }
    }
}

