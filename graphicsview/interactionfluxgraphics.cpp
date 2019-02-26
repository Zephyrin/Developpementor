#include "interactionfluxgraphics.hpp"
#include <interaction.hpp>
#include <usagetable.hpp>
#include <mood.hpp>
#include <simulation.h>
#include <QScrollBar>
#include <QTimer>

InteractionFluxGraphics::InteractionFluxGraphics(
    const BuildGraphics &build
    , const QString &title
    , const QString &configName
    , const Simulation *simulation
    , QAction *menuAction
    , QWidget *parent)
  :
    DockWidgetGraphicsView(build
                           , title
                           , configName
                           , menuAction
                           , false
                           , false
                           , parent)
  , timeLine(NULL)
  , excitement(0)
  , timeItem(NULL)
  , spaceBetweenNumber(10)
{
  font.setBold(false);
  font.setPixelSize(120);
  font.setStyleHint(QFont::SansSerif);
  addNumberOfLine();
  if(simulation)
  {
  connect(simulation, SIGNAL(simulationDone())
          , this, SLOT(simulationStop()));
  connect(simulation, SIGNAL(intendedEnactedSignal(
                               const Interaction*
                               , const Interaction*
                               , const std::vector<const UsageTable*> *
                               , int
                               , int))
          , this, SLOT(addInteractions(
                         const Interaction*
                         , const Interaction*
                         , const std::vector<const UsageTable*> *
                         , int
                         , int)));
  }
  connect(this, SIGNAL(newItemsInView())
          , this, SLOT(moveScrollBarToRight()));
}

InteractionFluxGraphics::~InteractionFluxGraphics()
{

}

void InteractionFluxGraphics::addInteractions(
    const Interaction *intended,
    const Interaction *enacted
    , const std::vector<const UsageTable *> *usagesTablesModified
    , const int moodInt
    , const int step)
{
  qreal posX = view->scene()->itemsBoundingRect().right() + 10;
  if(step % 10 == 0 || timeItem == NULL)
    {
      timeItem = view->scene()->addText(tr("%1").arg(step)
                               , font);
      timeItem->setPos(posX, 0);
    }

  QGraphicsItem *itemIntended = addInteractionsInLine(intended);
  QGraphicsItem *itemIntendedBox = addBoxToInteraction(itemIntended);
  QGraphicsItem *itemEnacted = addInteractionsInLine(enacted);
  QGraphicsItem *itemEnactedBox = addBoxToInteraction(itemEnacted,
                                                      enacted == intended ?
                                                        QBrush("white")
                                                      : QBrush("orange"));

  itemIntendedBox->setZValue(-4);
  itemIntended->setZValue(-3);
  itemEnactedBox->setZValue(-2);
  itemEnacted->setZValue(-1);
  itemIntended->setPos(posX + 5,
                      numberLineList.at(1)->pos().y() + 7.5);
  itemIntendedBox->setPos(posX, numberLineList.at(1)->pos().y());
  itemEnacted->setPos(posX + 25,
                      numberLineList.at(2)->pos().y() + 7.5);
  itemEnactedBox->setPos(posX + 20,
                         numberLineList.at(2)->pos().y());
  addValence(enacted->getValence()
             , posX + itemIntendedBox->boundingRect().width());
  QPolygonF p;
  p.append(QPointF(0., 25.));
  p.append(QPointF(0., 50.));
  p.append(QPointF(100., 50.));
  p.append(QPointF(100., 25.));
  p.append(QPointF(50., 0.));
  QGraphicsPolygonItem *itemPhenomena = view->scene()->addPolygon(
        p
        , QPen(Qt::black, 5));

  if(enacted->getUsageTable()->getBelieve() == NULL)
    {
      if(enacted->getUsageTable()->getStatus()
         == UsageTable::STATUS::PERSISTENT)
        itemPhenomena->setBrush(
              build.getColorOfFigures(enacted->getId()));
      else
        itemPhenomena->setBrush(QBrush(QColor(Qt::gray)));
    }
  else
    itemPhenomena->setBrush(
          build.getColorOfFigures(
            enacted->getUsageTable()->getBelieve()->getInteraction()->getId()));
  itemPhenomena->setPos(posX + 20, numberLineList.at(3)->pos().y() + 25);


  MOOD mood = (MOOD)moodInt;
  QGraphicsItem *itemMood = addMood(mood);
  itemMood->setPos(itemEnacted->pos().x()
                   + itemEnacted->boundingRect().center().x()
                   - itemMood->boundingRect().center().x()
                   , numberLineList.at(4)->pos().y());
  if(moodAhasB(mood, MOOD::CURIOUS))
    itemMood->setPos(itemMood->pos().x()
                     , numberLineList.at(4)->pos().y() - 28);
  qreal posY = numberLineList.at(5)->pos().y();
  if(usagesTablesModified != NULL)
    {
      for(const UsageTable *usageTableModified : *usagesTablesModified)
        {
          QGraphicsItem *itemNoema = NULL;
          if(usageTableModified->getStatus() == UsageTable::STATUS::PERSISTENT)
            {
              itemNoema = addModifiedUsageTablePersistant(
                    usageTableModified);

            }
          else if(usageTableModified->getStatus()
                  == UsageTable::STATUS::UNPERSISTENT)
            {
              itemNoema = addModifiedUsageTableUnPersistant(
                    usageTableModified);
            }
          itemNoema->setPos(posX + 20
                            , posY);
          posY += itemNoema->boundingRect().height() + 10;
        }
    }
  view->scene()->setSceneRect(view->scene()->itemsBoundingRect());

  QTimer::singleShot(1, this, SLOT(moveScrollBarToRight()));
}

