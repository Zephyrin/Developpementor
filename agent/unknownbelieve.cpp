#include "unknownbelieve.h"
#include <algorithm> // std::find

UnknownBelieve::UnknownBelieve()
  :
    UsageTable(NULL, true)
{
}

UnknownBelieve::~UnknownBelieve()
{

}

void UnknownBelieve::addNoema(UsageTable *noema)
{
  if(isUnknown(noema))
    return;
  noemas[noema->getInteraction()->getId()] = noema;
  pre += noema->getPreInteractions();
  post += noema->getPostInteractions();
  enacteds += noema->getEnacted();
}

bool UnknownBelieve::isUnknown(Interaction *interaction) const
{
  return noemas.find(interaction->getId()) != noemas.end();
}

bool UnknownBelieve::isUnknown(const UsageTable *noema) const
{
  return noemas.find(noema->getInteraction()->getId()) != noemas.end();
}

void UnknownBelieve::prevIntendedPrevEnacted(Interaction *previousIntended,
                                             Interaction *previousEnacted,
                                             Interaction *enacted)
{
  UsageTable::prevIntendedPrevEnacted(previousIntended, previousEnacted);
  noemas[enacted->getId()]->prevIntendedPrevEnacted(previousIntended
                                                    , previousEnacted);
}

void UnknownBelieve::nextIntendedNextEnacted(Interaction *intended
                                             , Interaction *enacted
                                             , Interaction *previousEnacted)
{
  UsageTable::nextIntendedNextEnacted(intended, enacted);
  noemas[previousEnacted->getId()]->nextIntendedNextEnacted(intended, enacted);
}

void UnknownBelieve::addNbEnacted(UsageTable *noemaEnacted
                                  , Interaction *interaction
                                  )
{
//  UsageTable::addNbEnacted(noemaEnacted);
//  noemas[interaction->getId()]->addNbEnacted(noemaEnacted);
}

void UnknownBelieve::prevEnactedWithFalseIntended(Interaction *previousEnacted
                                                  , Interaction *intended)
{
  UsageTable::prevEnactedWithFalseIntended(previousEnacted);
  noemas[intended->getId()]->prevEnactedWithFalseIntended(previousEnacted);
}

bool UnknownBelieve::isEmpty() const
{
  return noemas.size() == 0;
}

const std::map<std::string, UsageTable *> &UnknownBelieve::getNoemas() const
{
  return noemas;
}

UsageTable *UnknownBelieve::getNoema(const Interaction *interaction) const
{
  return noemas.at(interaction->getId());
}

bool UnknownBelieve::hasNoema(const Interaction *interaction) const
{
  std::map<std::string, UsageTable *>::const_iterator it;
  it = noemas.find(interaction->getId());
  return it != noemas.end();
}

void UnknownBelieve::getActivatedInteraction(
    std::vector<Anticipation *> &anticipates
    , Interaction *enacted) const
{
  for(std::pair<std::string, UsageTable *> pair : noemas)
    {
      float proclivity = calcProclivity(
            pair.second->getPreInteractions().getNbEnacted(enacted)
            , pair.second->getPreInteractions().getNbIntended(enacted)
            , pair.second->getInteraction());
      addAnticiptation(anticipates, pair.second->getInteraction(),
                      proclivity);
    }
}

bool UnknownBelieve::isCompatibleWithNoema(const Interaction *interaction) const
{
  return pre.isCompatibleWithInteraction(interaction);
}


float UnknownBelieve::calcProclivity(const std::size_t nbEnacted
                                     , const std::size_t nbIntended
                                     , const Interaction *interaction) const
{
  float proclivity = (int)nbEnacted - (int)nbIntended;
  //  float proclivity;
  //   if(pair.first + pair.second == 0)
  //    proclivity = 0;
  //  else
  //    proclivity = (float)((int)pair.first - (int)pair.second)
  //        / (pair.first + pair.second);
  float valence = interaction->getValence();
  if(proclivity < 0 && valence < 0)
    proclivity *= (-1 * valence);
  else
    proclivity *= valence;
  return proclivity;
}

void UnknownBelieve::addAnticiptation( std::vector<Anticipation *> &anticipates
                                   , Interaction *interaction
                                   , const float proclivity) const
{
  std::vector<Anticipation *>::iterator it;
  it = std::find(anticipates.begin(), anticipates.end(),
                 *(interaction));
  if(it == anticipates.end())
    {
      anticipates.push_back(
            new Anticipation(
              interaction,
              proclivity
              )
            );
    }
  else
    (*it)->addProclivity(proclivity);
}


