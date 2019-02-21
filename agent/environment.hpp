/*!
 *  @brief Environment @a interface
 *
 * This abstract class show the minimum functionnality of an environment to
 * be used with a the simulation.
 *
 *  @author    Florian Bernard
 *  @version   1.0
 *  @date      2015
 *  @copyright GNU Public License.
 */
#ifndef ENVIRONNEMENT_H
#define ENVIRONNEMENT_H

#include <interactions.hpp>

/**
 * @brief The Environment abstract class to implement to create a new
 * environment.
 *
 * To know how to create a new environment you can @see Environment1,
 * @see Environment4, @see EnvironmentString and @see EnvironmentSmallLoop.
 */
class Environment
{
public:
  /**
   * @brief Environment the default constructor
   */
  Environment();
  virtual ~Environment();

  /**
   * @brief init is call during the simulation at the beggining.
   * This function initialise experiments and results and motivation of the
   * agent. It's also there where the environment can load a file and tell if
   * the environment is correctly instantiated or not.
   */
  virtual void init() = 0;

  /**
   * @brief getResult return the result that the environment associated to the
   * experience.
   * @param experience the experience enact by the agent.
   * @return the result associated to the experience.
   */
  virtual const Interaction * getResult(const Interaction *interaction) = 0;

  /**
   * @brief getExperiences return the list of primitive experiences known by the
   * environment.
   * @return the list of experiences known by the environment.
   */
  const Interactions &getInteractions() const;

  /**
   * @brief toKtbs return a Turtle string that contains all the experience and
   * results to save it on KTBS. @see KTBS to known the model.
   * @param simulationId is the current simulation.
   * @return a Turtle string of the environment.
   */
  std::string toKtbs(const std::string &simulationId) const;

  /**
   * @brief getWellForm is use by the simulation to known if the environment is
   * correctly initiated or not. Usefull to read a user configuration file.
   * @return true if the environment is correctly instantiate and false
   * otherwise.
   */
  bool getWellForm() const;

protected:
  /**
   * @brief interactions is the primitive interactions known by the environment.
   */
  Interactions interactions;

  /**
   * @brief wellForm is usefull for the simulation to know if it can launch the
   * simulation or not.
   */
  bool wellForm;
};

#endif // ENVIRONNEMENT_H
