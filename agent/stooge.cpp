#include "stooge.hpp"
#include "hedonism.hpp"
#include "phenomenal.hpp"
#include "minimumalgorithm.hpp"
Stooge::Stooge()
{

}

Stooge::~Stooge()
{

}

RadicalInteractionism::Agent
*Stooge::createAgent(AGENT type,
                     const Interactions &interactions,
                     const std::size_t &maxInteractionHeight,
                     const std::size_t &sameInteractionBeforeBored,
                     const std::size_t &usePhenomena,
                     const std::size_t &excitementThreshold)
{
  RadicalInteractionism::Agent *agent = NULL;
  switch(type)
    {
    case AGENT::HEDONISM:
      agent = new Hedonism::Agent(interactions,
                                  maxInteractionHeight,
                                  sameInteractionBeforeBored,
                                  usePhenomena);
      break;
    case AGENT::PHENOMENA:
      agent = new Phenomenal::Agent(interactions,
                                    maxInteractionHeight);
      break;
    case AGENT::MINIMUMALGORITHM:
      agent = new MinimumAlgorithm::Agent(interactions,
                                          maxInteractionHeight,
                                          excitementThreshold);
    }
  return agent;
}

