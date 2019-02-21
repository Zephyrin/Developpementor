#include "existenceminimumalgorithm.hpp"

#include "trace.hpp"

using namespace MinimumAlgorithm;

Existence::Existence(const Interactions &interactions
                     , const std::size_t &maxInteractionHeight
                     , const std::size_t &excitementThreshold)
  :
    mood(MOOD::CURIOUS),
    interactions(interactions, maxInteractionHeight, &(this->phenomenas)),
    phenomenas(this->interactions),
    intended(NULL),
    enacted(NULL)
  , excitement(0)
  , excitementThreshold(excitementThreshold)
//  , modifiedNoema(NULL)
{

}

Existence::~Existence()
{

}

Interaction *Existence::stepExperience()
{
  if(moodAhasB(MOOD::EXCITED, mood))
    {
      intended = enacted;
    }
  else if(moodAhasB(MOOD::CURIOUS, mood))
    {
      intended = phenomenas.leastTriedExperience(enacted);
    }
  else if(moodAhasB(MOOD::HEDONIST, mood))
    {
      intended = phenomenas.withMaxExpectedOutcomeValence(enacted);
    }
  Trace::setIntended(intended);
  return intended;
}

void Existence::stepResult(Interaction *enacted)
{
  Trace::setEnacted(enacted);
  bool newBelieve = false;
//  modifiedNoema = NULL;
  if(moodAhasB(MOOD::EXCITED, mood))
    {
      if(enacted != intended)
        {
          newBelieve = true;
          phenomenas.setNoemaUnpersistent(intended);
          excitement = 0;
        }
      else if(excitement >= excitementThreshold)
        {
          newBelieve = true;
          phenomenas.setNoemaPersistent(enacted);
//          modifiedNoema = phenomenas.getPhenomena(enacted->getId());
          excitement = 0;
        }
      else
        excitement ++;
    }
//  if(modifiedNoema == NULL)
//    modifiedNoema = phenomenas.getModifiedNoema();
  bool previousExcited = moodAhasB(mood, MOOD::EXCITED);
  clearMood(mood);
  if(!previousExcited)
    {
      addMood(mood, MOOD::HEDONIST);
    }
  if(phenomenas.getPhenomena(enacted->getId()) != NULL
     && phenomenas.getPhenomena(enacted->getId())->getStatus()
     == UsageTable::STATUS::UNKNOWN)
    {
      addMood(mood, MOOD::EXCITED);
    }
  else if(newBelieve)
    {
      addMood(mood, MOOD::CURIOUS);
    }
  else if(enacted->getUsageTable()->needTest())
    {
      addMood(mood, MOOD::CURIOUS);
    }
  Trace::setMood(mood);
  // Done in agent.
//  phenomenas.intendedAndEnacted(this->intended, enacted);
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

const MOOD *Existence::getMood() const
{
  return &mood;
}

const std::vector<const UsageTable *> *Existence::getModifiedNoema() const
{
  return phenomenas.getModifiedNoema();
}

const UsageTable *Existence::getPhenomene() const
{
  return phenomenas.getNoema(enacted);
}

