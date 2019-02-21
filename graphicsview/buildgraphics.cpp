#include "buildgraphics.hpp"
#include <usagetable.hpp>
#include <interaction.hpp>

#include <string>
#include <QFile>
#include <QPen>
#include <QGraphicsScene>
#include <QPainter>

BuildGraphics::BuildGraphics()
{

}

BuildGraphics::~BuildGraphics()
{

}

void BuildGraphics::readSettings(const QString &filename)
{
  QFile file(filename);
  if(file.open(QIODevice::ReadOnly))
    {
      colors_.clear();
      figures_.clear();
      bool color = true;
      while(!file.atEnd())
        {
          QByteArray b = file.readLine();
          if(b.isEmpty())
            continue;
          if(b.at(0) == '#')
            continue;
          if(b.at(0) == '[')
            {
              if(b.contains("color"))
                color = true;
              else color = false;
              continue;
            }
          if(color)
            {
              std::string str(b.left(b.indexOf('=')).constData());
              QString sColor(b.right(b.size() - (b.indexOf('=') + 1)));
              if(sColor.endsWith('\n'))
                  sColor.remove(sColor.size() - 1, 1);
              colors_[str] = sColor;
            }
          else
            {
              std::string str(b.left(b.indexOf('=')).constData());
              QString fig(b.right(b.size() - (b.indexOf('=') + 1)));
              if(fig.endsWith('\n'))
                fig.remove(fig.size() - 1, 1);
              if(fig == "triangle")
                figures_[str] = BuildGraphics::TRIANGLE;
              else if(fig == "rectangle")
                figures_[str] = BuildGraphics::RECT;
              else if(fig == "losange down")
                figures_[str] = BuildGraphics::LOSDOWN;
              else if(fig == "losange up")
                figures_[str] = BuildGraphics::LOSUP;
              else if(fig == "triangle behind")
                figures_[str] = BuildGraphics::TRIANGLE_BEHIND;
              else if(fig == "triangle left")
                figures_[str] = BuildGraphics::TRIANGLE_LEFT;
              else if(fig == "triangle right")
                figures_[str] = BuildGraphics::TRIANGLE_RIGHT;
              else if(fig == "triangle behind square")
                figures_[str] = BuildGraphics::TRIANGLE_BEHIND_SQUARE;
              else if(fig == "triangle left square")
                figures_[str] = BuildGraphics::TRIANGLE_LEFT_SQUARE;
              else if(fig == "triangle right square")
                figures_[str] = BuildGraphics::TRIANGLE_RIGHT_SQUARE;
              else
                figures_[str] = BuildGraphics::SPHERE;
            }
        }
    }
}

void BuildGraphics::centerBInA(
    QGraphicsItem *itemA
    , QGraphicsItem *itemB) const
{
  itemB->setParentItem(itemA);
  itemB->setPos(itemA->boundingRect().center().x()
                - itemB->boundingRect().center().x()
                , itemA->boundingRect().center().y()
                - itemB->boundingRect().center().y());
}

QGraphicsItem *BuildGraphics::createExperience(QGraphicsScene &scene
    , const std::string &id, const qreal width, const qreal height) const
{
  QColor color(colors_.value(id));
  FIGURE fig = figures_.at(id);
  QPen pen(Qt::black, 6);
  switch(fig)
    {
    case SPHERE:
      return scene.addEllipse(0, 0, width, height, pen, color);
      break;
    case RECT:
      return scene.addRect(0, 0, width, height, pen, color);
      break;
    case TRIANGLE:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(0., 0.));
        p.append(QPointF(0., height));
        p.append(QPointF(width, height / 2.));
        return scene.addPolygon(p, pen, color);
      }
      break;
    case LOSDOWN:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(0., 0.));
        p.append(QPointF(width / 4., height));
        p.append(QPointF(width, height));
        p.append(QPointF(width * (3. / 4.), 0.));
        return scene.addPolygon(p, pen, color);
      }
      break;
    case LOSUP:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(width / 4., 0.));
        p.append(QPointF(0., height));
        p.append(QPointF(width * (3. / 4.), height));
        p.append(QPointF(width, 0.));
        return scene.addPolygon(p, pen, color);
      }
      break;
    case TRIANGLE_BEHIND:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(width / 2., 0.));
        p.append(QPointF(0., height));
        p.append(QPointF(width / 2., height * (8. / 10.)));
        p.append(QPointF(width, height));
        return scene.addPolygon(p, pen, color);
      }
      break;
    case TRIANGLE_LEFT:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(0., height / 2.));
        p.append(QPointF(width, 0.));
        p.append(QPointF(width * (8. / 10.), height / 2.));
        p.append(QPointF(width, height));
        return scene.addPolygon(p, pen, color);
      }
      break;
    case TRIANGLE_RIGHT:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(0., 0.));
        p.append(QPointF(width, height / 2.));
        p.append(QPointF(0., height));
        p.append(QPointF(width * (2. / 10.), height / 2.));
        return scene.addPolygon(p, pen, color);
      }
      break;
    case TRIANGLE_BEHIND_SQUARE:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(width / 2., 0.));
        p.append(QPointF(0., height));
        p.append(QPointF(width / 2., height * (8. / 10.)));
        p.append(QPointF(width, height));
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
        p.append(QPointF(width / 2., 0.));
        p.append(QPointF(0., height));
        p.append(QPointF(width / 2., height * (8. / 10.)));
        p.append(QPointF(width, height));
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
        p.append(QPointF(width / 2., 0.));
        p.append(QPointF(0., height));
        p.append(QPointF(width / 2., height * (8. / 10.)));
        p.append(QPointF(width, height));
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
    case COMPOSITE:
      {
        QPolygonF p;
        p.clear();
        qreal x = 50.;
        qreal y = 50.;
        qreal dec = 50.;
        p.append(QPointF(x, y - dec));
        p.append(QPointF(x - dec / 4., y - dec / 4.));
        p.append(QPointF(x - dec, y));
        p.append(QPointF(x - dec / 4., y + dec / 4.));
        p.append(QPointF(x, y + dec));
        p.append(QPointF(x + dec / 4., y + dec / 4.));
        p.append(QPointF(x + dec, y));
        p.append(QPointF(x + dec / 4., y - dec / 4.));
        return scene.addPolygon(p, pen, color);
      }
    }
  return NULL;
}

