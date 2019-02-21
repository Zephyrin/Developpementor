#include "interactiongraphics.hpp"
#include <simulation.h>

InteractionGraphics::InteractionGraphics(const BuildGraphics &build
                                         , const QString &title
                                         , const QString &configName
                                         , const Simulation *simulation
                                         , const char *signalSimulation
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
  , gInteraction(NULL)
  , simulation(simulation)
  , signalSimulation(signalSimulation)

{
  connect(this, SIGNAL(visibilityChanged(bool))
          , this, SLOT(manageVisibilitySignal(bool)));
}

InteractionGraphics::~InteractionGraphics()
{

}

void InteractionGraphics::showInteraction(
    const Interaction *interaction)
{
  if(gInteraction)
    {
      view->scene()->removeItem(gInteraction);
      delete gInteraction;
    }
  gInteraction = addInteractionToTree(interaction);
}

void InteractionGraphics::manageVisibilitySignal(bool visible)
{
  if(visible)
    {
      connect(simulation, signalSimulation
              , this, SLOT(showInteraction(const Interaction*)));
    }
  else
    simulation->disconnect(this);
}

void InteractionGraphics::addInteractions(
    const std::vector<const Interaction *> &interactions)
{
  view->scene()->clear();
  for(auto it : interactions)
    {
      QRectF bound = view->scene()->itemsBoundingRect();
      QGraphicsItem * newTree = addInteractionToTree(it);
      QGraphicsLineItem *newLine
          = view->scene()->addLine(0, 0, 0, it->getHeight() * 150);
      newTree->setPos(bound.right() + 100, 0);
      view->scene()->setSceneRect(view->scene()->itemsBoundingRect());
      newLine->setPos(view->scene()->itemsBoundingRect().right() + 50, 0);
    }
  view->sceneFitInView();
}


QGraphicsItem * InteractionGraphics::addInteractionToTree(
    const Interaction *interaction)
{
  QGraphicsItem *item;
  if(interaction->isPrimitive())
    {
      item = build.createExperience(*(view->scene()), interaction->getId());
    }
  else
    {
      if(interaction->getValence() >= 0)
        item = view->scene()->addRect(0, 0, 100, 100, QPen(),
                             build.getColorOfFigures("positiveValence"));
      else
        item = view->scene()->addRect(0, 0, 100, 100, QPen(),
                             build.getColorOfFigures("negativeValence"));
    }

  item->setToolTip(tr("%1\nValence : %2").arg(interaction->getId().c_str())
                   .arg(interaction->getValence()));

  if(!interaction->isPrimitive())
    {
      QGraphicsItem *child1 = addInteractionToTree(
            interaction->getPreInteraction());


      QGraphicsItem *child2 = addInteractionToTree(
            interaction->getPostInteraction());
      child1->setPos(0, 150);
      child2->setPos(child1->boundingRect().width() + 50., 150);
      item->setPos((child1->boundingRect().width()
                    + child2->boundingRect().width()) / 2 - 50, 0);
      QList<QGraphicsItem *> list;
      list.append(item);
      list.append(child1);
      list.append(child2);
      list.append(
            view->scene()->addLine((child1->boundingRect().width()
                           + child2->boundingRect().width()) / 2,
                          100.,
                          child1->boundingRect().width() / 2.,
                          150.));

      list.append(
            view->scene()->addLine((child1->boundingRect().width()
                           + child2->boundingRect().width()) / 2,
                          100.,
                          child2->boundingRect().width() / 2.
                          + child1->boundingRect().width() + 50.,
                          150.));


      return view->scene()->createItemGroup(list);
    }
  else
    {
      return item;
    }
}
