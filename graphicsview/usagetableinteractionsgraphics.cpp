#include "usagetableinteractionsgraphics.hpp"
#include <phenomenas.hpp>
#include <simulation.h>
#include <list>

UsageTableInteractionsGraphics::UsageTableInteractionsGraphics(
    const BuildGraphics &build
    , const QString &title
    , const QString &configName
    , QAction *menuAction
    , Simulation *simulation
    , QWidget *parent)
  :
    DockWidgetGraphicsView(build
                           , title
                           , configName
                           , menuAction
                           , true
                           , true
                           , parent)
  , nodes()
  , phenomenas(NULL)
  , posX(0.)
  , posY(0.)
  , level(0)
{
  connect(simulation, SIGNAL(simulationDone())
          , this, SLOT(simulationStop()));
  connect(simulation, SIGNAL(initPhenomenas(const Phenomenas*))
          , this, SLOT(showPhenomenas(const Phenomenas*)));
}

UsageTableInteractionsGraphics::~UsageTableInteractionsGraphics()
{

}

void UsageTableInteractionsGraphics::showPhenomenas(
    const Phenomenas *phenomenas)
{
  if(this->phenomenas != NULL && this->phenomenas != phenomenas)
    simulationStop();
  this->phenomenas = phenomenas;

  if(phenomenas != NULL)
    {
      posX = 0.;
      posY = 0.;
      level = 0;
      for(auto it : phenomenas->toListOfNoema())
        {
          addUsageTable(it);
        }
      nbUsageTableDisplay = phenomenas->toListOfNoema().size();
//      view->scene()->setSceneRect(view->scene()->itemsBoundingRect().x()
//                          - view->scene()->itemsBoundingRect().width() / 2.
//                          , view->scene()->itemsBoundingRect().y()
//                          - view->scene()->itemsBoundingRect().height() / 2.
//                          , view->scene()->itemsBoundingRect().width() * 2.
//                          , view->scene()->itemsBoundingRect().height() * 2.);
//      view->scene()->addRect(view->scene()->sceneRect());
//      view->centerOn(view->scene()->itemsBoundingRect().center());
    }
}

void UsageTableInteractionsGraphics::addUsageTable(const UsageTable *usageTable)
{
  UsageTableItem *item =
      new UsageTableItem(usageTable, build, view);
  view->scene()->addItem(item);
  item->setPos(posX, posY);
  if(level % 2 == 0)
    posY = item->boundingRect().height();
  else
    {
      posX += item->boundingRect().width();
      posY = 0.;
    }
  level ++;
  nodes.push_back(item);
}

void UsageTableInteractionsGraphics::simulationStop()
{
  phenomenas = NULL;
  for(UsageTableItem *item : nodes)
    {
      view->scene()->removeItem(item);
      delete item;
    }
  nodes.clear();
}

void UsageTableInteractionsGraphics::timerEvent(QTimerEvent *event)
{
  Q_UNUSED(event)
  userUpdate();
}

void UsageTableInteractionsGraphics::userUpdate()
{
  if(phenomenas != NULL)
    {
      for(UsageTableItem *item : nodes)
        item->update();
      if(nbUsageTableDisplay != phenomenas->toListOfNoema().size())
        {
          std::vector<const UsageTable *> list = phenomenas->toListOfNoema();
          for(std::size_t i = nbUsageTableDisplay; i < list.size(); ++i)
            addUsageTable(list.at(i));
          nbUsageTableDisplay = list.size();
        }
    }
}

void UsageTableInteractionsGraphics::userClear()
{
  for(UsageTableItem *item : nodes)
    {
      view->scene()->removeItem(item);
      delete item;
    }
  nodes.clear();
  showPhenomenas(phenomenas);
}

