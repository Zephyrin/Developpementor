#ifndef EXISTENCEMINIMUMALGORITHM_H
#define EXISTENCEMINIMUMALGORITHM_H

#include "interactions.hpp"
#include "phenomenas.hpp"
#include "mood.hpp"

namespace MinimumAlgorithm
{
  class Existence
  {
  public:
    Existence(const Interactions &interactions,
              const std::size_t &maxInteractionHeight,
              const std::size_t &excitementThreshold);
    ~Existence();

    Interaction *stepExperience();

    void stepResult(Interaction *enacted);
    Interactions &getInteractions();
    const Interactions &getInteractions() const;

    const Phenomenas &getPhenomenas() const;
    Phenomenas &getPhenomenas();

    const MOOD *getMood() const;

    const std::vector<const UsageTable *> *getModifiedNoema() const;
    const UsageTable *getPhenomene() const;
  private:
    MOOD mood;

    Interactions interactions;

    Phenomenas phenomenas;

    Interaction *intended;

    Interaction *enacted;

    std::size_t excitement;
    std::size_t excitementThreshold;
//    const UsageTable *modifiedNoema;
  };
}
#endif // EXISTENCEMINIMUMALGORITHM_H
