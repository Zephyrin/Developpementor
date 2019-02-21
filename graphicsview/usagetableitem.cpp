#include "usagetableitem.hpp"
#include <QDebug>
UsageTableItem::UsageTableItem(const UsageTable *noema
                               , const BuildGraphics &build
                               , GraphicsView *graphWidget)
  :
    graph(graphWidget)
  , build(build)
  , noema(noema)
{
  setFlag(ItemIsMovable);
  setFlag(ItemSendsGeometryChanges);
  setCacheMode(DeviceCoordinateCache);
  setZValue(-1);

}

UsageTableItem::~UsageTableItem()
{

}

QRectF UsageTableItem::boundingRect() const
{
  qreal elementSize = 100.;
  qreal space = 20.;
  qreal penSize = 5.;
  qreal width = 0;
  qreal height = 0.;
  qreal nbElementsSignatures = noema->getPostInteractions().nbElements();
  width = space
      + elementSize * 2 // preInteraction
      + penSize * 2 // interaction left + interaction right
      // + usageTable left +  usageTable right
      + 2 * space // between interaction and usage table and the next element
      + elementSize * 2 // postInteraction
      + penSize * 2 // between interaction
      // and usage table and the end of element
      + 2 * space
      + elementSize * 2
      + 2 * space
      + penSize * 4;
  height = space
      + elementSize
      + 2 * penSize
      + space
      + nbElementsSignatures * elementSize
      + nbElementsSignatures * space
      + nbElementsSignatures * penSize * 2;
  return QRectF(- width / 2. - penSize
                , - penSize - height / 2.
                , (width / 2. + penSize) * 2.
                , (penSize + height / 2.) * 2.);
}

QPainterPath UsageTableItem::shape() const
{
  QPainterPath path;
  path.addRect(boundingRect());
  QPolygon p;
  p.append(QPoint(0, 0));
  p.append(QPoint(1, 1));
  p.append(QPoint(2, 2));
  path.addPolygon(p);
  path.arcTo(1, 1, 1, 1, 1, 1);

  return path;
}

void UsageTableItem::paint(QPainter *painter
                           , const QStyleOptionGraphicsItem *option
                           , QWidget *widget)
{
  Q_UNUSED(option)
  Q_UNUSED(widget)
  build.paintUsageTable(painter, noema);
  painter->drawPie(QRect(), 1, 1);
  setToolTip(noema->toTextTip().c_str());
}


QVariant UsageTableItem::itemChange(GraphicsItemChange change
                                    , const QVariant &value)
{
  return QGraphicsItem::itemChange(change, value);
}

void UsageTableItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  update();
  QGraphicsItem::mousePressEvent(event);
}

void UsageTableItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  update();
  QGraphicsItem::mouseReleaseEvent(event);
  QRectF sceneRect = scene()->sceneRect();
  QPointF newPos = pos();
  newPos.setX(qMin(qMax(newPos.x(), sceneRect.left()
                        + boundingRect().width() / 2. + 10.),
                   sceneRect.right() - (boundingRect().width() / 2. + 10)));
  newPos.setY(qMin(qMax(newPos.y()
                        , sceneRect.top()
                        + boundingRect().height() / 2. + 10.)
                   , sceneRect.bottom() - (boundingRect().height() / 2. + 10)));
  setPos(newPos);
}

