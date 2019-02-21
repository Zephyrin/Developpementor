#ifndef PHENOMENASGRAPHICS_HPP
#define PHENOMENASGRAPHICS_HPP

#include "dockwidgetgraphicsview.hpp"
#include "unknownphenomenaitem.hpp"

class Phenomenas;
class Simulation;

#include <QMutex>

class PhenomenasGraphics : public DockWidgetGraphicsView
{
  Q_OBJECT

public:
  PhenomenasGraphics(const BuildGraphics &build
                     , const QString &title
                     , const QString &configName
                     , const Simulation *simulation
                     , QAction *action, QWidget *parent = NULL);
  virtual ~PhenomenasGraphics();

public slots:
  void showPhenomenas(const Phenomenas *phenomenas);
  void simulationStop();
protected:
  void timerEvent(QTimerEvent *event);
  void userUpdate();
  void userClear();
protected:
  UnknownPhenomenaItem *unknownNode;
  const Phenomenas *phenomenas;
};

#endif // PHENOMENASGRAPHICS_HPP
