#include "interaction.hpp"
#include "trace.hpp"

#include <algorithm> // std::replace
#include <sstream>

Interaction::Interaction(const std::string  &label,
                         const int valence)
  :
    id(label),
    valence(valence),
    preInteraction(NULL),
    postInteraction(NULL),
    weight(0),
    height(1)
  , usageTable(NULL)
  , bestNextInteraction(NULL)
{
  usageTable = new UsageTable(this, false);
  Trace::addLearned(this);
}

Interaction::Interaction(const Interaction &interaction)
  :
    id(interaction.id),
    valence(interaction.valence),
    preInteraction(interaction.preInteraction),
    postInteraction(interaction.postInteraction),
    weight(interaction.weight),
    height(interaction.height)
  , usageTable(NULL)
  , bestNextInteraction(NULL)
{
  usageTable = new UsageTable(this, false);
//  Trace::addLearned(this);
}

Interaction::~Interaction()
{
  if(usageTable)
    delete usageTable;
}

bool Interaction::isPrimitive() const
{
  return preInteraction == NULL;
}

float Interaction::getValence() const
{
  return valence;
}

void Interaction::setValence(float valence)
{
  this->valence = valence;
}
Interaction *Interaction::getPreInteraction() const
{
  return preInteraction;
}

void Interaction::setPreInteraction(Interaction *interaction)
{
  preInteraction = interaction;
  if(preInteraction->height >= height)
    height = preInteraction->height + 1;
  else
    height ++;
}

Interaction *Interaction::getPostInteraction() const
{
  return postInteraction;
}

void Interaction::setPostInteraction(Interaction *interaction)
{
  postInteraction = interaction;
  if(postInteraction->height >= height)
    height = postInteraction->height + 1;
  else
    height ++;

}

int Interaction::getWeight() const
{
  return weight;
}

void Interaction::incWeight()
{
  weight ++;
}

const std::string &Interaction::getId() const
{
  return id;
}

std::size_t Interaction::getHeight() const
{
  return height;
}

Interaction *Interaction::getLastInteraction()
{
  if(postInteraction != NULL)
    return postInteraction->getLastInteraction();
  return this;
}

void Interaction::addEnacted(Interaction *interactionEnacted)
{
  if(std::find(enacteds.begin(), enacteds.end(), interactionEnacted)
     == enacteds.end())
    {
      enacteds.push_back(interactionEnacted);
    }
}

std::vector<Interaction *> &Interaction::getEnactedInteractions()
{
  return enacteds;
}

UsageTable *Interaction::getUsageTable() const
{
  return usageTable;
}

Interaction *Interaction::getBestNextInteraction() const
{
  return bestNextInteraction;
}

void Interaction::setBestNextInteraction(Interaction *interaction)
{
  bestNextInteraction = interaction;
}

std::string Interaction::typeToKtbs(const std::string &simulationId) const
{
  std::stringstream os;
  os << labelToKtbs(simulationId) << " a m:Interaction"
        /*<< typeObsel*/  << ";" << std::endl
     << "i:label \"" << this->id << "\";" << std::endl;
  if(preInteraction != NULL)
    {
      os << "m:withPreInteraction " << preInteraction->labelToKtbs(simulationId)
         << ";" << std::endl;
    }
  if(postInteraction != NULL)
    {
      os << "m:withPostInteraction "
         << postInteraction->labelToKtbs(simulationId)
         << ";" << std::endl;
    }
  os << ":hasTrace <.> ;" << std::endl;
  return os.str();
}

std::string Interaction::labelToKtbs(const std::string &simulationId) const
{
  std::string label(this->id);
  std::replace(label.begin(), label.end(), '>', '_');
  std::replace(label.begin(), label.end(), '<', '_');
  return "<" + label + "_" + simulationId + ">";
}

std::string Interaction::toKtbs(const std::string &id,
                                const std::string typeObsel,
                                const std::size_t &timeStep) const
{
  std::stringstream os;
  os << "[ a " << typeObsel << ";" << std::endl
     << "m:withInteraction " << labelToKtbs(id)
     << ";" << std::endl
     << ":hasTrace <.> ;" << std::endl
     << "is:weight " << weight << ";" << std::endl
     << "is:valence " << valence << ";" << std::endl
     << "m:onStep <" << timeStep << "_" << id << "> ;]." << std::endl;

  return os.str();
}

std::string Interaction::toText() const
{
  std::stringstream os;
  os << "weight : " << weight << ", valence : " << valence
     << ", height : " << height << ", id : " << id;
  return os.str();
}

bool operator !=(const Interaction *a, const Interaction &b)
{
  return a->id != b.id;
}

bool operator ==(const Interaction *a, const Interaction &b)
{
  return a->id == b.id;
}

std::ostream &operator <<(std::ostream &os, const Interaction &interaction)
{
  os << interaction.id << " valence " << interaction.valence << " weight "
     << interaction.weight;
  return os;
}
