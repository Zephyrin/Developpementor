#include "existencehedonism.hpp"
#include "trace.hpp"

#include <algorithm> // std::sort std::transform
#include <iostream>

using namespace Hedonism;

Existence::Existence(const Interactions &interactions,
                     const std::size_t &maxInteractionHeight,
                     const std::size_t &maxInteractionBeforeBored,
                     const bool usePhenomenas)
  :
    mood(MOOD::SELF_SATISFIED),
    previousPhenomenaMood(MOOD::BORED),
    selfSatisfactionCounter(0),
    currentExperience(NULL),
    interactions(interactions, maxInteractionHeight, &phenomenas),
    phenomenas(this->interactions, true),
    enactedInteraction(NULL),
    intendedInteraction(NULL),
    currentEnactedInteraction(NULL),
    sameInteraction(0),
    maxInteractionBeforeBored(maxInteractionBeforeBored),
    maxInteractionCurious(5),
    usePhenomenas(usePhenomenas)
{
}

Existence::~Existence()
{

}

Interaction *Existence::stepExperience()
{
  std::vector<Anticipation *> anticipations;
  anticipate(anticipations);
  currentExperience = selectExperiment(anticipations);
//  if(maxInteractionBeforeBored > 0)
//    {
//      if(intendedInteraction == currentExperience)
//        sameInteraction ++;
//      else
//        sameInteraction = 0;
//      intendedInteraction = currentExperience;
//    }
//  else
//    {
  intendedInteraction = currentExperience;
//    }
  for(Anticipation *anticipation : anticipations)
    delete anticipation;
  return intendedInteraction;
}

void Existence::stepResult(Interaction *enactedInteraction)
{
  Trace::setEnacted(enactedInteraction);
//  bool curious = moodIs(MOOD::CURIOUS);
  clearMood();
/*  if(maxInteractionBeforeBored > 0 && curious)
    {

    }
  else*/ if(enactedInteraction != intendedInteraction)
    {
      currentExperience->addEnacted(enactedInteraction);
      addMood(MOOD::FRUSTRATED);
    }
  else
    addMood(MOOD::SELF_SATISFIED);
  if(maxInteractionBeforeBored > 0 &&
     sameInteraction >= maxInteractionBeforeBored)
    addMood(MOOD::BORED);
  if(enactedInteraction->getValence() >= 0)
    addMood(MOOD::PLEASED);
  else
    addMood(MOOD::PAINED);
  if(this->enactedInteraction != NULL)
    {
//      if(/*!usePhenomenas*/ false)
//        {
//          if(enactedInteraction == this->enactedInteraction)
//            {
//              if(!phenomenas.isPhenomene(enactedInteraction))
//                interactions.learnCompositeInteraction(enactedInteraction,
//                                                       this->enactedInteraction
//                                                       );
//              else
//                {
//                  intendedInteraction->incWeight();
//                  enactedInteraction->incWeight();
//                }
//            }
//          else
            interactions.learnCompositeInteraction(enactedInteraction,
                                                   this->enactedInteraction);
//        }
//      else
//        {
//          interactions.learnCompositeInteraction(enactedInteraction,
//                                                 this->enactedInteraction);
//        }
    }
  this->enactedInteraction = enactedInteraction;
}

MOOD Existence::getMood() const
{
  return mood;
}

void Existence::clearMood()
{
  mood = MOOD::NO_MOOD;
}

void Existence::addMood(const MOOD &value)
{
  mood = (mood | value);
  Trace::setMood(mood);
}

bool Existence::moodIs(MOOD value) const
{
  return (mood & value) != 0;
}

void Existence::incSelfSatisfactionCounter()
{
  selfSatisfactionCounter ++;
}

void Existence::resetSelfSatisfactionCounter()
{
  selfSatisfactionCounter = 0;
}

//void Existence::anticipateOnPhenomenas(std::vector<Anticipation *>
//                                       & anticipations)
//{
//  getDefaultAnticipations(anticipations);
//  if(this->enactedInteraction == NULL)
//    {
//      return;
//    }
//  std::vector<Interaction *> primitiveActivateds
//      = phenomenas.getActivatedInteractions(this->enactedInteraction);
//  for(Interaction *primitiveActivated : primitiveActivateds)
//    {
//      for(Interaction *activated
//          : interactions.getActivatedInteractions(primitiveActivated))
//        {
//          std::vector<Anticipation *>::iterator it;
//          it = std::find(anticipations.begin(), anticipations.end(),
//                         *(activated->getPostInteraction()));
//          if(it == anticipations.end())
//            anticipations.push_back(
//                  new Anticipation(
//                    activated->getPostInteraction(),
//                    activated->getWeight()
//                    * activated->getPostInteraction()->getValence()));
//          else
//            (*it)->addProclivity(activated->getWeight()
//                                 * activated->getPostInteraction()
//                                 ->getValence());
//        }
//    }
//}

void Existence::anticipate(std::vector<Anticipation *> & anticipations)
{

  std::vector<Interaction *> activateds
      = interactions.getActivatedInteractions(this->enactedInteraction);
//  if(activateds.size() < 3)
    getDefaultAnticipations(anticipations);
  if(this->enactedInteraction != NULL)
    {
      for(Interaction *activated : activateds)
        {
          std::vector<Anticipation *>::iterator it;
          it = std::find(anticipations.begin(), anticipations.end(),
                         *(activated->getPostInteraction()));
          if(it == anticipations.end())
            anticipations.push_back(
                  new Anticipation(
                    activated->getPostInteraction(),
                    activated->getWeight()
                    * activated->getPostInteraction()->getValence()));
          else
            (*it)->addProclivity(activated->getWeight()
                                 * activated->getPostInteraction()
                                 ->getValence());
        }
    }

  for(Anticipation *anticipation : anticipations)
    {
      for(Interaction *interaction
          : anticipation->getInteraction()->getEnactedInteractions())
        {
          for(Interaction *activatedInteraction
              : activateds)
            {
              if(interaction == activatedInteraction->getPostInteraction())
                {
                  anticipation->removeProclivity(
                        activatedInteraction->getWeight()
                        * interaction->getValence() * 0.5);
                }
            }
        }
    }
}

void Existence::getDefaultAnticipations(
    std::vector<Anticipation *> & anticipations)
{
  for(Interaction *primitive : interactions.getPrimitiveInteractions())
    {
        anticipations.push_back(new Anticipation(
                                  primitive,
//                                  0
                                  primitive->getWeight()
                                  * primitive->getValence()));
    }
}

bool compProclivity(const Anticipation *a, const Anticipation *b)
{
  return a->getProclivity() > b->getProclivity();
}

Interaction *Existence::selectExperiment(
    std::vector<Anticipation *> &anticipations)
{
  std::sort(anticipations.begin(), anticipations.end(), compProclivity);
  std::vector<Anticipation *>::iterator it;
  for(it = anticipations.begin(); it != anticipations.end(); it++)
    {
      Trace::addProposed(*it);
    }
  return anticipations.front()->getInteraction();
}

Interaction *Existence::selectOtherExperiment(Interaction *intended)
{
  return interactions.getOtherInteraction(intended);
}

Interactions &Existence::getInteractions()
{
  return interactions;
}

const Interactions &Existence::getInteractions() const
{
  return interactions;
}

Phenomenas &Existence::getPhenomenas()
{
  return phenomenas;
}

const Phenomenas &Existence::getPhenomenas() const
{
  return phenomenas;
}
