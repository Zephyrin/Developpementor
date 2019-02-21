/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "phenomenaitem.hpp"
#include "phenomenaedge.hpp"
#include "unknownphenomenaitem.hpp"
#include <interaction.hpp>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
PhenomenaItem::PhenomenaItem(const BuildGraphics &build
                             , GraphicsView *graphWidget)
  :
    edgeList(QList<PhenomenaEdge *>())
  , graph(graphWidget)
  , build(build)
  , noema(NULL)
  , noemas(noemaPhenomena)
{
  setFlag(ItemIsMovable);
  setFlag(ItemSendsGeometryChanges);
  setCacheMode(DeviceCoordinateCache);
  setZValue(-1);
}

PhenomenaItem::PhenomenaItem(const BuildGraphics &build
                             , const UsageTable *noema
                             , GraphicsView *graphWidget
                             , UnknownPhenomenaItem *unknown
                             , QList<PhenomenaItem *> &noemas)
  :
    graph(graphWidget)
  , build(build)
  , noema(noema)
  , unknown(unknown)
  , noemas(noemas)
{
  setFlag(ItemIsMovable);
  setFlag(ItemSendsGeometryChanges);
  setCacheMode(DeviceCoordinateCache);
  setZValue(-1);
}

PhenomenaItem::~PhenomenaItem()
{
  for(PhenomenaEdge *edge : edgeList)
    {
      graph->scene()->removeItem(edge);
      delete edge;
    }
}

void PhenomenaItem::addEdge(PhenomenaEdge *edge)
{
  edgeList << edge;
  edge->adjust();
}

QList<PhenomenaEdge *> PhenomenaItem::edges() const
{
  return edgeList;
}

bool PhenomenaItem::updateNode()
{
  if(noema->getStatus() != UsageTable::STATUS::PERSISTENT)
    {
      return true;
    }
  else
    {
      for(auto const & pair : noema->getPostInteractions())
        {
          const Signature *signature = pair.second;
          if(signature->isConfirm()
             && pair.first != noema->getInteraction()->getId())
            {
              bool found = false;
              std::string id = pair.first;
              if(signature->getInteraction()->getUsageTable()->getBelieve())
                id = signature->getInteraction()->getUsageTable()->getBelieve()
                    ->getInteraction()->getId();
              for(PhenomenaItem *destNode : noemas)
                {

                  if(destNode->hasNoema(id) )
                    {
                      createOrUpdateEdge(this, pair.first, destNode);
                      removeInteractionEdge(this, pair.first, unknown);
                      found = true;
                      break;
                    }
                }
              if(!found)
                {
                  createOrUpdateEdge(this, pair.first, unknown);
                }
            }
          else
            {
              bool found = false;
              for(PhenomenaItem *destNode : noemas)
                {
                  if(destNode->hasNoema(pair.first))
                    {
                      removeInteractionEdge(this, pair.first
                                            , destNode);
                      found = true;
                      break;
                    }
                }
              if(!found)
                removeInteractionEdge(this, pair.first, unknown);
            }
        }
//      if(!interactionWithThisBelieve.isEmpty())
//        {
//          for(const UsageTable *usageTable : interactionWithThisBelieve)
//            {
//              Interaction *believe = usageTable->getInteraction();
//              PhenomenaItem *node = NULL;
//              for(PhenomenaItem *item : noemas)
//                if(item->getNoema() == believe->getUsageTable())
//                  {
//                    node = item;
//                    break;
//                  }
//              if(node)
//                createOrUpdateEdge(this
//                                   , usageTable->getInteraction()->getId()
//                                   , node);
//            }
////          Interaction *best = noema->getInteraction()->getBestNextInteraction();

////          PhenomenaItem *bestNode = NULL;
////          if(best)
////            {
////              for(PhenomenaItem *node : noemas)
////                {
////                  if(node->hasNoema(best->getId()))
////                    {
////                      bestNode = node;
////                      break;
////                    }
////                }
////            }
////          if(bestNode)
////            {
////              for(const UsageTable *usage : interactionWithThisBelieve)
////                {
////                  createOrUpdateEdge(this, usage->getInteraction()->getId()
////                                     , bestNode);
////                }
////            }
//        }
    }
  QPainterPath path;
  qreal nbElements = 0;
  qreal elementSize = 100.;
  qreal space = 20.;
  qreal spaceTop = 90.;
  qreal penSize = 5.;
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
  qreal height =
      spaceTop
      + elementSize
      + 2 * penSize
      + space
      + (nbElements * elementSize)
      + (nbElements * (2 * penSize))
      + (nbElements * space)
      + (nbElements == 0 ? 0 : space) ;
  QPolygonF p;
  qreal width = 2 * space
      + elementSize
      + 2 * penSize;
  p.append(QPointF(-width / 2., - height / 2. + 10.));
  p.append(QPointF(-width / 2., height / 2.));
  p.append(QPointF(width / 2., height / 2.));
  p.append(QPointF(width / 2. , - height / 2. + 10.));
  p.append(QPointF(0, -height / 2.));
  p.append(QPointF(-width / 2., - height / 2. + 10.));
  path.addPolygon(p);
  boundingRect_ = path.boundingRect();
  path_ = path;
  polygon = path.toFillPolygon();

  setToolTip(noema->toTextTip().c_str());
  return false;
}

