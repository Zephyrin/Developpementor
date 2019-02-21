#include "environment.hpp"
#include <sstream>

Environment::Environment()
  :
    wellForm(true)
{

}

Environment::~Environment()
{

}

const Interactions &Environment::getInteractions() const
{
  return interactions;
}

bool Environment::getWellForm() const
{
    return wellForm;
}

std::string Environment::toKtbs(const std::string &simulationId) const
{
  std::stringstream os;
  for(std::pair<std::string, Interaction *> it
      : interactions)
    {
      os << it.second->toKtbs(simulationId, "primitive", 0);
      os << ":hasBegin 0 ." << std::endl;
    }
  return os.str();
}