void InteractionFluxGraphics::moveScrollBarToRight()
{
  view->horizontalScrollBar()->setValue(view->horizontalScrollBar()->maximum());
}

void InteractionFluxGraphics::addValence(const int valence, const qreal posX)
{
  lineValence->setLine(
        lineValence->line().x1()
        , lineValence->line().y1()
        , posX
        - numberLineList.at(0)->boundingRect().width()
        - numberLineList.at(0)->pos().x()
//        - 115
        , lineValence->line().y2());
  QGraphicsItem *line = view->scene()->addLine(0.
                         , -15.
                         , 0.
                         , 15.);
  line->setPos(posX + 5.
              , numberLineList.at(0)->pos().y() + 50);
  if(valence == 0)
    return;

  QColor color = build.getColorOfFigures(valence > 0 ? "positiveValence"
                                                     : "negativeValence");
  QGraphicsRectItem *item = view->scene()->addRect(0, 0, 150, 50, QPen(),
                                          QBrush(color));
  if(valence > 0)
    item->setPos(posX - 145, numberLineList.at(0)->pos().y());
  else
    item->setPos(posX - 145, numberLineList.at(0)->pos().y() + 50);
  item->setToolTip(tr("Valence : %1").arg(valence));
}

void InteractionFluxGraphics::simulationStop()
{
    userClear();
}
void InteractionFluxGraphics::userClear()
{
  view->scene()->clear();
  addNumberOfLine();
  view->scene()->setSceneRect(view->scene()->itemsBoundingRect());

  timeItem = NULL;
}

QGraphicsItem *InteractionFluxGraphics::addBoxToInteraction(
    const QGraphicsItem *interaction
    , const QBrush b)
{
  QPolygon p;
  p.clear();
  int height = interaction->boundingRect().height() + 10;
  int width = interaction->boundingRect().width() + 10;
  int widthPoint = width + 50;
  p.append(QPoint(0, 0));
  p.append(QPoint(0, height));
  p.append(QPoint(width, height));
  p.append(QPoint(widthPoint, height / 2));
  p.append(QPoint(width, 0));
  QGraphicsPolygonItem *box = view->scene()->addPolygon(p);
  if(b.color() != "white")
    box->setOpacity(0.6);
  box->setBrush(b);
  return box;
}

QGraphicsItem * InteractionFluxGraphics::addInteractionsInLine(
    const Interaction *interaction)
{
  QGraphicsItem *item;
  if(interaction->isPrimitive())
    {
      item = build.createExperience(*(view->scene()), interaction->getId()
                                    , 90., 90.);
      item->setToolTip(tr("%1\nValence : %2").arg(interaction->getId().c_str())
                       .arg(interaction->getValence()));
      return item;
    }
  QGraphicsItem *child1 = addInteractionsInLine(
        interaction->getPreInteraction());
  QGraphicsItem *child2 = addInteractionsInLine(
        interaction->getPostInteraction());
  child2->setPos(child1->boundingRect().width() + 50, 0);
  QList<QGraphicsItem *> list;

  list.append(child1);
  list.append(child2);
  return view->scene()->createItemGroup(list);
}

