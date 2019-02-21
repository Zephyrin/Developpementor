#ifndef INTERACTIONGRAPHICS_HPP
#define INTERACTIONGRAPHICS_HPP

#include "dockwidgetgraphicsview.hpp"
#include <interaction.hpp>
class Simulation;

class InteractionGraphics : public DockWidgetGraphicsView
{
  Q_OBJECT
public:
  explicit InteractionGraphics(const BuildGraphics &build
                               , const QString &title
                               , const QString &configName
                               , const Simulation *simulation
                               , const char *signalSimulation
                               , QAction *menuAction
                               , QWidget *parent = NULL);
  ~InteractionGraphics();

public slots:
  void addInteractions(const std::vector<const Interaction *> &interactions);
  void showInteraction(const Interaction *interaction);
  void manageVisibilitySignal(bool visible);
protected:
  QGraphicsItem *addInteractionToTree(const Interaction *interaction);

private:
  QGraphicsItem *gInteraction;
  const Simulation *simulation;
  const char *signalSimulation;
};

#endif // INTERACTIONGRAPHICS_HPP
