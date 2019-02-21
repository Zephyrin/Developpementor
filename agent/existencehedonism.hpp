#ifndef EXISTENCEHEDONISM_HPP
#define EXISTENCEHEDONISM_HPP

#include "interactions.hpp"
#include "mood.hpp"
#include "phenomenas.hpp"
#include "anticipation.hpp"

#include <utility> // std::pair
#include <vector> // std::vector

namespace Hedonism {
  class Existence
  {
  public:
    Existence(const Interactions &interactions,
              const std::size_t &maxInteractionHeight,
              const std::size_t &maxInteractionBeforeBored,
              const bool usePhenomenas);
    ~Existence();

    Interaction *stepExperience();
    void stepResult(Interaction *enactedInteraction);

    Interactions &getInteractions();

    const Interactions &getInteractions() const;
    const Phenomenas &getPhenomenas() const;
    Phenomenas &getPhenomenas();

    MOOD getMood() const;
  protected:

    void clearMood();
    void addMood(const MOOD &value);
    bool moodIs(MOOD value) const;

    void incSelfSatisfactionCounter();

    void resetSelfSatisfactionCounter();

    void initExistence();

    void addOrGetExperience();

    void anticipate(std::vector<Anticipation *> &anticipations);

    void getDefaultAnticipations(std::vector<Anticipation *> &anticipations);

    //  Interaction *enact(Interaction *intendedInteraction);

    std::vector<Interaction *> &getActivatedInteractions();

    Interaction *selectExperiment(
        std::vector<Anticipation *> &anticipations);
    Interaction *selectOtherExperiment(Interaction *intended);

//    void anticipateOnPhenomenas(std::vector<Anticipation *> &anticipations);
  private:
    MOOD mood;
    MOOD previousPhenomenaMood;
    std::size_t selfSatisfactionCounter;
    Interaction *currentExperience;
    Interactions interactions;
    Phenomenas phenomenas;
    Interaction *enactedInteraction;
    Interaction *intendedInteraction;
    Interaction *currentEnactedInteraction;
    std::size_t sameInteraction;
    std::size_t maxInteractionBeforeBored;
    std::size_t maxInteractionCurious;
    bool usePhenomenas;
  };
}
#endif // EXISTENCEHEDONISM_HPP
