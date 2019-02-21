#ifndef INTERACTIONFLUXGRAPHICS_HPP
#define INTERACTIONFLUXGRAPHICS_HPP

#include "dockwidgetgraphicsview.hpp"

class Interaction;
enum class MOOD;
class Simulation;

class InteractionFluxGraphics : public DockWidgetGraphicsView
{
  Q_OBJECT
public:
  InteractionFluxGraphics(const BuildGraphics &build
                          , const QString &title
                          , const QString &configName
                          , const Simulation *simulation
                          , QAction *menuAction
                          , QWidget *parent = NULL);
  ~InteractionFluxGraphics();

protected:
  QGraphicsItem *addInteractionsInLine(const Interaction *interaction);
  QGraphicsItem *addMood(const MOOD mood);
  QGraphicsItem *addModifiedUsageTablePersistant(const UsageTable *noema);
  QGraphicsItem *addModifiedUsageTableUnPersistant(const UsageTable *noema);

  void addNumberOfLine();

  void addValence(const int valence, const qreal posX);
  QGraphicsItem *addBoxToInteraction(const QGraphicsItem *interaction
                                     , const QBrush b = QBrush("white"));

protected:
  void userClear();

signals:
  void newItemsInView();
public slots:
  void addInteractions(const Interaction *intended
      , const Interaction *enacted
      , const std::vector<const UsageTable *> *usagesTablesModified
      , const int moodInt
      , const int step);

protected slots:
  void moveScrollBarToRight();
protected:
  QGraphicsLineItem *timeLine;
  std::size_t excitement;
  QGraphicsTextItem *timeItem;
  QFont font;
  QList<QGraphicsItem *> numberLineList;
  QGraphicsLineItem *lineValence;
  qreal spaceBetweenNumber;
};

#endif // INTERACTIONFLUXGRAPHICS_HPP
