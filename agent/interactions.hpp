
#ifndef INTERACTIONS_H
#define INTERACTIONS_H

#include "interaction.hpp"

#include <map> // std::map
#include <vector> // std::vector
#include <string> // std::string

class Phenomenas;
/**
 * @brief The Interactions class.
 * This class can be call the interaction's memory of the agent. Interactions
 * give functionnality to create primitive and composite interaction.
 *
 * @author Florian Bernard
 * @version 0.5
 * @date 2015
 * @copyright GNU Public License.
 */
class Interactions
{
public:
  /**
   * @brief Interactions the defaults constructor use for the creation of every
   * environment.
   */
  Interactions();

  /**
   * @brief Interactions the defaults constructor.
   * @param experiences the list of primitive experiences known at the beginning
   * of the simulation. For now, it's the full list of interaction and there is
   * no functionnality to add new experience.
   * @param motivations the motivation of primitive interaction with the result.
   * The motivation is set by the environment.
   * @param maxInteractionHeight the max height of the tree of interactions
   * composite.
   * @param phenomenas is the container of all UsageTable. It's need in this
   * class when it's create a new interaction composite, to allow the fact that
   * this interaction must be add in each UsageTable.
   */
  Interactions(const Interactions &interactions,
               const std::size_t &maxInteractionHeight
               , Phenomenas *phenomenas);
  ~Interactions();

  /**
   * @brief addOrGetPrimitiveInteraction add if not exists a new primitive
   * interaction and return it.
   * @param experience the experience of the new interaction.
   * @param result the result associated to the interaction.
   * @param valence the valence of the new interaction.
   * @return the interaction, created if not exists, with id experience.id
   * + result.id.
   */
  Interaction *addOrGetPrimitiveInteraction(std::string id,
                                            const int valence);

  /**
   * @brief addOrGetPrimitiveInteraction add if not exists a new primitive
   * experience. If not exists the valence is set to the value given by the
   * environment in the motivation structure. If there is no value, then 0 is
   * used.
   * @param experience the experience of the interaction
   * @param result the result associated to the interaction.
   * @return the interaction with id equal to experience.id + result.id.
   */
  Interaction *addOrGetPrimitiveInteraction(const std::string &id);

  /**
   * @brief getInteraction with its id.
   * @param id the interction's id.
   * @return the interaction with id id or NULL if not exists.
   */
  Interaction *getInteraction(const std::string &label) const;

  /**
   * @brief learnCompositeInteraction create a new interaction. The height of
   * the tree is increase here. This function create in general three
   * interactions. For example on how it's work launch the simulation and
   * activated the output of learned interaction and reinforce.
   * The valence of this new interaction is the average of the two associated
   * interactions.
   *
   * @param lastInteraction
   * @param previousInteraction
   */
  void learnCompositeInteraction(Interaction *lastInteraction,
                                 Interaction *previousInteraction);

  /**
   * @brief getActivatedInteractions return a list of interactions which have
   * enactedInteraction in preInteraction.
   * @param enactedInteraction the interaction looking for.
   * @return the list of all interactions that have enactedInteraction in their
   * preInteraction.
   */
  std::vector<Interaction *> getActivatedInteractions(
      Interaction *enactedInteraction);

  /**
   * @brief addOrGetCompositeInteraction create a new composite interaction
   * with preInteraction and postInteraction. The valence is the average of the
   * preInteraction and the postInteraction. This function also create an
   * abstract experiment.
   * @param preInteraction
   * @param postInteraction
   * @return return new interaction if not exists or the interaction with id
   * <preInteraction.id + postInteraction.id>.
   */
  Interaction *addOrGetCompositeInteraction(Interaction *preInteraction,
                                            Interaction *postInteraction);

  /**
   * @brief toVector return all interactions into a vector.
   * @return all interactions into a vector.
   */
  std::vector<const Interaction *> toVector() const;

  /**
   * @brief getOtherInteraction return a better interaction with a maximum
   *  valence.
   * @param interaction the interaction looking for better.
   * @return an annother interaction or NULL if not exists. Not suppose to
   * append.
   */
  Interaction *getOtherInteraction(Interaction *interaction);

  /**
   * @brief begin return the const first iterator.
   * @return the first const iterator of interactions.
   */
  std::map<std::string, Interaction *>::const_iterator begin() const;

  /**
   * @brief end return the last const iterator.
   * @return the last const iterator of interactions.
   */
  std::map<std::string, Interaction *>::const_iterator end() const;

  /**
   * @brief begin return the first iterator.
   * @return the first iterator of interactions.
   */
  std::map<std::string, Interaction *>::iterator begin();

  /**
   * @brief end return the last iterator.
   * @return the last iterator of interactions.
   */
  std::map<std::string, Interaction *>::iterator end();

  /**
   * @brief getPrimitiveInteractions return the list of all primitive
   * interactions create at the initialisation of the agent.
   * @return the list of primitive interactions.
   */
  std::vector<Interaction *> getPrimitiveInteractions();


  typedef std::map<std::string, Interaction *> mInteractions;
  typedef std::pair<std::string, Interaction *> pInteractions;

protected:

  /**
   * @brief addOrGetInteraction create a new interaction if not exists and
   * return the interaction associated to the id.
   * @param id the id of the interaction.
   * @return an interaction with id id.
   */
  Interaction *addOrGetInteraction(const std::string &label);

  /**
   * @brief addOrGetAndReinforceCompositeInteraction create and reinforce a
   * composite interaction by adding a weight to the interaction.
   * @param preInteraction
   * @param postInteraction
   * @return the interaction created or reinforce.
   */
  Interaction * addOrGetAndReinforceCompositeInteraction(
      Interaction *preInteraction, Interaction *postInteraction);
private:
  /**
   * @brief interactions the map of interactions.
   */
  mInteractions interactions;
  /**
   * @brief superInteraction the last super interaction created. Its usefull for
   * creating new composite interaction to get the pre interaction.
   */
  Interaction *superInteraction;

  /**
   * @brief maxInteractionHeight the maximal height of an interaction.
   */
  std::size_t maxInteractionHeight;

  /**
   * @brief listPrimitiveInteraction is the list of primitive interaction create
   * in the beggining of the agent.
   */
  std::vector<Interaction *> listPrimitiveInteraction;

  /**
    * @brief phenomenas is the container of all UsageTable.
    */
  Phenomenas *phenomenas;

};

#endif // INTERACTIONS_H
