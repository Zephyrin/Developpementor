#ifndef PHENOMENAL_HPP
#define PHENOMENAL_HPP

#include "agent.hpp"
#include "existencephenomenal.hpp"

namespace Phenomenal {

  class Agent : public RadicalInteractionism::Agent
  {
  public:
    Agent(const Interactions &interactions,
          const std::size_t &maxInteractionHeight);
    ~Agent();

    // Agent interface
    const Interaction *getExperience();
    void resultExperience(const Interaction *enacted);
    const Phenomenas *getPhenomenas() const;
    std::vector<const Interaction *> getInteractions() const;
    MOOD getMood() const;
    const UsageTable *getPhenomene() const;
    const std::vector<const UsageTable *> *getModifiedNoema() const;
  protected:
    Phenomenal::Existence existence;
  };

}
#endif // PHENOMENAL_HPP
