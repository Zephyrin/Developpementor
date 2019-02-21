#ifndef USAGETABLEITEM_HPP
#define USAGETABLEITEM_HPP

#include "graphicsview.hpp"
#include "buildgraphics.hpp"

#include <usagetable.hpp>
#include <QGraphicsItem>

class PhenomenaEdge;
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class UsageTableItem : public QGraphicsItem
{
public:
  UsageTableItem(const UsageTable *noema
                 , const BuildGraphics &build
                 , GraphicsView *graphWidget);
  ~UsageTableItem();
  enum { Type = UserType + 4 };
  int type() const Q_DECL_OVERRIDE { return Type; }
  QRectF boundingRect() const Q_DECL_OVERRIDE;
  QPainterPath shape() const Q_DECL_OVERRIDE;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option
             , QWidget *widget) Q_DECL_OVERRIDE;

protected:
    QVariant itemChange(GraphicsItemChange change
                        , const QVariant &value) Q_DECL_OVERRIDE;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

protected:
    QPointF newPos;
    GraphicsView *graph;
    const BuildGraphics &build;
    const UsageTable *noema;
};

#endif // USAGETABLEITEM_HPP