void BuildGraphics::addTextToItem(const std::string &text
    , QGraphicsItem *item) const
{
  QGraphicsTextItem *iText = new QGraphicsTextItem(
        QString("%1").arg(text.c_str())
        , item);
  iText->setDefaultTextColor(getColorOfText(text));
  QFont font;
  font.setBold(true);
  iText->setFont(font);
  centerBInA(item, iText);
}

const QColor BuildGraphics::getColorOfFigures(const std::string &id) const
{
  return QColor(colors_.value(id));
}

const QColor BuildGraphics::getColorOfText(const std::string &id) const
{
  if(colors_.find("textOn" + colors_.value(id).toStdString()) != colors_.end())
    return QColor(colors_.value("textOn" + colors_.value(id).toStdString())
          );
  return QColor(colors_.value("textDefault"));
}


QGraphicsPolygonItem *BuildGraphics::drawAgent(QGraphicsScene &scene) const
{
  QPolygonF p;
  p.clear();
  p.append(QPointF(0., 0.));
  p.append(QPointF(50., 100.));
  p.append(QPointF(100., 0.));
  p.append(QPointF(50., 20.));
  return scene.addPolygon(p, QPen(Qt::black, 6), QBrush(Qt::blue) );
}

QGraphicsRectItem *BuildGraphics::drawLeftRectAgent(
    QGraphicsScene &scene) const
{
  return scene.addRect(70., 40., 20, 20, QPen(Qt::black, 4),
                          QBrush(Qt::green));
}

QGraphicsRectItem *BuildGraphics::drawRightRectAgent(
    QGraphicsScene &scene) const
{
  return scene.addRect(10., 40., 20, 20, QPen(Qt::black, 4),
                         QBrush(Qt::yellow));
}

QGraphicsRectItem *BuildGraphics::drawBehindRectAgent(
    QGraphicsScene &scene) const
{
  return scene.addRect(40., 90., 20., 20., QPen(Qt::black, 4),
                          QBrush(Qt::cyan));
}

