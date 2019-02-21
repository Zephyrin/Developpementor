#ifndef AGENT_H
#define AGENT_H

#include "agent.hpp"

#include <existencehedonism.hpp>
#include <phenomenas.hpp>

#include <cstddef> // std::size_t
#include <vector> // std::vector
namespace Hedonism
{
  class Agent : public RadicalInteractionism::Agent
  {
  public:
    Agent(const Interactions &interactions,
             const std::size_t &maxInteractionHeight,
             const std::size_t &sameInteractionBeforeBored,
             const bool usePhenomenas);

    const Interaction *getExperience();
    void resultExperience(const Interaction *enacted);

    const Phenomenas *getPhenomenas() const;
    std::vector<const Interaction *> getInteractions() const;

    MOOD getMood() const;
  protected:

  private:

    Hedonism::Existence existence;

    bool usePhenomenas;

  };
}

#endif // AGENT_H