QGraphicsItem *InteractionFluxGraphics::addMood(const MOOD mood)
{
  QGraphicsItem *item;
  QList<QGraphicsItem *> lItems;
  if(moodAhasB(mood, MOOD::CURIOUS))
    {
      item = view->scene()->addText("?", font);
      lItems.push_back(item);
    }
  if(moodAhasB(mood, MOOD::EXCITED))
    {
      excitement ++;
      item = view->scene()->addRect(0, 0, 100, excitement * 10, QPen()
                           , QBrush(QColor(Qt::black)));
      lItems.push_back(item);
    }
  else
    {
      excitement = 0;
    }
  return view->scene()->createItemGroup(lItems);
}

QGraphicsItem *InteractionFluxGraphics::addModifiedUsageTablePersistant(
    const UsageTable *noema)
{
  QList<QGraphicsItem *> lItems;
  QGraphicsItem *item = build.createExperience(
        *(view->scene())
        , noema->getInteraction()->getId());
  lItems.push_back(item);
  QGraphicsItem *postItem = NULL;
  QList<QGraphicsItem *> lItemsPost;
  for(auto const & pair
      : noema->getPostInteractions())
    {
      const Signature *signature = pair.second;
      if(signature->isConfirm()
         && pair.first != noema->getInteraction()->getId())
        {

          qreal posY = 0;
          if(postItem == NULL)
            posY = item->pos().y() + item->boundingRect().height() + 40;
          else
            posY = postItem->pos().y() + postItem->boundingRect().height() + 5;
          postItem = build.createExperience(*(view->scene()), pair.first);
          postItem->setPos(0, posY);
          lItems.push_back(postItem);
          lItemsPost.push_back(postItem);
        }
    }

  QGraphicsItem *items = NULL;
  qreal posYDown = item->pos().y() + item->boundingRect().height() + 10;
  if(postItem != NULL)
    {
      items = view->scene()->createItemGroup(lItemsPost);
      posYDown = postItem->pos().y() + postItem->boundingRect().height()
          + item->boundingRect().height();
      posYDown *= 0.6;
    }
  else
    {
      items = item;
    }
  QPolygonF p;
  p.clear();

  p.append(QPointF(items->boundingRect().center().x(), 0.));
  p.append(QPointF(0., item->boundingRect().height() / 3.));
  p.append(QPointF(0.
                   , posYDown
                   ));
  p.append(QPointF(items->boundingRect().center().x()
                   , posYDown
                   ));
  p.append(QPointF(items->boundingRect().right()
                   , posYDown
                   ));
  p.append(QPointF(items->boundingRect().right()
                   , item->boundingRect().height() / 3.));
  p.append(QPointF(items->boundingRect().center().x(), 0.));

  QGraphicsItem *boundRect = view->scene()->addPolygon(p, QPen(Qt::black, 6));
  lItems.push_back(boundRect);
  if(items != item)
    {
      lItemsPost.append(item);
      items = view->scene()->createItemGroup(lItemsPost);

    }

  items->setScale(0.6);

  items->setPos(boundRect->boundingRect().center().x()
               - items->boundingRect().width() / 2 * 0.6
               , item->boundingRect().height() / 3.);
  if(postItem != NULL)
    {
      qreal posY = boundRect->mapFromItem(
            item
            , item->pos().x()
            , item->pos().y() + item->boundingRect().height()
            ).y() + 10;
      item = view->scene()->addLine(0.
                           , posY
                           , item->boundingRect().right()
                           , posY
                           , QPen(Qt::black, 6));
      lItems.push_back(item);
    }
  return view->scene()->createItemGroup(lItems);
}

QGraphicsItem *InteractionFluxGraphics::addModifiedUsageTableUnPersistant(const UsageTable *noema)
{
  QList<QGraphicsItem *> lItems;
  qreal nbItem = 1;
  QGraphicsItem *item = build.createExperience(
        *(view->scene())
        , noema->getInteraction()->getId());
  lItems.push_back(item);
  if(noema->getBelieve() != NULL)
    {
      QGraphicsItem *itemBelieve = build.createExperience
          (*(view->scene())
           , noema->getBelieve()->getInteraction()->getId());
      itemBelieve->setPos(item->pos().x()
                          , item->pos().y()
                          + item->boundingRect().height() + 50);
      lItems.push_back(itemBelieve);
      nbItem ++;
    }
  QGraphicsItem *items = view->scene()->createItemGroup(lItems);
  items->setScale(0.6);

  QGraphicsRectItem *box = view->scene()->addRect(0.
                                                  , 0.
                                                  , 100.
                                                  , items->boundingRect()
                                                  .height() * 0.6 + 40
                                                  , QPen(Qt::black, 6));

  items->setPos(box->boundingRect().center().x()
               - items->boundingRect().width() / 2 * 0.6
               , 20.);
  lItems.push_back(box);
  if(nbItem == 2)
    {
      QGraphicsLineItem *line = view->scene()->addLine(0.
                                                       ,0.
                                                       , 100.
                                                       , 0.
                                                       , QPen(Qt::black, 6));
      line->setPos(0., item->pos().y() + item->boundingRect().height() - 5);
      lItems.push_back(line);
    }
  return view->scene()->createItemGroup(lItems);
}

