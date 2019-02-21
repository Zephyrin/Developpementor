/*!
 * @brief Anticipation class
 *
 * This class convert an experience to a potentialy experience to enact. It's
 * add a new field call proclivity.
 * The proclivity is computed on the basis of the possible interactions that
 * may actually be enacted as an effect of performing this experiment, as far
 * as the system can tell from its experience.
 *
 *  @author    Florian Bernard
 *  @version   1.0
 *  @date      2015
 *  @copyright GNU Public License.
 */
#ifndef ANTICIPATION_H
#define ANTICIPATION_H

#include "interaction.hpp"
#include <vector> // std::vector
#include <ostream>

/**
 * @brief The Anticipation class to compute the basis of the possible
 * interactions that may actually be enacted.
 */
class Anticipation
{
public:
  /**
   * @brief Anticipation is the default constructor.
   * @param interaction is the interaction.
   * @param proclivity is the possible interactions that actually be enacted.
   */
  Anticipation(Interaction *interaction, float proclivity);
  /**
   * @brief Anticipation is the constructor by copy.
   * @param anticipation the anticipation to copy in.
   */
  Anticipation(const Anticipation &anticipation);

  ~Anticipation();

  /**
   * @brief getExperience return the experience.
   * @return the experience.
   */
  Interaction *getInteraction() const;

  /**
   * @brief changeInteraction change the interaction with the new one.
   * @param interaction the new interaction.
   */
  void changeInteraction(Interaction *interaction);

  /**
   * @brief getProclivity return the proclivity.
   * @return the proclivity.
   */
  float getProclivity() const;
  /**
   * @brief addProclivity add value to proclivity.
   * @param value is a number to add to proclivity.
   */
  void addProclivity(float value);

  void removeProclivity(float value);

  /**
   * @brief operator == allow to compare an anticipation with an experience.
   * @param anticipation the anticipation.
   * @param experience the experience.
   * @return true if anticipation.experience equals experience.
   */
  friend bool operator ==(const Anticipation *anticipation,
                          const Interaction &interaction);

  /**
   * @brief operator << append to an ostream the anticipation. All field are in
   * the ostream.
   * @param os the output stream.
   * @param anticipation to show.
   * @return os and the anticipation append.
   */
  friend std::ostream &operator <<(std::ostream &os,
                                   const Anticipation &anticipation);

  /**
   * @brief toKtbs return a output stream that contains the anticipation in
   * Turtle.
   * @param os the output stream
   * @param step the step in the simulation @see Simulation.
   * @param id is the id of the simulation @see Simulation
   * @return a string format in Turtle without the context @see KTBS.
   */
  std::ostream &toKtbs(std::ostream &os,
                       const std::string &step,
                       const std::string &id) const;

  /**
   * @brief toText return the anticipation in text mode.
   * @return all field of the anticipation in text.
   */
  std::string toText() const;


private:
  /**
   * @brief experience
   */
  Interaction *interaction;

  /**
   * @brief oldInteractions representes all interaction that have been choosen
   * before the interaction.
   */
  std::vector<const Interaction *> oldInteractions;

  /**
   * @brief proclivity is the change to enact this experience on the next step
   * of the simulation.
   */
  float proclivity;
};

#endif // ANTICIPATION_H
