#ifndef USAGETABLEINTERACTIONS_HPP
#define USAGETABLEINTERACTIONS_HPP

#include "usagetableitem.hpp"
#include "buildgraphics.hpp"
#include "dockwidgetgraphicsview.hpp"
#include <QList>
class Phenomenas;
class Simulation;

class UsageTableInteractionsGraphics : public DockWidgetGraphicsView
{
  Q_OBJECT
public:
  explicit UsageTableInteractionsGraphics(const BuildGraphics &build
                                          , const QString &title
                                          , const QString &configName
                                          , QAction *menuAction
                                          , Simulation *simulation
                                          , QWidget *parent = NULL);
  virtual ~UsageTableInteractionsGraphics();


public slots:
  void showPhenomenas(const Phenomenas *phenomenas);
  void simulationStop();
protected:
  void timerEvent(QTimerEvent *event);
  void userUpdate();
  void userClear();

  void addUsageTable(const UsageTable *usageTable);
protected:
  QList<UsageTableItem *> nodes;
  const Phenomenas *phenomenas;
  std::size_t nbUsageTableDisplay;
  qreal posX;
  qreal posY;
  int level;
};

#endif // USAGETABLEINTERACTIONS_HPP
