#include "agent.hpp"

using namespace RadicalInteractionism;

Agent::Agent()
  :
    intendedInteractionGlobal(NULL),
    enactedInteractionGlobal(NULL)
{
  currentExperience = intendedExperiences.end();
}

Agent::~Agent()
{

}

const Interaction *Agent::primitiveAction()
{
  return *currentExperience;
}

void Agent::createExperiencesSequence()
{
  enactedInteractions.clear();
  intendedExperiences.clear();
  addExperience(intendedInteractionGlobal);
  currentExperience = intendedExperiences.begin();
}

void Agent::addExperience(Interaction *interaction)
{
  if(interaction->isPrimitive())
    {
        intendedExperiences.push_back(interaction);
    }
  else
    {
      addExperience(interaction->getPreInteraction());
      addExperience(interaction->getPostInteraction());
      intendedExperiences.push_back(interaction);
    }
}

const Interaction *Agent::getIntendedInteraction() const
{
  return intendedInteractionGlobal;
}

const Interaction *Agent::getEnactedInteraction() const
{
  return enactedInteractionGlobal;
}

const std::vector<const UsageTable *> *Agent::getModifiedNoema() const
{
  return NULL;
}

