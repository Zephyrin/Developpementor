#include "anticipates.hpp"
#include "trace.hpp"
#include <algorithm> // std::sort

Anticipates::Anticipates()
{
}


void Anticipates::addAnticipation(Interaction *interaction)
{
  std::vector<Anticipation *>::iterator it;
  it = std::find(anticipations.begin(), anticipations.end(),
                 *(activated->getPostInteraction()));
  if(it == anticipations.end())
    anticipations.push_back(
          new Anticipation(
            interaction->getPostInteraction(),
            interaction->getWeight()
            * interaction->getPostInteraction()->getValence()));
  else
    (*it)->addProclivity(interaction->getWeight()
                         * interaction->getPostInteraction()
                         ->getValence());
}

bool compProclivity(const Anticipation *a, const Anticipation *b)
{
  return a->getProclivity() > b->getProclivity();
}

Interaction *Anticipates::getIntended()
{
  std::sort(anticipates.begin(), anticipates.end(), compProclivity);
  std::vector<Anticipation *>::iterator it;
  for(it = anticipates.begin(); it != anticipates.end(); it++)
  {
     Trace::addProposed(*it);
  }
  return anticipates.front()->getInteraction();
}
