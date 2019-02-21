#include "phenomenas.hpp"
#include "trace.hpp"
#include "signature.hpp"

#include <iostream>
#include <algorithm> // std::transform
Phenomenas::Phenomenas(Interactions &interactions, bool setPersistent)
  :
    phenomenas(),
    list(),
    previousEnacted(NULL),
    previousIntended(NULL),
    interactions(interactions),
    unknownInBelieveState(NULL  )
{
  for(Interactions::pInteractions pair : interactions)
    {
      UsageTable *usageTable = addOrGetPhenomena(pair.second);
      if(setPersistent)
        usageTable->setPersistent();
    }
  for(auto pairNoema : phenomenas)
    {
      for(auto pairSignature : phenomenas)
        {
          UsageTable *noema = pairNoema.second;
          UsageTable *signature = pairSignature.second;
          noema->addPostInteraction(signature);
          noema->addPreInteraction(signature);
        }
    }
}

Phenomenas::~Phenomenas()
{
}

void Phenomenas::intendedAndEnacted(Interaction *intended, Interaction *enacted)
{
  modifiedNoema.clear();
  if(previousEnacted != NULL && previousIntended != NULL)
    {
      if(unknownInBelieveState)
        {
          if(enacted != intended)
            {
              unknownInBelieveState->believeNotTrue();
            }
        }
      unknownInBelieveState = NULL;
      enacted->getUsageTable()->prevIntendedPrevEnacted(
            previousIntended
            , previousEnacted);
      // Retrieve the modified noema for the trace.
      if(getPhenomena(previousEnacted->getId()))
        {
          int value;
          if(getPhenomena(previousEnacted->getId())->getStatus()
             == UsageTable::STATUS::PERSISTENT)
            {
              if(previousEnacted != enacted)
                {
                  value = getPhenomena(previousEnacted->getId())
                      ->getPostInteractions().getNbEnacted(enacted);
                  if(value == 0)
                    modifiedNoema.push_back(
                          getPhenomena(previousEnacted->getId()));
                }
            }
        }
      const UsageTable *believe = NULL;
      if(previousEnacted->getUsageTable()->getStatus()
         == UsageTable::STATUS::UNPERSISTENT)
        believe = previousEnacted->getUsageTable()->getBelieve();
      previousEnacted->getUsageTable()->nextIntendedNextEnacted(
            intended
            , enacted);
      if(previousEnacted->getUsageTable()->getStatus()
         == UsageTable::STATUS::UNPERSISTENT
         && believe
         != previousEnacted->getUsageTable()->getBelieve())
        modifiedNoema.push_back(previousEnacted->getUsageTable());
      if(intended != enacted)
        {
          intended->getUsageTable()
              ->prevEnactedWithFalseIntended(previousEnacted);
//          if(getPhenomena(intended->getId()))
//            getPhenomena(intended->getId())
//                ->prevEnactedWithFalseIntended(previousEnacted);
//          else
//            unknown.prevEnactedWithFalseIntended(previousEnacted, intended);
        }
      if(previousIntended == intended && previousEnacted != enacted)
        {
//          if(enacted->getUsageTable()->getStatus()
//             == UsageTable::STATUS::PERSISTENT)
//            enacted->getUsageTable()->unpersistent();
          if(intended->getUsageTable()->getStatus()
             == UsageTable::STATUS::PERSISTENT)
            setNoemaUnpersistent(intended);
        }
    }
  intended->getUsageTable()->addNbEnacted(enacted);
  if(intended != enacted)
    {
      enacted->getUsageTable()->addNbEnacted(enacted);
    }
  previousIntended = intended;
  previousEnacted = enacted;
}

UsageTable *Phenomenas::getNoema(const Interaction *interaction) const
{
  if(phenomenas.find(interaction->getId()) != phenomenas.end())
    return phenomenas.at(interaction->getId());
  return NULL;
}

