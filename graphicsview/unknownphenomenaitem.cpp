#include "unknownphenomenaitem.hpp"
#include "phenomenaedge.hpp"
#include <interaction.hpp>
#include <QDebug>

UnknownPhenomenaItem::UnknownPhenomenaItem(const BuildGraphics &build
    , GraphicsView *graphWidget
    , const std::vector<const UsageTable *> &lNoema)
  :
    PhenomenaItem(build
                  , graphWidget)
  , noemaUnknown()
{

  for(const UsageTable *noema : lNoema)
    {
      if(noema->getStatus() == UsageTable::STATUS::UNKNOWN)
        noemaUnknown.push_back(noema);
      else
        {
          PhenomenaItem *item = new PhenomenaItem(build
                                                  , noema
                                                  , graph
                                                  , this
                                                  , noemaPhenomena);
          noemaPhenomena.push_back(item);
          graph->scene()->addItem(item);
        }
    }
}

UnknownPhenomenaItem::~UnknownPhenomenaItem()
{
  for(PhenomenaItem *item : noemaPhenomena)
    {
      graph->scene()->removeItem(item);
      delete item;
    }
}

void UnknownPhenomenaItem::updateNode()
{
  QList<const UsageTable *>::iterator it = noemaUnknown.begin();
  while(it != noemaUnknown.end())
    {
      if((*it)->getStatus() == UsageTable::STATUS::PERSISTENT)
        {
          prepareGeometryChange();
          PhenomenaItem *node = new PhenomenaItem(build
                                                  , (*it)
                                                  , graph
                                                  , this
                                                  , noemaPhenomena);
          noemaPhenomena.push_back(node);
          it = noemaUnknown.erase(it);
          node->setPos(pos().x() + 50
                       , pos().y() - 50 * noemaPhenomena.size());
          graph->scene()->addItem(node);
        }
      else
        it ++;
    }
  for(const UsageTable *noema : noemaUnknown)
    {
      if(noema->getBelieve() != NULL)
        {
          interactionHasBelieve(noema);
        }
      else
        {
          interactionHasNoBelieve(noema);
        }
    }
  for(PhenomenaItem *node : noemaPhenomena)
    {
      if(node->updateNode())
        {
          prepareGeometryChange();
          noemaUnknown.push_back(node->getNoema());
          graph->scene()->removeItem(node);
          noemaPhenomena.removeOne(node);
          auto itMap = believeList.begin();
          while(itMap != believeList.end())
            {
              if(itMap.value() == node)
                itMap = believeList.erase(itMap);
              else
                itMap ++;
            }
          delete node;
        }
    }
  QPainterPath path;
  qreal nbElements = noemaUnknown.size();
  qreal elementSize = 100.;
  qreal space = 20.;
  qreal penSize = 10.;
  if(nbElements == 0)
    nbElements ++;
  qreal width = nbElements * elementSize
      + nbElements * 2 * penSize
      + (nbElements + 1) * space
      + 8 * space;
  qreal height = elementSize + 5 * space;
  path.addEllipse(- width / 2
                  , - height / 2
                  , width
                  , height);
  boundingRect_ = path.boundingRect();
  path_ = path;
  polygon = path.toFillPolygon();
  QString toolTipString;
  for(const UsageTable *noema : noemaUnknown)
    {
      toolTipString += QObject::tr("%1 valence %2\n")
          .arg(noema->getInteraction()->getId().c_str())
          .arg(noema->getReward());
    }
  if(noemaUnknown.empty())
    {
      toolTipString += QObject::tr("No more unknown stuff...");
    }
  setToolTip(toolTipString);
}

