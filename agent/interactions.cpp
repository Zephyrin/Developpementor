#include "interactions.hpp"
#include "trace.hpp"
#include "phenomenas.hpp"
#include <algorithm> // std::find std::transform std::back_inserter
#include <iostream>

Interactions::Interactions()
  :
    superInteraction(NULL),
    maxInteractionHeight(0)
  , phenomenas(NULL)
{

}

Interactions::Interactions(const Interactions &interactions,
                           const std::size_t &maxInteractionHeight
                           , Phenomenas *phenomenas)
  :
    superInteraction(NULL),
    maxInteractionHeight(maxInteractionHeight)
  , phenomenas(phenomenas)
{
  for(const pInteractions interaction : interactions)
    {
      this->interactions[interaction.second->getId()]
          = new Interaction(*(interaction.second));
      listPrimitiveInteraction.push_back(
            this->interactions[interaction.second->getId()]);
    }
}

Interactions::~Interactions()
{
  for(mInteractions::iterator it = interactions.begin();
      it != interactions.end();
      it++)
    delete it->second;
}

Interaction *Interactions::addOrGetPrimitiveInteraction(std::string id,
    const int valence)
{
  if(interactions.find(id) == interactions.end())
    {
      Interaction *interaction = new Interaction(id);
      interaction->setValence(valence);
      interactions[id] = interaction;
      return interaction;
    }
  return interactions[id];
}

Interaction *Interactions::addOrGetPrimitiveInteraction(const std::string &id)
{
  Interaction *interaction = addOrGetInteraction(id);
  return interaction;
}

Interaction *Interactions::getInteraction(const std::string &label) const
{
  return interactions.find(label) == interactions.end() ?
        NULL
      : interactions.at(label);
}

void Interactions::learnCompositeInteraction(Interaction *lastInteraction,
                                             Interaction *previousInteraction)
{
  Interaction *lastSuperInteraction = NULL;
  Interaction *previousSuperInteraction = superInteraction;
  if(previousInteraction != NULL
    && (maxInteractionHeight == 0
    || (previousInteraction->getHeight() < maxInteractionHeight
        && lastInteraction->getHeight() < maxInteractionHeight)))
    lastSuperInteraction = addOrGetAndReinforceCompositeInteraction(
          previousInteraction, lastInteraction);

  if(previousSuperInteraction != NULL
     )
    {
      if(maxInteractionHeight == 0
          || (previousSuperInteraction->getHeight() < maxInteractionHeight
              && lastSuperInteraction->getHeight() < maxInteractionHeight))
      addOrGetAndReinforceCompositeInteraction(
            previousSuperInteraction->getPreInteraction(),
            lastSuperInteraction);
      if(maxInteractionHeight == 0
           || (previousSuperInteraction->getHeight() < maxInteractionHeight
               && lastInteraction->getHeight() < maxInteractionHeight))
      addOrGetAndReinforceCompositeInteraction(previousSuperInteraction,
                                               lastInteraction);
    }
  superInteraction = lastSuperInteraction;
}

std::vector<Interaction *> Interactions::getActivatedInteractions(
    Interaction *enactedInteraction)
{
  std::vector<Interaction *> contextInteractions;
  if(enactedInteraction != NULL)
    {
      contextInteractions.push_back(enactedInteraction);
      if(!enactedInteraction->isPrimitive())
        contextInteractions.push_back(enactedInteraction->getPostInteraction());
      if(superInteraction != NULL)
        contextInteractions.push_back(superInteraction);
    }
  std::vector<Interaction *> activated;
  for(mInteractions::iterator it = interactions.begin();
      it != interactions.end();
      it++)
    {
      if(!it->second->isPrimitive())
        {
          if(std::find(contextInteractions.begin(), contextInteractions.end(),
                       *(it->second->getPreInteraction()))
             != contextInteractions.end())
            {
              activated.push_back(it->second);
              Trace::addActivated(it->second);
            }
        }
    }
  return activated;
}

Interaction *Interactions::addOrGetInteraction(const std::string &label)
{
  if(interactions.find(label) == interactions.end())
    {
      Interaction *newInteraction = new Interaction(label);
      interactions[label] = newInteraction;
    }
  return interactions[label];
}

Interaction *Interactions::addOrGetAndReinforceCompositeInteraction(
    Interaction *preInteraction, Interaction *postInteraction)
{
  Interaction *interaction = addOrGetCompositeInteraction(
        preInteraction, postInteraction);
  interaction->incWeight();
  if(interaction->getWeight() != 1)
    {
      Trace::addReinforce(interaction);
    }
  return interaction;
}

std::map<std::string, Interaction *>::const_iterator Interactions::begin() const
{
  return interactions.cbegin();
}

std::map<std::string, Interaction *>::const_iterator Interactions::end() const
{
  return interactions.end();
}

std::map<std::string, Interaction *>::iterator Interactions::begin()
{
  return interactions.begin();
}

std::map<std::string, Interaction *>::iterator Interactions::end()
{
  return interactions.end();
}

std::vector<Interaction *> Interactions::getPrimitiveInteractions()
{
  return listPrimitiveInteraction;
}

Interaction *Interactions::addOrGetCompositeInteraction(
    Interaction *preInteraction,
    Interaction *postInteraction)
{
  Interaction *interaction = getInteraction("<"
                                            + preInteraction->getId()
                                            + postInteraction->getId()
                                            + ">");
  if(interaction == NULL)
    {
      if(maxInteractionHeight == 0
         || (maxInteractionHeight > preInteraction->getHeight()
             && maxInteractionHeight > postInteraction->getHeight()))
        {
          interaction = addOrGetInteraction("<"
                                            + preInteraction->getId()
                                            + postInteraction->getId()
                                            + ">");
          interaction->setPreInteraction(preInteraction);
          interaction->setPostInteraction(postInteraction);
          interaction->setValence(preInteraction->getValence()
                                  + postInteraction->getValence());
          phenomenas->addInteraction(interaction);
        }
    }
  return interaction;
}

std::vector<const Interaction *> Interactions::toVector() const
{
  std::vector<const Interaction *> vector;
  for(std::pair<std::string, Interaction *> it : interactions)
    vector.push_back(it.second);
  return vector;
}

Interaction *Interactions::getOtherInteraction(Interaction *interaction)
{
  for(std::pair<std::string, Interaction *> it : interactions )
    {
      if(it.second != interaction)
        {
          if(interaction->getValence() < it.second->getValence())
            return it.second;
        }
    }

  for(std::pair<std::string, Interaction *> it : interactions )
    {
      if(it.second != interaction)
        {
          return it.second;
        }
    }

  return NULL;
}