void Phenomenas::addInteraction(Interaction *interaction)
{
  if(phenomenas.find(interaction->getId()) == phenomenas.end())
    {
      UsageTable *usageTable = interaction->getUsageTable();
      phenomenas[interaction->getId()] = usageTable;
      list.push_back(phenomenas[interaction->getId()]);
      for(auto pairNoema : phenomenas)
        {
          UsageTable *noema = pairNoema.second;
          noema->addPostInteraction(usageTable);
          noema->addPreInteraction(usageTable);
          usageTable->addPostInteraction(noema);
          usageTable->addPreInteraction(noema);
        }
      if(!interaction->isPrimitive())
        {
          Interaction *preInteraction = interaction->getPreInteraction();
          Interaction *postInteraction = interaction->getPostInteraction();
          while(!preInteraction->isPrimitive())
            {
              preInteraction = preInteraction->getPreInteraction();
            }
          while(!postInteraction->isPrimitive())
            {
              postInteraction = postInteraction->getPostInteraction();
            }
          usageTable->setPreInteraction(preInteraction);
          usageTable->setPostInteraction(postInteraction);
        }
    }
}

const std::vector<const UsageTable *> &Phenomenas::toListOfNoema() const
{
  return list;
}

bool compAnticipatesLeast(const Anticipation *a,
                          const Anticipation *b)
{
  return a->getProclivity() < b->getProclivity();
}

Interaction *Phenomenas::leastTriedExperience(Interaction *enacted) const
{
  std::vector<Anticipation *> anticipates;

  if(enacted == NULL)
    {
      for(std::pair<std::string, UsageTable *> pair : phenomenas)
        {
          addAnticiptation(anticipates
                           , pair.second
                           , enacted
                           , pair.second->calcNbTried());
        }
    }
  else if(enacted->getUsageTable()->getStatus()
          == UsageTable::STATUS::UNPERSISTENT)
    {
      for(auto const &it
          : enacted->getUsageTable()->getPostInteractions())
        {
          const Signature * signature = it.second;
          addAnticiptation(anticipates
                           , signature->getInteraction()->getUsageTable()
                           , enacted
                           , signature->getAdditionEnactedIntended()
                           );
        }
    }
  else
    {
      for(auto const &it
          : enacted->getUsageTable()->getPostInteractions())
        {
          const Signature * signature = it.second;
          addAnticiptation(anticipates
                           , signature->getInteraction()->getUsageTable()
                           , enacted
                           , signature->getAdditionEnactedIntended()
                           );
        }
    }
  std::sort(anticipates.begin(), anticipates.end(), compAnticipatesLeast);
  for(Anticipation *anticipate : anticipates)
    {
      if(enacted)
        {
          anticipate->changeInteraction(
                enacted->getUsageTable()->getLeastExperience(
                  anticipate->getInteraction()->getUsageTable()
                  ));
        }
      Trace::addProposed(anticipate);
    }
  Interaction *intended = anticipates.front()->getInteraction();
  for(Anticipation *anticipate : anticipates)
    delete anticipate;
  return intended;
}


bool compAnticipates(const Anticipation *a,
                     const Anticipation *b)
{
  return a->getProclivity() > b->getProclivity();
}

Interaction *Phenomenas::withMaxExpectedOutcomeValence(Interaction *enacted)
{
  Interaction *intended = NULL;

  std::vector<Anticipation *> anticipates;
  if(enacted == NULL)
    {
      return phenomenas.begin()->second->getInteraction();
    }
  if(enacted->getUsageTable()->getStatus() == UsageTable::STATUS::PERSISTENT)
    {
      if(enacted->getValence() > 0)
        addAnticiptation(anticipates
                        , enacted->getUsageTable()
                        , enacted
                        , enacted->getValence());
      for(auto const &it
          : getPhenomena(enacted->getId())->getPostInteractions())
        {
          const Signature *signature = it.second;
          if(signature->isAchievable()
             && it.first != enacted->getId())
            {
              addAnticiptation(anticipates
                               , signature->getInteraction()->getUsageTable()
                               , enacted
                               , signature->getReward()
                               );
            }
        }
    }
  else
    {
      if(enacted->getUsageTable()->getBelieve() != NULL)
        {
          unknownInBelieveState = enacted->getUsageTable();
          Interaction *intended =  withMaxExpectedOutcomeValence(
                enacted->getUsageTable()->getBelieve()->getInteraction());
          enacted->setBestNextInteraction(intended);
          return intended;
        }
      for(auto const &it
          : enacted->getUsageTable()->getPostInteractions() )
        {
          Signature *signature = it.second;
          if(signature->getInteraction()->getUsageTable()->getStatus()
             == UsageTable::STATUS::PERSISTENT)
            {
              addAnticiptation(anticipates
                               , signature->getInteraction()->getUsageTable()
                               , enacted
                               , signature->getReward()
                               );
            }
        }
    }
  if(anticipates.empty())
    {
      return leastTriedExperience(enacted);
    }
  std::sort(anticipates.begin(), anticipates.end(), compAnticipates);
  for(Anticipation *anticipate : anticipates)
    {
      Interaction *interaction = getNoema(enacted)->getBestExperience(
            getNoema(anticipate->getInteraction()));
      if(interaction)
        anticipate->changeInteraction(interaction);
      Trace::addProposed(anticipate);
    }
  intended = anticipates.front()->getInteraction();
  enacted->setBestNextInteraction(intended);
  for(Anticipation *anticipate : anticipates)
    delete anticipate;
  return intended;
}