void UnknownPhenomenaItem::interactionHasBelieve(const UsageTable *noema)
{
  PhenomenaItem *nodeInBelieve = NULL;
  if(believeList.contains(noema))
    {
      if(believeList.value(noema)->getId()
         == noema->getBelieve()->getInteraction()->getId())
        return;
      else
        {
          // Need to remove the old believe
          believeList.value(noema)->removeBelieve(noema);
          believeList.remove(noema);
        }
    }
  for(PhenomenaItem *node : noemaPhenomena)
    {
      if(node->getNoema() == noema->getBelieve())
        {
          nodeInBelieve = node;
          believeList.insert(noema, nodeInBelieve);
          nodeInBelieve->addBelieve(noema);
        }
    }
//  if(nodeInBelieve && nodeInBelieve != this)
//    {
//      for(PhenomenaItem *node : noemaPhenomena)
//        {
//          for(auto const &pair
//              : node->getNoema()->getPostInteractions())
//            {
//              const Signature *signature = pair.second;
//              if(pair.first != node->getNoema()
//                 ->getInteraction()->getId()
//                 && signature->isConfirm())
//                {
//                  prepareGeometryChange();
//                  PhenomenaEdge *edge = node->addInteractionToEdge(
//                        noema->getInteraction()->getId()
//                        , nodeInBelieve);
//                  edge->setUnknownInBelieve(this);
//                  if(believeListDeprecated.find(noema)
//  == believeListDeprecated.end())
//                    {
//                      believeListDeprecated.insert(noema
//                                         , QList<PhenomenaEdge *>());
//                    }
//                  auto mIt = believeListDeprecated.find(noema);
//                  if(mIt.value().indexOf(edge) == - 1)
//                    mIt.value().push_back(edge);
//                }
//            }
//        }
//    }
}

void UnknownPhenomenaItem::interactionHasNoBelieve(const UsageTable *noema)
{
//  if(believeListDeprecated.find(noema) != believeListDeprecated.end())
//    {
//      auto mIterator = believeListDeprecated.find(noema);
//      for(PhenomenaEdge *edge : mIterator.value())
//        {
//          prepareGeometryChange();
//          edge->removeInteractionId(
//                noema->getInteraction()->getId());
//        }
//      believeListDeprecated.remove(noema);
//    }
  if(believeList.contains(noema))
    {
      believeList.value(noema)->removeBelieve(noema);
      believeList.remove(noema);
    }
  for(auto const & pair : noema->getPostInteractions())
    {
      const Signature *signature = pair.second;
      if(signature->isConfirm()
         && pair.first != noema->getInteraction()->getId())
        {
          for(PhenomenaItem *node : noemaPhenomena)
            {
              if(node->getNoema()
                 == signature->getInteraction()->getUsageTable())
                {
                  createOrUpdateEdge(this, pair.first, node);
                  break;
                }
            }
        }
      else if(pair.first != noema->getInteraction()->getId())
        {
          for(PhenomenaItem *node : noemaPhenomena)
            {
              if(node->getNoema()
                 == signature->getInteraction()->getUsageTable())
                {
                  removeInteractionEdge(this, pair.first
                                        , node);
                }
              for(PhenomenaItem *nodeSource : noemaPhenomena)
                {
                  if(nodeSource != node)
                    removeInteractionEdge(
                          nodeSource
                          , noema->getInteraction()->getId()
                          , node);
                }
            }
        }
    }
}

const PhenomenaItem *UnknownPhenomenaItem::getBelieve() const
{
  return NULL;
}

bool UnknownPhenomenaItem::hasNoema(const std::string &id) const
{
  for(const UsageTable *noema : noemaUnknown)
    if(noema->getInteraction()->getId() == id)
      return true;
  return false;
}

QRectF UnknownPhenomenaItem::boundingRect() const
{
  return boundingRect_;
}

QPainterPath UnknownPhenomenaItem::shape() const
{
  return path_;
}

void UnknownPhenomenaItem::paint(QPainter *painter
                        , const QStyleOptionGraphicsItem *option
                        , QWidget *)
{
  Q_UNUSED(option);
  build.paintUnknownPhenomena(painter, noemaUnknown);

}

