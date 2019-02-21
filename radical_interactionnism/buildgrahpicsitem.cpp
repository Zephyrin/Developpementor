#include "buildgrahpicsitem.h"
#include <interaction.hpp>
#include <QPainter>
#include <QDebug>
#include <string>

BuildGrahpicsItem::BuildGrahpicsItem()
{
}

BuildGrahpicsItem::~BuildGrahpicsItem()
{

}

void BuildGrahpicsItem::centerBInA(
    QGraphicsItem *itemA
    , QGraphicsItem *itemB) const
{
  itemB->setParentItem(itemA);
  itemB->setPos(itemA->boundingRect().center().x()
                - itemB->boundingRect().center().x()
                , itemA->boundingRect().center().y()
                - itemB->boundingRect().center().y());
}

QGraphicsItem *BuildGrahpicsItem::createExperience(
    QGraphicsScene &scene
    , const std::string &id) const
{
  QColor color = figuresResults.at(id);
  FIGURE fig = figuresExperiences.at(id);
  QPen pen(Qt::black, 6);
  switch(fig)
    {
    case SPHERE:
      return scene.addEllipse(0, 0, 100, 100, pen, color);
      break;
    case RECT:
      return scene.addRect(0, 0, 100, 100, pen, color);
      break;
    case TRIANGLE:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(0., 0.));
        p.append(QPointF(0., 100.));
        p.append(QPointF(100., 50.));
        return scene.addPolygon(p, pen, color);
      }
      break;
    case LOSDOWN:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(0., 0.));
        p.append(QPointF(25., 100.));
        p.append(QPointF(100., 100.));
        p.append(QPointF(75., 0.));
        return scene.addPolygon(p, pen, color);
      }
      break;
    case LOSUP:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(25., 0.));
        p.append(QPointF(0., 100.));
        p.append(QPointF(75., 100.));
        p.append(QPointF(100., 0.));
        return scene.addPolygon(p, pen, color);
      }
      break;
    case TRIANGLE_BEHIND:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(50., 0.));
        p.append(QPointF(0., 100.));
        p.append(QPointF(50., 80.));
        p.append(QPointF(100., 100.));
        return scene.addPolygon(p, pen, color);
      }
      break;
    case TRIANGLE_LEFT:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(0., 50.));
        p.append(QPointF(100., 0.));
        p.append(QPointF(80., 50.));
        p.append(QPointF(100., 100.));
        return scene.addPolygon(p, pen, color);
      }
      break;
    case TRIANGLE_RIGHT:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(0., 0.));
        p.append(QPointF(100., 50.));
        p.append(QPointF(0., 100.));
        p.append(QPointF(20., 50.));
        return scene.addPolygon(p, pen, color);
      }
      break;
    case TRIANGLE_BEHIND_SQUARE:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(50., 0.));
        p.append(QPointF(0., 100.));
        p.append(QPointF(50., 80.));
        p.append(QPointF(100., 100.));
        QGraphicsPolygonItem *t = new QGraphicsPolygonItem(p);
        t->setPen(pen);
        t->setBrush(color);
        QGraphicsRectItem *r = new QGraphicsRectItem(40., -10., 20, 20);
        r->setPen(pen);
        r->setBrush(color);
        QList<QGraphicsItem *> l;
        l.append(t);
        l.append(r);
        return scene.createItemGroup(l);
      }
      break;
    case TRIANGLE_LEFT_SQUARE:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(50., 0.));
        p.append(QPointF(0., 100.));
        p.append(QPointF(50., 80.));
        p.append(QPointF(100., 100.));
        QGraphicsPolygonItem *t = new QGraphicsPolygonItem(p);
        t->setPen(pen);
        t->setBrush(color);
        QGraphicsRectItem *r = new QGraphicsRectItem(10., 40., 20, 20);
        r->setPen(pen);
        r->setBrush(color);
        QList<QGraphicsItem *> l;
        l.append(t);
        l.append(r);
        return scene.createItemGroup(l);
      }
      break;
    case TRIANGLE_RIGHT_SQUARE:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(50., 0.));
        p.append(QPointF(0., 100.));
        p.append(QPointF(50., 80.));
        p.append(QPointF(100., 100.));
        QGraphicsPolygonItem *t = new QGraphicsPolygonItem(p);
        t->setPen(pen);
        t->setBrush(color);
        QGraphicsRectItem *r = new QGraphicsRectItem(70., 40., 20, 20);
        r->setPen(pen);
        r->setBrush(color);
        QList<QGraphicsItem *> l;
        l.append(t);
        l.append(r);
        return scene.createItemGroup(l);
      }
      break;
    }
  return NULL;
}

void BuildGrahpicsItem::addTextToItem(const std::string &text
    , QGraphicsItem *item) const
{
  QGraphicsTextItem *iText = new QGraphicsTextItem(
        tr("%1").arg(text.c_str())
        , item);
  iText->setDefaultTextColor(getColorOfText(text));
  QFont font;
  font.setBold(true);
  iText->setFont(font);
  centerBInA(item, iText);
}