void Phenomenas::setNoemaUnpersistent(Interaction *enacted)
{
  if(getPhenomena(enacted->getId()))
    {
      if(getPhenomena(enacted->getId())->getStatus()
         != UsageTable::STATUS::UNPERSISTENT)
        {
          getPhenomena(enacted->getId())->unpersistent();
          modifiedNoema.push_back(getPhenomena(enacted->getId()));
        }
//      placeNoemeInUnknownBelieve(enacted);
    }
}

void Phenomenas::setNoemaPersistent(Interaction *enacted)
{
  if(getPhenomena(enacted->getId()))
    {
      if(getPhenomena(enacted->getId())->getStatus()
         != UsageTable::STATUS::PERSISTENT)
        {
          getPhenomena(enacted->getId())->setPersistent();
          modifiedNoema.push_back(getPhenomena(enacted->getId()));
        }
    }
}

const std::vector<const UsageTable *> *Phenomenas::getModifiedNoema() const
{
  return &modifiedNoema;
}

bool Phenomenas::isPhenomene(Interaction *interaction) const
{
  return phenomenas.find(interaction->getId()) != phenomenas.end();
}

UsageTable *Phenomenas::addOrGetPhenomena(Interaction *interaction)
{
  if(phenomenas.find(interaction->getId()) == phenomenas.end())
    {
      phenomenas[interaction->getId()]
          = interaction->getUsageTable();
      list.push_back(phenomenas[interaction->getId()]);
    }
  return phenomenas[interaction->getId()];
}

UsageTable *Phenomenas::getPhenomena(const std::string &id) const
{
  {
    if(phenomenas.find(id) == phenomenas.end())
      return NULL;
  }
  return phenomenas.at(id);
}

float Phenomenas::calcProclivity(const std::size_t nbEnacted
                                 , const std::size_t nbIntended
                                 , const float valence) const
{
  float proclivity = (int)nbEnacted - (int)nbIntended;
  //  float proclivity;
  //   if(pair.first + pair.second == 0)
  //    proclivity = 0;
  //  else
  //    proclivity = (float)((int)pair.first - (int)pair.second)
  //        / (pair.first + pair.second);
  if(proclivity < 0 && valence < 0)
    proclivity *= (-1 * valence);
  else
    proclivity *= valence;
  return proclivity;
}

void Phenomenas::addAnticiptation( std::vector<Anticipation *> &anticipates
                                   , UsageTable *noema
                                   , const Interaction *enacted
                                   , const float proclivity) const
{
  std::vector<Anticipation *>::iterator it;
  it = std::find(anticipates.begin(), anticipates.end(),
                 *(noema->getInteraction()));
  if(it == anticipates.end())
    {
      bool found = false;
      for(UsageTable *opposite : noema->getOpposites())
        {
          it = std::find(anticipates.begin(), anticipates.end(),
                         *(opposite->getInteraction()));
          if(it != anticipates.end())
            {
//              if((*it)->getInteraction() == enacted)
//                (*it)->addProclivity(proclivity);
              (*it)->addProclivity(proclivity);
              found = true;
            }
        }
      if(!found)
        anticipates.push_back(
              new Anticipation(
                noema->getInteraction(),
                proclivity
                )
              );
    }
  else
    (*it)->addProclivity(proclivity);
}

