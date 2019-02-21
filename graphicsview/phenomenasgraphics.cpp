#include "phenomenasgraphics.hpp"
#include <phenomenas.hpp>
#include <simulation.h>

PhenomenasGraphics::PhenomenasGraphics(const BuildGraphics &build
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
                           , true
                           , true
                           , parent)
  , unknownNode(NULL)
  , phenomenas(NULL)
{
  connect(simulation, SIGNAL(simulationDone()),
          this, SLOT(simulationStop()));
  connect(simulation, SIGNAL(initPhenomenas(const Phenomenas*))
          , this, SLOT(showPhenomenas(const Phenomenas*)));
}

PhenomenasGraphics::~PhenomenasGraphics()
{
  if(unknownNode)
    {
      view->scene()->removeItem(unknownNode);
      delete unknownNode;
    }
}

void PhenomenasGraphics::showPhenomenas(const Phenomenas *phenomenas)
{
  if(this->phenomenas != NULL && phenomenas != this->phenomenas)
    simulationStop();
  this->phenomenas = phenomenas;
  if(phenomenas == NULL)
    return;
  unknownNode = new UnknownPhenomenaItem(
        build
        , view
        , phenomenas->toListOfNoema());
  view->scene()->addItem(unknownNode);
  std::size_t nbElements = phenomenas->toListOfNoema().size();
  qreal elementSize = 110.;
  view->scene()->setSceneRect(- (nbElements * elementSize)
                      , - (nbElements * elementSize)
                      , nbElements * nbElements * elementSize
                      , nbElements * nbElements * elementSize);
  view->fitInView(view->scene()->sceneRect(), Qt::KeepAspectRatio);
}

void PhenomenasGraphics::simulationStop()
{
  phenomenas = NULL;
  if(unknownNode)
    {
      view->scene()->removeItem(unknownNode);
      delete unknownNode;
      unknownNode = NULL;
      view->scene()->clear();
    }
}

void PhenomenasGraphics::timerEvent(QTimerEvent *event)
{
  Q_UNUSED(event);
  if(phenomenas == NULL)
    return;
  QList<UnknownPhenomenaItem *> unknownItems;
  foreach(QGraphicsItem *item, view->scene()->items()) {
      if(UnknownPhenomenaItem *unknownNode
         = qgraphicsitem_cast<UnknownPhenomenaItem *>(item))
        unknownItems << unknownNode;
    }
  if(unknownItems.size() > 1)
    {
      qDebug() << "Two much unknown items in view... quit.";
      exit(2);
    }
  for(UnknownPhenomenaItem *unknownNode : unknownItems)
    {
      unknownNode->updateNode();
      unknownNode->calculateForces();
      unknownNode->advance();
      unknownNode->update();
    }
  QList<PhenomenaItem *> nodes;
  foreach (QGraphicsItem *item, view->scene()->items())
    {
      if (PhenomenaItem *node = qgraphicsitem_cast<PhenomenaItem *>(item))
          nodes << node;
    }
  foreach (PhenomenaItem *node, nodes)
    {
      node->calculateForces();

      node->advance();
      node->update();
    }
}

void PhenomenasGraphics::userClear()
{
  if(unknownNode)
    {
      view->scene()->removeItem(unknownNode);
      delete unknownNode;
      unknownNode = NULL;
    }
  showPhenomenas(phenomenas);
}

void PhenomenasGraphics::userUpdate()
{
  if(phenomenas != NULL)
    {
      QList<UnknownPhenomenaItem *> unknownItems;
      foreach(QGraphicsItem *item, view->scene()->items()) {
          if(UnknownPhenomenaItem *unknownNode
             = qgraphicsitem_cast<UnknownPhenomenaItem *>(item))
            unknownItems << unknownNode;
        }
      for(UnknownPhenomenaItem *unknownNode : unknownItems)
        {
          unknownNode->updateNode();
        }
    }
}