const QColor &BuildGrahpicsItem::getColorOfFigures(const std::string &id) const
{
  return figuresResults.at(id);
}

const QColor &BuildGrahpicsItem::getColorOfText(const std::string &id) const
{
  if(id.find("Up") != std::string::npos)
    return figuresResults.at("textUp");
  if(id.find("Down") != std::string::npos)
    return figuresResults.at("textDown");
  if(id.find("Empty") != std::string::npos)
    return figuresResults.at("textEmpty");
  if(id.find("Wall") != std::string::npos)
    return figuresResults.at("textWall");
  return figuresResults.at("textDefault");
}


QGraphicsPolygonItem *BuildGrahpicsItem::drawAgent(QGraphicsScene &scene) const
{
  QPolygonF p;
  p.clear();
  p.append(QPointF(0., 0.));
  p.append(QPointF(50., 100.));
  p.append(QPointF(100., 0.));
  p.append(QPointF(50., 20.));
  return scene.addPolygon(p, QPen(Qt::black, 6), QBrush(Qt::blue) );
}

QGraphicsRectItem *BuildGrahpicsItem::drawLeftRectAgent(
    QGraphicsScene &scene) const
{
  return scene.addRect(70., 40., 20, 20, QPen(Qt::black, 4),
                          QBrush(Qt::green));
}

QGraphicsRectItem *BuildGrahpicsItem::drawRightRectAgent(
    QGraphicsScene &scene) const
{
  return scene.addRect(10., 40., 20, 20, QPen(Qt::black, 4),
                         QBrush(Qt::yellow));
}

QGraphicsRectItem *BuildGrahpicsItem::drawBehindRectAgent(
    QGraphicsScene &scene) const
{
  return scene.addRect(40., 90., 20., 20., QPen(Qt::black, 4),
                          QBrush(Qt::cyan));
}

QGraphicsItemGroup *BuildGrahpicsItem::createAgentGroupAndScale(
    QGraphicsItem *agent
    , QGraphicsRectItem *leftItem
    , QGraphicsRectItem *rightItem
    , QGraphicsRectItem *behindItem
    , QGraphicsScene &scene) const
{
  leftItem->setVisible(false);
  rightItem->setVisible(false);
  behindItem->setVisible(false);
  QList<QGraphicsItem *> list;
  list.append(agent);
  list.append(leftItem);
  list.append(rightItem);
  list.append(behindItem);
  QGraphicsItemGroup *group = scene.createItemGroup(list);
//  group->setScale(0.014);
  group->setBoundingRegionGranularity(1.);
  group->setTransformOriginPoint(0.1, -0.45);
  return group;
}



void BuildGrahpicsItem::changeFiguresDefinition(
    const std::map<std::string, BuildGrahpicsItem::FIGURE> &figuresExperiences
    , const std::map<std::string, QColor> &figuresResults)
{
  this->figuresExperiences = figuresExperiences;
  this->figuresResults = figuresResults;
}

void BuildGrahpicsItem::paintPhenomena(QPainter *painter,
                                       const UsageTable *noema) const
{
  QPolygonF p;
  qreal nbElements = 0;
  qreal height = 0.;
  qreal elementSize = 100.;
  qreal space = 20.;
  qreal spaceTop = 90.;
  qreal penSize = 5.;
  qreal width = 0;
  QPen pen(Qt::black, 5);
  painter->setPen(pen);
  painter->setBrush(Qt::white);
  if(noema->getStatus() == UsageTable::STATUS::PERSISTENT)
    {
      for(auto const & pair
          : noema->getPostInteractions())
        {
          const Signature *signature = pair.second;
          if(signature->isConfirm()
             && pair.first != noema->getInteraction()->getId())
            {
              nbElements ++;
            }
        }
      height =
          spaceTop
          + elementSize
          + 2 * penSize
          + space
          + (nbElements * elementSize)
          + (nbElements * (2 * penSize))
          + (nbElements * space)
          + (nbElements == 0 ? 0 : space) ;
      width = 2 * space
          + elementSize
          + 2 * penSize;
      p.append(QPointF(- width / 2., - height / 2. + 5 * space));
      p.append(QPointF(- width /2., height / 2.));
      p.append(QPointF(width /2., height / 2.));
      p.append(QPointF(width /2., - height / 2. + 5 * space));
      p.append(QPointF(0, -height / 2.));
      painter->drawPolygon(p);
      qreal lineY = - height / 2.
          + spaceTop
          + space
          + penSize * 2
          + elementSize;
      painter->drawLine(- width / 2.
                        , lineY
                        , width / 2.
                        , lineY);
      qreal posY = - height / 2. + spaceTop + space + elementSize
          + penSize * 3 + space + elementSize / 2.;
      for(auto const & pair
          : noema->getPostInteractions())
        {
          const Signature *signature = pair.second;
          if(signature->isConfirm()
             && pair.first != noema->getInteraction()->getId())
            {
              paintExperience(painter
                              , pair.first
                              , 0
                              , posY);
              posY += elementSize + space + penSize;
            }
        }
    }
  paintExperience(painter, noema->getInteraction()->getId(),
                  0, - height / 2. + spaceTop + elementSize / 2.);
}

