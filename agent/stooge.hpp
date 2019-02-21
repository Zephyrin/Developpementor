#ifndef STOOGE_HPP
#define STOOGE_HPP

#include "agent.hpp"

#include <interactions.hpp>

enum class AGENT{
  HEDONISM,
  PHENOMENA,
  MINIMUMALGORITHM
};

class Stooge
{
public:
  Stooge();
  ~Stooge();

  RadicalInteractionism::Agent *createAgent(
      AGENT type,
      const Interactions &interactions,
      const std::size_t &maxInteractionHeight,
      const std::size_t &sameInteractionBeforeBored = 0,
      const std::size_t &usePhenomena = false
      , const std::size_t &excitementThreshold = 5);
private:

};

#endif // STOOGE_HPP
