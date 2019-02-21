#include "anticipation.hpp"

#include <algorithm> // std::replace
#include <sstream>

Anticipation::Anticipation(Interaction *interaction,
                           float proclivity)
  :
    interaction(interaction)
  , oldInteractions()
  , proclivity(proclivity)
{

}

Anticipation::Anticipation(const Anticipation &anticipation)
  :
    interaction(anticipation.interaction)
    , oldInteractions(anticipation.oldInteractions)
    , proclivity(anticipation.proclivity)
{

}

Anticipation::~Anticipation()
{

}

Interaction *Anticipation::getInteraction() const
{
  return interaction;
}

void Anticipation::changeInteraction(Interaction *interaction)
{
  this->oldInteractions.push_back(this->interaction);
  this->interaction = interaction;
}

float Anticipation::getProclivity() const
{
  return proclivity;
}

void Anticipation::addProclivity(float value)
{
  proclivity += value;
}

void Anticipation::removeProclivity(float value)
{
  if(value > 0)
    proclivity -= value;
  else
    proclivity += value;
}

std::ostream &operator <<(std::ostream &os, const Anticipation &anticipation)
{
  os << *(anticipation.getInteraction()) << " Proclivity "
     << anticipation.proclivity;
  if(anticipation.oldInteractions.size() > 0)
    os << " (";
  for(const Interaction * interaction : anticipation.oldInteractions)
    os << " " << *interaction;
  if(anticipation.oldInteractions.size() > 0)
    os << " )";
  return os;
}

bool operator ==(const Anticipation *anticipation,
                 const Interaction &interaction)
{
   return anticipation->interaction == &interaction;
}

std::ostream & Anticipation::toKtbs(std::ostream &os,
                                    const std::string &step,
                                    const std::string &id) const
{
  std::string label(interaction->getId());
  std::replace(label.begin(), label.end(), '>', '_');
  std::replace(label.begin(), label.end(), '<', '_');
  std::replace(label.begin(), label.end(), '|', '_');
  os << "<p" << step << "_"
     << label << "_" << id  << "> a m:Proposed;" << std::endl
     << "pr:label \"" << *(interaction) << "\";" << std::endl
     << "pr:proclivity " << proclivity << ";" << std::endl
     << "m:withExperience <" << label << "_" << id << ">;" << std::endl
     << ":hasTrace <.> ;" << std::endl;
  return os;
}

std::string Anticipation::toText() const
{
  std::stringstream os;
  os << interaction->getId() << ", proclivity : " << proclivity;
  return os.str();
}