void InteractionFluxGraphics::addNumberOfLine()
{
  numberLineList.clear();

  QGraphicsRectItem *item = view->scene()->addRect(0, 0, 100, 100,
                                          QPen(Qt::black, 6));
  QGraphicsTextItem *text = new QGraphicsTextItem("1", item);
  text->setFont(font);
  text->setPos(item->boundingRect().center().x()
               - text->boundingRect().center().x()
               , item->boundingRect().center().y()
               - text->boundingRect().center().y());
  item->setPos(70, 148);
  numberLineList.push_back(item);
  lineValence = view->scene()->addLine(0, 0, 0, 0, QPen(Qt::black, 6));
  lineValence->setPos(item->pos().x() + item->boundingRect().width() + 5
                      , item->pos().y() + 50);
  QGraphicsRectItem *nextItem = view->scene()->addRect(0, 0, 100, 100,
                       QPen(Qt::black, 6)
                                              , QBrush(Qt::white));
  text = new QGraphicsTextItem("2", nextItem);
  text->setFont(font);
  text->setPos(nextItem->boundingRect().center().x()
               - text->boundingRect().center().x()
               , nextItem->boundingRect().center().y()
               - text->boundingRect().center().y());
  nextItem->setPos(0
                   , item->pos().y()
                   + item->boundingRect().height()
                   + spaceBetweenNumber);
  item = nextItem;
  numberLineList.push_back(item);

  nextItem = view->scene()->addRect(0, 0, 100, 100,
                       QPen(Qt::black, 6),
                           QBrush(Qt::white));
  nextItem->setPos(70
                   , item->pos().y() + 25);
  text = new QGraphicsTextItem("3", nextItem);
  text->setFont(font);
  text->setPos(nextItem->boundingRect().center().x()
               - text->boundingRect().center().x()
               , nextItem->boundingRect().center().y()
               - text->boundingRect().center().y());
  item = nextItem;
  numberLineList.push_back(item);

  nextItem = view->scene()->addRect(0, 0, 100, 100,
                       QPen(Qt::black, 6));
  nextItem->setPos(70
                   , item->pos().y()
                   + item->boundingRect().height()
                   + spaceBetweenNumber);
  text = new QGraphicsTextItem("4", nextItem);
  text->setFont(font);
  text->setPos(nextItem->boundingRect().center().x()
               - text->boundingRect().center().x()
               , nextItem->boundingRect().center().y()
               - text->boundingRect().center().y());
  item = nextItem;
  numberLineList.push_back(item);

  nextItem = view->scene()->addRect(0, 0, 100, 100,
                       QPen(Qt::black, 6));
  nextItem->setPos(70
                   , item->pos().y()
                   + item->boundingRect().height()
                   + spaceBetweenNumber);
  text = new QGraphicsTextItem("5", nextItem);
  text->setFont(font);
  text->setPos(nextItem->boundingRect().center().x()
               - text->boundingRect().center().x()
               , nextItem->boundingRect().center().y()
               - text->boundingRect().center().y());
  item = nextItem;
  numberLineList.push_back(item);

  nextItem = view->scene()->addRect(0, 0, 100, 100,
                       QPen(Qt::black, 6));
  nextItem->setPos(70
                   , item->pos().y()
                   + item->boundingRect().height()
                   + spaceBetweenNumber);
  text = new QGraphicsTextItem("6", nextItem);
  text->setFont(font);
  text->setPos(nextItem->boundingRect().center().x()
               - text->boundingRect().center().x()
               , nextItem->boundingRect().center().y()
               - text->boundingRect().center().y());
  numberLineList.push_back(nextItem);
  view->centerOn(numberLineList.at(0));
}

