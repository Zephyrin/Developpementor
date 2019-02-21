#ifndef MINIMUMALGORITHM_H
#define MINIMUMALGORITHM_H

#include "agent.hpp"
#include "existenceminimumalgorithm.hpp"

namespace MinimumAlgorithm {
  class Agent : public RadicalInteractionism::Agent
  {
  public:
    Agent(const Interactions &interactions,
                     const std::size_t &maxInteractionHeight
          ,const std::size_t &excitementThreshold);
    ~Agent();

    // Agent interface
    const Interaction *getExperience();
    void resultExperience(const Interaction *enacted);
    std::vector<const Interaction *> getInteractions() const;
    const Phenomenas *getPhenomenas() const;
    MOOD getMood() const;
    const std::vector<const UsageTable *> *getModifiedNoema() const;

    const UsageTable *getPhenomene() const;
  protected:
    MinimumAlgorithm::Existence existence;
    MOOD moodEnacted;
  };
}
#endif // MINIMUMALGORITHM_H
