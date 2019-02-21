#include "signatures.hpp"
#include "interaction.hpp"
#include "usagetable.hpp"
#include <climits> // min int

Signatures::Signatures()
{

}

Signatures::~Signatures()
{
  for(std::pair<std::string, Signature *> psignature: elements)
    delete psignature.second;
}

void Signatures::add(Interaction *interaction)
{
  if(elements.find(interaction->getId()) == elements.end())
    {
      elements[interaction->getId()]
          = new Signature(interaction);
    }
}

std::size_t Signatures::getNbEnacted(const Interaction *interaction) const
{
  return elements.at(interaction->getId())->getNbEnacted();
}

std::size_t Signatures::getNbEnacted(const std::string &id) const
{
  return elements.at(id)->getNbEnacted();
}

std::size_t Signatures::getNbIntended(const Interaction *interaction) const
{
  return elements.at(interaction->getId())->getNbIntended();
}

std::size_t Signatures::getNbTried(const Interaction *interaction) const
{
  return elements.at(interaction->getId())->getNbTried();
}

void Signatures::addNbEnacted(const Interaction *interaction)
{
  elements.at(interaction->getId())->addNbEnacted();
}

void Signatures::addNbIntended(const Interaction *interaction)
{
  elements.at(interaction->getId())->addNbIntended();
}

bool Signatures::has(const Interaction *interaction) const
{
  return elements.find(interaction->getId()) != elements.end();
}

float Signatures::getNextValence() const
{
  float valence = INT_MIN;
  float bestNextInteraction = INT_MIN;
  for(std::pair<std::string, Signature *> pair : elements)
    {
      if(pair.second->isAchievable())
        {
          if(pair.second->getReward()
             > bestNextInteraction)
            {
              valence = pair.second->getValence();
              bestNextInteraction
                  = pair.second->getReward();
            }
        }
    }
  return valence;
}

float Signatures::getFactor(const Interaction *interaction) const
{
  return elements.at(interaction->getId())->getFactor();
}

bool Signatures::needTest() const
{
  for(std::pair<std::string, Signature *> pair : elements)
    {
      if(pair.second->needTest())
        {
          std::vector<UsageTable *> vector
              = pair.second->getInteraction()->getUsageTable()->getOpposites();
          bool need = true;
          for(UsageTable *opposite : vector)
            {
              if(!elements.at(opposite->getInteraction()->getId())->needTest())
                need = false;
            }
          if(need)
            return true;
        }
    }
  return false;
}

Signatures &Signatures::operator +=(const Signatures &b)
{
  for(auto pair : b.elements)
    {
      Signature *signature = pair.second;
      if(elements.find(pair.first) != elements.end())
        {
          elements[pair.first]->addNbIntended(signature->getNbIntended());
          elements[pair.first]->addNbEnacted(signature->getNbEnacted());
        }
      else
        {
          elements[pair.first] = new Signature(*(signature));
        }
    }
  return *this;
}

bool Signatures::isCompatibleWithInteraction(
    const Interaction *interaction) const
{
  return elements.at(interaction->getId())->getNbEnacted()
      - elements.at(interaction->getId())->getNbIntended() > 0;
}

Signatures::signatures_iterator Signatures::begin() const
{
  return elements.cbegin();
}

Signatures::signatures_iterator Signatures::end() const
{
  return elements.cend();
}

bool Signatures::isAchievable(const std::string &id) const
{
  return elements.at(id)->isAchievable();
}

bool Signatures::isConfirm(const std::string &id) const
{
  return elements.at(id)->isConfirm();
}

std::size_t Signatures::nbElements() const
{
  return elements.size();
}
