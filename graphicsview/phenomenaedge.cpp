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

#include "phenomenaedge.hpp"
#include "phenomenaitem.hpp"
#include "unknownphenomenaitem.hpp"

#include <interaction.hpp>
#include <math.h>

#include <QPainter>
#include <QDebug>
static const double Pi = 3.14159265358979323846264338327950288419717;

PhenomenaEdge::PhenomenaEdge(PhenomenaItem *A, PhenomenaItem *B, const BuildGraphics &build)
    : arrowSize(100)
    , build(build)
    , unknownInBelieve(NULL)
{
    setAcceptedMouseButtons(0);
    A_ = A;
    B_ = B;
    A_->addEdge(this);
    B_->addEdge(this);
    adjust();
    setZValue(-1.1);
}

PhenomenaEdge::~PhenomenaEdge()
{
  A_->removeEdge(this);
  B_->removeEdge(this);
//  if(unknownInBelieve)
//    unknownInBelieve->removeBelieveEdge(this);
  A_ = NULL;
  B_ = NULL;
}

PhenomenaItem *PhenomenaEdge::nodeA() const
{
    return A_;
}

PhenomenaItem *PhenomenaEdge::nodeB() const
{
  return B_;
}

void PhenomenaEdge::addInteractionId(const std::string &id, PhenomenaItem *node)
{
  if(node == A_)
    {
      if(!lNoemaA.contains(id))
        {
          prepareGeometryChange();
          lNoemaA.append(id);
        }
    }
  else
    if(!lNoemaB.contains(id))
      {
        prepareGeometryChange();
        lNoemaB.append(id);
      }
}

void PhenomenaEdge::removeInteractionId(const std::string &id
                                        , PhenomenaItem *node)
{
  if(node == A_)
    {
      if(lNoemaA.removeOne(id))
        prepareGeometryChange();
    }
  else
    if(lNoemaB.removeOne(id))
      prepareGeometryChange();
}

void PhenomenaEdge::removeInteractionId(const std::string &id)
{
  if(A_ == NULL || B_ == NULL)
    return;
  if(lNoemaA.removeOne(id))
    prepareGeometryChange();
  if(lNoemaB.removeOne(id))
    prepareGeometryChange();
}


bool PhenomenaEdge::empty() const
{
  return lNoemaA.empty() && lNoemaB.empty();
}

bool PhenomenaEdge::hasNode(PhenomenaItem *node)
{
  return A_ == node || B_ == node;
}

void PhenomenaEdge::setUnknownInBelieve(UnknownPhenomenaItem *unknown)
{
  this->unknownInBelieve = unknown;
}

void PhenomenaEdge::adjust()
{
    if (!A_ || !B_)
        return;
    QPointF pDest = mapFromItem(B_, 0, 0);
    if(B_->getBelieve() != NULL)
      pDest = mapFromItem(B_->getBelieve(), 0, 0);
    QLineF line(mapFromItem(A_, 0, 0), pDest);
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.) * lNoemaA.size() + 1) {
        sourceA = line.p1();
        sourceB = line.p2();
        QLineF centerLine(A_->pos(), B_->pos());
        QPolygonF endPolygon = B_->getPolygon();
        if(endPolygon.empty())
          return;
        QPointF p1 = endPolygon.first() + B_->pos();
        QPointF p2;
        QPointF intersectPointDest;
        QLineF polyLine;
        for (int i = 1; i < endPolygon.count(); ++i) {
            p2 = endPolygon.at(i) + B_->pos();
            polyLine = QLineF(p1, p2);
            QLineF::IntersectType intersectType =
                polyLine.intersect(centerLine, &intersectPointDest);
            if (intersectType == QLineF::BoundedIntersection)
              break;
            p1 = p2;
          }

        QPolygonF sourcePolygon = A_->getPolygon();
        if(sourcePolygon.empty())
          return;
        p1 = sourcePolygon.first() + A_->pos();
        QPointF intersectPointSource;
        for (int i = 1; i < sourcePolygon.count(); ++i) {
            p2 = sourcePolygon.at(i) + A_->pos();
            polyLine = QLineF(p1, p2);
            QLineF::IntersectType intersectType =
                polyLine.intersect(centerLine, &intersectPointSource);
            if (intersectType == QLineF::BoundedIntersection)
              break;
            p1 = p2;
          }
        sourceA = intersectPointSource;
        sourceB = intersectPointDest;
    } else {
        sourceA = sourceB = line.p1();
    }
}

