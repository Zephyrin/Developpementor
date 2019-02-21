#include "interaction.hpp"
#include "signature.hpp"
#include "usagetable.hpp"
Signature::Signature(Interaction *interaction)
  :
    interaction(interaction)
  , nbEnacted(0)
  , nbIntended(0)
{

}

Signature::Signature(const Signature &signature)
  :
    interaction(signature.interaction)
  , nbEnacted(signature.nbEnacted)
  , nbIntended(signature.nbIntended)
{

}

Signature::~Signature()
{

}

std::size_t Signature::getNbEnacted() const
{
  return nbEnacted;
}
std::size_t Signature::getNbIntended() const
{
  return nbIntended;
}

std::size_t Signature::getNbTried() const
{
  return nbEnacted + nbIntended;
}

void Signature::addNbEnacted()
{
  nbEnacted ++;
}

void Signature::addNbIntended()
{
  nbIntended ++;
}

void Signature::addNbEnacted(const std::size_t nbEnacted)
{
  this->nbEnacted += nbEnacted;
}

void Signature::addNbIntended(const std::size_t nbIntended)
{
  this->nbIntended += nbIntended;
}

bool Signature::isAchievable() const
{
  if(nbEnacted == 0 && nbIntended == 0)
    return true;
  return nbEnacted * 50. / 100. >= nbIntended;
}

float Signature::getReward() const
{
  return interaction->getUsageTable()->getReward();
}

float Signature::getFactor() const
{
  if(nbEnacted == 0)
    return nbEnacted;
  return (float)nbEnacted / (float)(nbEnacted + nbIntended);
}

float Signature::getValence() const
{
  return interaction->getValence();
}

bool Signature::needTest() const
{
  return nbEnacted + nbIntended < 1;
}

std::size_t Signature::getAdditionEnactedIntended() const
{
  return nbEnacted + nbIntended;
}

const Interaction *Signature::getInteraction() const
{
  return interaction;
}

bool Signature::isConfirm() const
{
//  return nbEnacted > 0 && nbIntended == 0;
  return nbEnacted * 5. / 100. > nbIntended;
}