void BuildGrahpicsItem::paintUnknownPhenomena(
    QPainter *painter
    , const QList<const UsageTable *> &noemas) const
{
  QPen pen(Qt::black, 5);
  painter->setPen(pen);
  painter->setBrush(Qt::white);
  qreal nbElements = noemas.size();
  qreal elementSize = 100.;
  qreal space = 20.;
  qreal penSize = 10.;

  if(nbElements == 0)
    nbElements ++;
  qreal width = nbElements * elementSize
      + nbElements * 2 * penSize
      + (nbElements - 1) * space
      + 8 * space;
  qreal height = elementSize + 5 * space;

  painter->drawEllipse(- width / 2
                       , - height / 2
                       , width
                       , height);
  qreal posX = - width / 2 + 4 * space + penSize + elementSize / 2.;
  for(auto const & pair : noemas)
    {
      paintExperience(painter
                      , pair->getInteraction()->getId()
                      , posX
                      , 0);
      posX += elementSize + space;
    }
}

void BuildGrahpicsItem::paintExperience(QPainter *painter
                                        , const std::string &id
                                        , qreal x
                                        , qreal y) const
{
  QColor color = figuresResults.at(id);
  FIGURE fig = figuresExperiences.at(id);
  QPen pen(Qt::black, 10.);
  painter->setPen(pen);
  painter->setBrush(color);
  qreal dec = 50;
  switch(fig)
    {
    case SPHERE:
      painter->drawEllipse(x - dec, y - dec, 100., 100.);
      break;
    case RECT:
      painter->drawRect(x - dec, y - dec, 100., 100.);
      break;
    case TRIANGLE:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(x - dec, y - dec));
        p.append(QPointF(x - dec, y + 100. - dec));
        p.append(QPointF(x + 100. - dec, y + 50. - dec));
        painter->drawPolygon(p);
      }
      break;
    case LOSDOWN:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(x - dec, y - dec));
        p.append(QPointF(x + 250 - dec, y + 100. - dec));
        p.append(QPointF(x + 100. - dec, y + 100. - dec));
        p.append(QPointF(x + 75. - dec, y + 00. - dec));
        painter->drawPolygon(p);
      }
      break;
    case LOSUP:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(x + 25. - dec, y - dec));
        p.append(QPointF(x - dec, y + 100. - dec));
        p.append(QPointF(x + 75. - dec, y + 100. - dec));
        p.append(QPointF(x + 100. - dec, y + 0. - dec));
        painter->drawPolygon(p);
      }

      break;
    case TRIANGLE_BEHIND:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(x - dec + 50., y - dec));
        p.append(QPointF(x - dec, y + 100. - dec));
        p.append(QPointF(x - dec + 50., y - dec + 70.));
        p.append(QPointF(x - dec + 100., y + 100. - dec));
        painter->drawPolygon(p);
      }
      break;
    case TRIANGLE_LEFT:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(x - dec, y - dec + 50.));
        p.append(QPointF(x - dec + 100, y - dec));
        p.append(QPointF(x - dec + 70., y - dec + 50.));
        p.append(QPointF(x - dec + 100., y - dec + 100.));
        painter->drawPolygon(p);
      }
      break;
    case TRIANGLE_RIGHT:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(x - dec + 100., y - dec + 50.));
        p.append(QPointF(x - dec, y - dec + 100.));
        p.append(QPointF(x - dec + 30., y - dec + 50.));
        p.append(QPointF(x - dec, y - dec ));
        painter->drawPolygon(p);

      }
      break;
    case TRIANGLE_BEHIND_SQUARE:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(x - dec + 50., y - dec));
        p.append(QPointF(x - dec, y + 100. - dec));
        p.append(QPointF(x - dec + 50., y - dec + 70.));
        p.append(QPointF(x - dec + 100., y + 100. - dec));
        painter->drawPolygon(p);
        pen.setWidth(1);
        painter->setPen(pen);
        painter->drawRect(x - dec + 35., y - dec, 30., 30.);
      }
      break;
    case TRIANGLE_LEFT_SQUARE:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(x - dec + 50., y - dec));
        p.append(QPointF(x - dec, y + 100. - dec));
        p.append(QPointF(x - dec + 50., y - dec + 70.));
        p.append(QPointF(x - dec + 100., y + 100. - dec));
        painter->drawPolygon(p);
        pen.setWidth(1);
        painter->setPen(pen);
        painter->drawRect(x - dec , y - dec + 30., 30., 30.);
      }
      break;
    case TRIANGLE_RIGHT_SQUARE:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(x - dec + 50., y - dec));
        p.append(QPointF(x - dec, y + 100. - dec));
        p.append(QPointF(x - dec + 50., y - dec + 70.));
        p.append(QPointF(x - dec + 100., y + 100. - dec));
        painter->drawPolygon(p);
        pen.setWidth(1);
        painter->setPen(pen);
        painter->drawRect(x - dec + 70., y - dec + 30., 30., 30.);
      }
      break;
    }
}
