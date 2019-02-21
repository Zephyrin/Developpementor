/*!
 *  @brief Interaction class.
 *
 * This class represent a couple of experience and result. A primitive
 * interaction is an interaction which is composed with only one experience and
 * one result. An interaction composite is an interaction with an abstract
 * experience and a pre interaction and a post interaction. It's like a reflex
 * for the agent. To see how the interaction is use in details please
 * @see Interactions, @see Existence and @see Agent. An interaction is the base
 * learning class for the agent.
 *
 *  @author    Florian Bernard
 *  @version   1.0
 *  @date      2015
 *  @copyright GNU Public License.
 */

#ifndef INTERACTION_H
#define INTERACTION_H

#include "usagetable.hpp"

#include <vector> // std::vector
#include <string>
#include <ostream>
#include <map>
/**
 * @brief The Interaction class.
 *
 * An interaction is compose of an experience, a result, a valence and a height.
 */
class Interaction
{
public:
  /**
   * @brief Interaction is the default constructor
   * @param label is the id or label.
   * @param valence is the way for the agent to known if an interaction is good
   * for him or not. By default he does not know so it's 0.
   */
  Interaction(const std::string &id,
              const int valence = 0);

  /**
   * @brief Interaction is the constructor by copy.
   * @param interaction is the interaction to copy in.
   */
  Interaction(const Interaction &interaction);
  ~Interaction();

  /**
   * @brief isPrimitive enable to know if an interaction is primitive or not.
   * i.e. if the interaction has only experience and result or if the
   * interaction is composed of interaction.
   * @return
   */
  bool isPrimitive() const;

  /**
   * @brief getValence return the interaction's valence.
   * @return return the interaction's valence.
   */
  float getValence() const;
  /**
   * @brief setValence change the interaction's valence.
   * @param valence the new valence.
   */
  void setValence(float valence);

  /**
   * @brief getPreInteraction return the preInteraction if exists or NULL.
   * @return the preInteraction if exists or NULL.
   */
  Interaction *getPreInteraction() const;

  /**
   * @brief setPreInteraction change the preInteraction.
   * @param interaction the new preInteraction.
   */
  void setPreInteraction(Interaction *interaction);

  /**
   * @brief getPostInteraction return the postInteraction if exists otherwise
   * NULL.
   * @return the postInteraction if exists or NULL.
   */
  Interaction *getPostInteraction() const;

  /**
   * @brief setPostInteraction change the postInteraction.
   * @param interaction the new postInteraction.
   */
  void setPostInteraction(Interaction *interaction);

  /**
   * @brief getWeight return the interaction's weight.
   * @return the interaction's weight.
   */
  int getWeight() const;
  /**
   * @brief incWeight increase by one the weight.
   */
  void incWeight();

  /**
   * @brief getId return the interaction's label/id.
   * @return the interaction's label/id.
   */
  const std::string &getId() const;

  /**
   * @brief getHeight return the height of the interaction. A primitive
   * interaction has an height of 1.
   * @return the height of the interaction.
   */
  std::size_t getHeight() const;

  /**
   * @brief getLastInteraction return the last postInteraction
   * @return the last interaction of the interaction
   */
  Interaction *getLastInteraction();

  /**
   * @brief addEnacted add in list of interaction that have been enacted instead
   * of this interactions
   * @param interactionEnacted
   */
  void addEnacted(Interaction *interactionEnacted);

  /**
   * @brief getEnactedInteractions return the list of all interactions that been
   * enacted instead of this interaction.
   * @return the list of enacted interactions of this interactions
   */
  std::vector<Interaction *> &getEnactedInteractions();

  /**
   * @brief getUsageTable allow to get and modify the usageTable of the
   * interaction
   * @return the usageTable of this interaction;
   */
  UsageTable *getUsageTable() const;