PhenomenaEdge *PhenomenaItem::addInteractionToEdge(const std::string &id
                                         , PhenomenaItem *nodeDest)
{
  return createOrUpdateEdge(this, id, nodeDest);
}

void PhenomenaItem::addBelieve(const UsageTable *usageTable)
{
  if(!interactionWithThisBelieve.contains(usageTable))
    {
      interactionWithThisBelieve.push_back(usageTable);
    }
}

void PhenomenaItem::removeBelieve(const UsageTable *usageTable)
{
  interactionWithThisBelieve.removeOne(usageTable);
  for(PhenomenaEdge * edge : edgeList)
    {
      edge->removeInteractionId(usageTable->getInteraction()->getId());
      if(edge->empty())
        {
          graph->scene()->removeItem(edge);
          delete edge;
        }
    }
}

PhenomenaEdge *PhenomenaItem::createOrUpdateEdge(PhenomenaItem *source
                                     , const std::string &id
                                     , PhenomenaItem *dest)
{
//  QList<PhenomenaEdge *> edgeList;
//  foreach (QGraphicsItem *item, graph->scene()->items()) {
//      if(PhenomenaEdge *edge = qgraphicsitem_cast<PhenomenaEdge *>(item))
//        edgeList << edge;
//    }
  for(PhenomenaEdge *edge : edgeList)
    {
      if(edge->hasNode(source) && edge->hasNode(dest))
        {
          edge->addInteractionId(id, source);
          return edge;
        }
    }
  PhenomenaEdge *edge = new PhenomenaEdge(source, dest, build);
  edge->addInteractionId(id, source);
  graph->scene()->addItem(edge);
  return edge;
}

void PhenomenaItem::removeInteractionEdge(PhenomenaItem *source
                                        , const std::string &id
                                        , PhenomenaItem *dest)
{
//  QList<PhenomenaEdge *> edgeList;
//  foreach (QGraphicsItem *item, graph->scene()->items()) {
//      if(PhenomenaEdge *edge = qgraphicsitem_cast<PhenomenaEdge *>(item))
//        edgeList << edge;
//    }
  for(PhenomenaEdge *edge : edgeList)
    {
      if(edge->hasNode(source) && edge->hasNode(dest))
        {
          edge->removeInteractionId(id, source);
          if(edge->empty())
            {
              graph->scene()->removeItem(edge);
              delete edge;
              return;
            }
        }
    }
}

