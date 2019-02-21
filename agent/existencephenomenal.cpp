#include "existencephenomenal.hpp"
#include "trace.hpp"

using namespace Phenomenal;

Existence::Existence(const Interactions &interactions,
                     const std::size_t &maxInteractionHeight)
  :
    mood(MOOD::SELF_SATISFIED),
    interactions(interactions, maxInteractionHeight, &this->phenomenas),
    phenomenas(this->interactions, true),
    intended(NULL),
    enacted(NULL)
{

}

Existence::~Existence()
{

}

Interaction *Existence::stepExperience()
{
//  intended = phenomenas.getActivatedInteraction(enacted);
  clearMood(mood);
  if(enacted == NULL)
    intended = phenomenas.withMaxExpectedOutcomeValence(enacted);
  else if(enacted->getUsageTable()->needTest())
    {
      addMood(mood, MOOD::CURIOUS);
      intended = phenomenas.leastTriedExperience(enacted);
    }
  else
    intended = phenomenas.withMaxExpectedOutcomeValence(enacted);
  return intended;
}

void Existence::stepResult(Interaction *enacted)
{
  Trace::setEnacted(enacted);
  phenomenas.intendedAndEnacted(intended, enacted);
  this->enacted = enacted;
}

Interactions &Existence::getInteractions()
{
  return interactions;
}

const Interactions &Existence::getInteractions() const
{
  return interactions;
}

const Phenomenas &Existence::getPhenomenas() const
{
  return phenomenas;
}

Phenomenas &Existence::getPhenomenas()
{
  return phenomenas;
}

MOOD Existence::getMood() const
{
  return mood;
}