  /**
   * @brief getBestNextInteraction return the next best interaction to do when
   * this interaction was just enacted.
   * @return the next best interaction.
   */
  Interaction *getBestNextInteraction() const;

  /**
   * @brief setBestNextInteraction allow to change the next best interaction
   * when the decision need.
   * @param interaction the new best next interaction.
   */
  void setBestNextInteraction(Interaction *interaction);

  /**
   * @brief typeToKtbs return a string of Turtle to create the interaction's
   * on the ktbs. @see KTBS#Model. Because an interaction is construct on
   * a simulation, the id of this simulation is need.
   * @param simulationId is the id of the current simulation @see Simulation
   * @return a Turtle string to create a new interaction on KTBS.
   */
  std::string typeToKtbs(const std::string &simulationId) const;

  /**
   * @brief labelToKtbs return the label in KTBS format.
   * @param simulationId the id of the simulation.
   * @return a string without caractere that not allow on KTBS.
   */
  std::string labelToKtbs(const std::string &simulationId) const;

  /**
   * @brief toKtbs create an obsel of the interaction in office of it's use.
   * This obsels is connect to the object id create by typeToKtbs.
   * @param simulationId is the id of the simulation @see Simulation and
   * @see KTBS.
   * @param typeObsel is the type of usage of the interaction. It's can be
   * enacted or intended. More details can be found in @see Simulation,
   * @trace and @KTBS#Model.
   * @param timeStep represente the point in a step of simulation when the
   * interaction is use.
   * @return a Turtle string to create an obsel.
   */
  std::string toKtbs(const std::string &id, const std::string typeObsel,
                     const std::size_t &timeStep) const;

  /**
   * @brief toText return a string of the interaction that is human readable.
   * @return a string of the interaction to be human readable
   */
  std::string toText() const;

  /**
   * @brief operator << return a output string of the interaction readable by
   * human.
   * @param os the output stream.
   * @param interaction the interaction to print.
   * @return the output stream.
   */
  friend std::ostream & operator << (std::ostream &os,
                                     const Interaction &interaction);

  /**
   * @brief operator == allow to compare two interaction, one in pointer and the
   * other one in reference. Usefull to find an interaction in a map.
   * @param a the first interaction.
   * @param b the second one.
   * @return bool if a->id == b.id and false otherwise.
   */
  friend bool operator ==(const Interaction *a, const Interaction &b);

  friend bool operator !=(const Interaction *a, const Interaction &b);
private:
  /**
   * @brief id is generaly compose by interaction id fallow be result id.
   */
  std::string id;

  /**
   * @brief valence represente the feel of the interaction to the agent. More
   * the valence is hight more the agent feel good. If the valence is negative
   * the the agent feel bad.
   */
  float valence;
  /**
   * @brief preInteraction is the interaction to do in first to do this one.
   */
  Interaction *preInteraction;
  /**
   * @brief postInteraction is the interaction to do after the preInteraction
   * to finish this one.
   */
  Interaction *postInteraction;
  /**
   * @brief weight allow the agent to create interaction that are able to
   * please itself but that requiere many interaction with negative valence.
   * For example, the environment 4 show a complex environment that need
   * composite interaction with 3 painfull interaction to get one pleasant
   * experience.
   */
  int weight;

  /**
   * @brief height is the height of the tree of interaction. A primitive
   * interaction has a height equal to one and a interaction compose with two
   * interaction primitive has a height of two, and so on.
   */
  int height;

  /**
   * @brief enacteds is the list off all interactions that have been enacted
   * instead of this one.
   */
  std::vector<Interaction *> enacteds;

  /**
   * @brief usageTable is an information of how this interaction is use with
   * other interaction.
   */
  UsageTable *usageTable;

  /**
   * @brief bestNextInteraction representes the next interaction to do when the
   * decision has made it's choice. It's not algorithm, but more for displaying
   * and understood the next step of each interaction.
   */
  Interaction * bestNextInteraction;
};

#endif // INTERACTION_H
