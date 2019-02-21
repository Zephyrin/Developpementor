#include "minimumalgorithm.hpp"
#include "trace.hpp"
#include <iostream>

using namespace MinimumAlgorithm;

Agent::Agent(const Interactions &interactions,
             const std::size_t &maxInteractionHeight
             , const std::size_t &excitementThreshold)
  :
  existence(interactions,
            maxInteractionHeight
            , excitementThreshold)
{

}

Agent::~Agent()
{

}
const Interaction *Agent::getExperience()
{
  if(currentExperience == intendedExperiences.end())
    {
      Trace::startNewEvent();
      intendedInteractionGlobal = existence.stepExperience();
      Trace::setIntended(intendedInteractionGlobal);
      enactedInteractionGlobal = NULL;

      moodEnacted = *(existence.getMood());
      createExperiencesSequence();
    }
  return primitiveAction();
}

void Agent::resultExperience(const Interaction *enacted)
{
  Interaction *interaction = *currentExperience;

  currentExperience ++;
  if(interaction->isPrimitive())
    {
      Interaction *enactedInteraction = existence.getInteractions()
          .addOrGetPrimitiveInteraction(enacted->getId());
      existence.getPhenomenas().intendedAndEnacted(interaction,
                                                   enactedInteraction);
      enactedInteractions.push_back(enactedInteraction);
    }
  if(currentExperience != intendedExperiences.end())
    {
      while(currentExperience != intendedExperiences.end()
            && !(*currentExperience)->isPrimitive())
        {
          Interaction *postInteraction = enactedInteractions.back();
          enactedInteractions.pop_back();
          Interaction *preInteraction = enactedInteractions.back();
          enactedInteractions.pop_back();
          Interaction *enactedInteraction =
              existence.getInteractions().addOrGetCompositeInteraction(
                preInteraction, postInteraction);
          enactedInteractions.push_back(enactedInteraction);
          interaction = *currentExperience;
          currentExperience ++;
        }
    }
  if(currentExperience == intendedExperiences.end()
     || enactedInteractions.back() != interaction)
    {
      enactedInteractionGlobal = NULL;
      currentExperience = intendedExperiences.end();
      while(!enactedInteractions.empty())
        {
          Interaction *postInteraction = enactedInteractions.back();
          enactedInteractions.pop_back();
          if(enactedInteractions.empty())
            {
              if(enactedInteractionGlobal != NULL)
                {
                  enactedInteractionGlobal
                      = existence.getInteractions()
                      .addOrGetCompositeInteraction(postInteraction
                                                    , enactedInteractionGlobal);
                }
              else
                enactedInteractionGlobal = postInteraction;
            }
          else
            {
              if(enactedInteractionGlobal != NULL)
                {
                  Interaction *preInteraction = enactedInteractions.back();
                  enactedInteractions.pop_back();
                  enactedInteractionGlobal
                      = existence.getInteractions()
                      .addOrGetCompositeInteraction(preInteraction
                                                    , enactedInteractionGlobal);
                }
              else
                {
              Interaction *preInteraction = enactedInteractions.back();
              enactedInteractions.pop_back();
              enactedInteractionGlobal
                  = existence.getInteractions()
                  .addOrGetCompositeInteraction(preInteraction,
                                                postInteraction);
                }
            }
        }
      if(enactedInteractions.size() > 1)
        {
          std::cout << "Enacted interactions not clear." << std::endl;
          for(Interaction *en : enactedInteractions)
            {
              std::cout << *en << std::endl;
            }
          std::cout << std::endl;
          for(Interaction *in : intendedExperiences)
            {
              std::cout << *in << std::endl;
            }
          std::cout << std::endl << "intended: " << std::endl
                    << *intendedInteractionGlobal << std::endl << "Enacted:"
                    << std::endl << *enactedInteractionGlobal << std::endl;

          std::exit(2);
        }
      existence.stepResult(enactedInteractionGlobal);
      moodEnacted = *(existence.getMood());
      Trace::closeEvent();
    }
}


const Phenomenas *Agent::getPhenomenas() const
{
  return &(existence.getPhenomenas());
}

std::vector<const Interaction *> Agent::getInteractions() const
{
  return existence.getInteractions().toVector();
}

MOOD Agent::getMood() const
{
  return moodEnacted;
}

const std::vector<const UsageTable *> *Agent::getModifiedNoema() const
{
  return existence.getModifiedNoema();
}

const UsageTable *Agent::getPhenomene() const
{
  return existence.getPhenomene();
}