void PhenomenaItem::calculateForces()
{
  if (!scene() || scene()->mouseGrabberItem() == this) {
      newPos = pos();
      return;
    }

  // Sum up all forces pushing this item away
  qreal xvel = 0;
  qreal yvel = 0;
  foreach (QGraphicsItem *item, graph->scene()->items()) {
      PhenomenaItem *node = qgraphicsitem_cast<PhenomenaItem *>(item);
      if (!node)
          {
                  continue;
          }
      if(node == this)
        continue;
      QPointF vec = mapToItem(node, 0, 0);
      qreal dx = vec.x();
      qreal dy = vec.y();
      double l = 2.0 * (dx * dx + dy * dy);
      if (l > 0 && l < 10000000. - 1000000.) {
          xvel += (dx * 15000.0) / l;
          yvel += (dy * 15000.0) / l;
        }
      else if(node != this && l == 0)
        {
          xvel += (10. * 10.0);
          yvel += (10. * 10.0);
        }
    }

  // Now subtract all forces pulling items together
  double weight = (edgeList.size() + 1) * 10;
  foreach (PhenomenaEdge *edge, edgeList) {
      QPointF vec;
      if (edge->nodeA() == this)
        vec = mapToItem(edge->nodeB(), 0, 0);
      else
        vec = mapToItem(edge->nodeA(), 0, 0);
      qreal dx = vec.x();
      qreal dy = vec.y();
      double l = 2.0 * (dx * dx + dy * dy);
      if(l > 10000000.)
        {
          xvel -= vec.x() / weight;
          yvel -= vec.y() / weight;
        }
    }

  if (qAbs(xvel) < 0.1 && qAbs(yvel) < .1)
    xvel = yvel = 0;

  QRectF sceneRect = scene()->sceneRect();
  newPos = pos() + QPointF(xvel, yvel);
  newPos.setX(qMin(qMax(newPos.x(), sceneRect.left()
                        + boundingRect().width()
                        + 10),
                   sceneRect.right() - (boundingRect().width() + 10)));
  newPos.setY(qMin(qMax(newPos.y()
                        , sceneRect.top()
                        + boundingRect().height()
                        +10)
                   , sceneRect.bottom() - (boundingRect().height() + 10)));
}

bool PhenomenaItem::advance()
{
  if (newPos == pos())
    return false;

  setPos(newPos);
  return true;
}

void PhenomenaItem::removeEdge(PhenomenaEdge *edge)
{
  edgeList.removeOne(edge);
}

const PhenomenaItem* PhenomenaItem::getBelieve() const
{
  if(noema->getBelieve() == NULL && noema->getBelieve() == noema)
    return NULL;
  for(PhenomenaItem *node : noemaPhenomena)
    if(node->noema == noema->getBelieve())
      return node;
  return NULL;
}

const std::string &PhenomenaItem::getId() const
{
  return noema->getInteraction()->getId();
}

UsageTable::STATUS PhenomenaItem::getStatus() const
{
  return noema->getStatus();
}

const UsageTable *PhenomenaItem::getNoema() const
{
  return noema;
}

bool PhenomenaItem::hasNoema(const std::string &id) const
{
  if(noema && noema->getInteraction()->getId() == id)
    return true;
  return false;
}

QPolygonF PhenomenaItem::getPolygon() const
{
  return polygon;
}

QRectF PhenomenaItem::boundingRect() const
{
  return boundingRect_;
}

QPainterPath PhenomenaItem::shape() const
{
  return path_;
}

void PhenomenaItem::paint(QPainter *painter
                          , const QStyleOptionGraphicsItem *option
                 , QWidget *)
{
  if(!noema)
    return;
  Q_UNUSED(option);
  build.paintPhenomena(painter, noema);
}

QVariant PhenomenaItem::itemChange(GraphicsItemChange change
                                   , const QVariant &value)
{
  switch (change) {
    case ItemPositionHasChanged:
      foreach (PhenomenaEdge *edge, edgeList)
        edge->adjust();
      //        graph->itemMoved();
      break;
    default:
      break;
    };

  return QGraphicsItem::itemChange(change, value);
}

void PhenomenaItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  update();
  QGraphicsItem::mousePressEvent(event);
}

void PhenomenaItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  update();
  QGraphicsItem::mouseReleaseEvent(event);
}