QGraphicsItemGroup *BuildGraphics::createAgentGroupAndScale(
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

QRectF BuildGraphics::paintPhenomena(QPainter *painter,
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
  return QRectF(- width / 2. - penSize
                , - penSize - height / 2.
                , (width / 2. + penSize) * 2.
                , (penSize + height / 2.) * 2. );
}

void BuildGraphics::paintUnknownPhenomena(
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

void BuildGraphics::paintUsageTable(QPainter *painter
                                    , const UsageTable *noema) const
{
  qreal elementSize = 100.;
  qreal space = 20.;
  qreal penSize = 5.;
  qreal width = 0;
  qreal height = 0.;
  qreal nbElementsSignatures = noema->getPostInteractions().nbElements();
  width = space
      + elementSize * 2. // preInteraction
      + penSize * 2. // interaction left + interaction right
      // + usageTable left +  usageTable right
      + 2. * space // between interaction and usage table and the next element
      + elementSize * 2. // postInteraction
      + penSize * 2. // between interaction
      // and usage table and the end of element
      + 2. * space
      + elementSize * 2.
      + 2. * space
      + penSize * 4.;
  height = space
      + elementSize
      + 2. * penSize
      + space
      + nbElementsSignatures * elementSize
      + nbElementsSignatures * space
      + nbElementsSignatures * penSize * 2.;
  // Draw the including rect
  painter->setPen(QPen(Qt::black, 5.));
  painter->setBrush(QBrush(Qt::white));
  painter->drawRect(- width / 2.
                    , - height / 2.
                    , width
                    , height);
  paintExperience(painter
                  , noema->getInteraction()->getId()
                  , - elementSize - space / 2.
                  , - height / 2. + space + elementSize / 2.);
  paintUsage(painter
             , noema->getNbEnacted()
             , noema->getNbIntented()
             , space / 2.
             , -height / 2. + space + elementSize / 2.);
  if(noema->getStatus() == UsageTable::STATUS::PERSISTENT)
    {
      paintExperience(painter
                      , noema->getInteraction()->getId()
                      , elementSize + space + space / 2.
                      , -height / 2.
                      + space
                      + elementSize / 2.);
      paintText(painter
                , "P"
                , - ( elementSize + 2. * space + elementSize )
                , -height / 2.
                + space
                + elementSize / 2.);

    }
  else if(noema->getBelieve() != NULL)
    {
      paintExperience(painter
                      , noema->getBelieve()->getInteraction()->getId()
                      , elementSize + space + space / 2.
                      , -height / 2.
                      + space
                      + elementSize / 2.);
      paintText(painter
                , "S B"
                , - ( elementSize + 2. * space + elementSize)
                , -height / 2.
                + space
                + elementSize / 2.);
    }
  else
    paintText(painter
              , "S"
              , - ( elementSize + 2. * space + elementSize)
              , -height / 2.
              + space
              + elementSize / 2.);
  if(noema->getInteraction()->getBestNextInteraction() != NULL)
    paintExperience(painter
                    , noema->getInteraction()->getBestNextInteraction()->getId()
                    , elementSize * 2 + space * 2 + space / 2.
                    , - height / 2.
                    + space
                    + elementSize / 2.);
  qreal posX = -width / 2. + space + elementSize / 2.;
  qreal posY = -height / 2.
      + 2 * space
      + elementSize
      + elementSize / 2.
      + space;
  for(auto const & it : noema->getPreInteractions())
    {
      const Signature *sign = it.second;
      paintExperience(painter
                      , sign->getInteraction()->getId()
                      , posX
                      , posY);
      paintUsage(painter
                 , sign->getNbEnacted()
                 , sign->getNbIntended()
                 , posX + elementSize + space
                 , posY);
      posY += elementSize + space;
    }
  posY = -height / 2.
      + 2 * space
      + elementSize
      + elementSize / 2.
      + space;
  posX += elementSize * 2 + space * 2 + penSize * 4;
  for(auto const & it : noema->getEnacted())
    {
      const Signature *sign = it.second;
      paintExperience(painter
                      , sign->getInteraction()->getId()
                      , posX
                      , posY);
      paintUsage(painter
                 , sign->getNbEnacted()
                 , sign->getNbIntended()
                 , posX + elementSize + space
                 , posY);
      posY += elementSize + space;
    }
  posY = -height / 2.
      + 2 * space
      + elementSize
      + elementSize / 2.
      + space;
  posX += elementSize * 2 + space * 2 + penSize * 4;
  for(auto const & it : noema->getPostInteractions())
    {
      const Signature *sign = it.second;
      paintExperience(painter
                      , sign->getInteraction()->getId()
                      , posX
                      , posY);
      paintUsage(painter
                 , sign->getNbEnacted()
                 , sign->getNbIntended()
                 , posX + elementSize + space
                 , posY);
      posY += elementSize + space;
    }
}

void BuildGraphics::paintUsage(QPainter *painter
                               , const int nbEnacted
                               , const int nbIntended
                               , const qreal x
                               , const qreal y) const
{
  QColor color(Qt::white);
  QPen pen(Qt::black, 10.);
  qreal dec = 50.;
  qreal size = 100.;
  painter->setPen(pen);
  painter->setBrush(color);
  painter->setFont(QFont("sans-serif", 30));
  painter->drawRect(x - dec, y - dec, size, size);
  painter->drawText(x - dec
                    , y - dec
                    , size
                    , size
                    , Qt::AlignCenter
                    , QString("%1\n%2")
                    .arg(nbIntended)
                    .arg(nbEnacted));
}

void BuildGraphics::paintText(QPainter *painter
                               , const QString &text
                               , const qreal x
                               , const qreal y) const
{
  QColor color(Qt::white);
  QPen pen(Qt::black, 10.);
  qreal dec = 50.;
  qreal size = 100.;
  painter->setPen(pen);
  painter->setBrush(color);
  painter->setFont(QFont("sans-serif", 30));
  painter->drawRect(x - dec, y - dec, size, size);
  painter->drawText(x - dec
                    , y - dec
                    , size
                    , size
                    , Qt::AlignCenter
                    , text);
}

void BuildGraphics::paintExperience(QPainter *painter
                                    , const std::string &id
                                    , qreal x
                                    , qreal y) const
{
  QColor color(colors_.value(id));
  FIGURE fig = COMPOSITE;
  if(figures_.find(id) != figures_.end())
    fig = figures_.at(id);
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
    case COMPOSITE:
      {
        QPolygonF p;
        p.clear();
        p.append(QPointF(x, y - dec));
        p.append(QPointF(x - dec / 4., y - dec / 4.));
        p.append(QPointF(x - dec, y));
        p.append(QPointF(x - dec / 4., y + dec / 4.));
        p.append(QPointF(x, y + dec));
        p.append(QPointF(x + dec / 4., y + dec / 4.));
        p.append(QPointF(x + dec, y));
        p.append(QPointF(x + dec / 4., y - dec / 4.));
        painter->drawPolygon(p);
      }
      break;
    }
}

