#ifndef AGENT_HPP
#define AGENT_HPP

#include "interaction.hpp"
#include "phenomenas.hpp"
#include "mood.hpp"
namespace RadicalInteractionism
{
  class Agent
  {
  public:
    Agent();
    virtual ~Agent();

    virtual const Interaction *getExperience() = 0;

    virtual void resultExperience(const Interaction *enacted) = 0;

    virtual std::vector<const Interaction *> getInteractions() const = 0;

    virtual const Phenomenas *getPhenomenas() const = 0;

    const Interaction *getIntendedInteraction() const;

    const Interaction *getEnactedInteraction() const;

    virtual MOOD getMood() const = 0;

    virtual const std::vector<const UsageTable *> *getModifiedNoema() const;

  protected:

    enum ABRETYPE {
      PRE_INTERACTION,
      POST_INTERACTION,
      PRIMITIVE,
      INTERACTION
    };

    const Interaction *primitiveAction();

    void createExperiencesSequence();
    void addExperience(Interaction *interaction);

    Interaction *intendedInteractionGlobal;
    Interaction *enactedInteractionGlobal;
    std::vector<Interaction *> intendedExperiences;
    std::vector<Interaction *>::iterator currentExperience;
    std::vector<Interaction *> enactedInteractions;
  };
}
#endif // AGENT_HPP