QRectF PhenomenaEdge::boundingRect() const
{
    if (!A_ || !B_)
        return QRectF();

    qreal penWidth = 5.;
    qreal extra = (penWidth + arrowSize) / 2.0;

    return QRectF(sourceA, QSizeF(sourceB.x() - sourceA.x(),
                                sourceB.y() - sourceA.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void PhenomenaEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *
                          , QWidget *)
{
    if (!A_ || !B_)
        return;

    // Draw the line itself
    painter->setPen(QPen(Qt::black, 5, Qt::SolidLine
                         , Qt::RoundCap, Qt::RoundJoin));


    QLineF line(QLineF(sourceA, sourceB));

    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
      angle = (Pi * 2) - angle;

    QPointF arrowP1 = line.p1()
        + QPointF(line.dx() / 2, line.dy() / 2)
        + QPointF(sin(angle + Pi / 3) * arrowSize,
                  cos(angle + Pi / 3) * arrowSize);
    QPointF arrowP2 = line.p1()
        + QPointF(line.dx() / 2, line.dy() / 2)
        + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                  cos(angle + Pi - Pi / 3) * arrowSize);
    QPolygonF arrowHead;
    arrowHead.clear();
    arrowHead <<  arrowP1 << arrowP2;
    painter->drawLine(line);

    qreal i = 1;
    QString toolTipString = "";
    for(const std::string &id : lNoemaB)
      {
        qreal posX = line.x1() + line.dx() / 2
            + ((line.dx() / 2) / (lNoemaB.size() + 1)) * i;
        qreal posY = line.y1() + line.dy() / 2
            + ((line.dy() / 2) / (lNoemaB.size() + 1)) * i;
        build.paintExperience(painter
                              , id
                              , posX
                              , posY);
        i ++;
        if(B_->getNoema() != NULL)
          {
            toolTipString +=
                QObject::tr("To Phenomena : %1\n")
                .arg(B_->getNoema()->getInteraction()->getId().c_str());
            for(auto const & pair : B_->getNoema()->getPostInteractions())
              {
                if(pair.first == id)
                  {
                    const Signature *signature = pair.second;
                    toolTipString +=
                        QObject::tr("%1 : enacted : %2 & intended : %3\n")
                        .arg(id.c_str())
                        .arg(signature->getNbEnacted())
                        .arg(signature->getNbIntended())
                        ;
                  }
              }
          }
      }
    i = 1;
    for(const std::string &id : lNoemaA)
      {
        qreal posX = line.x1()
            + ((line.dx() / 2) / (lNoemaA.size() + 1)) * i;
        qreal posY = line.y1()
            + ((line.dy() / 2) / (lNoemaA.size() + 1)) * i;
        build.paintExperience(painter
                              , id
                              , posX
                              , posY);
        i ++;
        if(A_->getNoema() != NULL)
          {
            toolTipString +=
                QObject::tr("From Phenomena : %1\n")
                .arg(A_->getNoema()->getInteraction()->getId().c_str());
            for(auto const & pair : A_->getNoema()->getPostInteractions())
              {
                if(pair.first == id)
                  {
                    const Signature *signature = pair.second;
                    toolTipString +=
                        QObject::tr("%1 : enacted : %2 & intended : %3\n")
                        .arg(id.c_str())
                        .arg(signature->getNbEnacted())
                        .arg(signature->getNbIntended());

                  }
              }
          }
      }
    painter->drawPolygon(arrowHead);
    setToolTip(toolTipString);
}
