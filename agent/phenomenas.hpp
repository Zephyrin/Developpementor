#ifndef PHENOMENAS_H
#define PHENOMENAS_H

#include "usagetable.hpp"
//#include "unknownbelieve.h"
#include "interaction.hpp"
#include "interactions.hpp"
#include "anticipation.hpp"

#include <map>
#include <vector>
#include <list>

class Phenomenas
{
public:
  Phenomenas(Interactions &interactions, bool setPersistent = false);
  ~Phenomenas();

  void intendedAndEnacted(Interaction *intended, Interaction *enacted);
  const std::vector<const UsageTable *> &toListOfNoema() const;

  bool isPhenomene(Interaction *interaction) const;

  UsageTable *getPhenomena(const std::string &id) const;

  Interaction *leastTriedExperience(Interaction *enacted) const;
  Interaction *withMaxExpectedOutcomeValence(Interaction *enacted);

  void setNoemaUnpersistent(Interaction *enacted);
  void setNoemaPersistent(Interaction *enacted);

  const std::vector<const UsageTable *> *getModifiedNoema() const;

  UsageTable *getNoema(const Interaction *interaction) const;

  void addInteraction(Interaction *interaction);
protected:
  UsageTable *addOrGetPhenomena(Interaction *interaction);

  void addAnticiptation(std::vector<Anticipation *> &anticipates
                        , UsageTable *noema
                        , const Interaction *enacted
                        , const float proclivity) const;
  float calcProclivity(const std::size_t nbEnacted
                       , const std::size_t nbIntended
                       , const float valence) const;

private:
  std::map<std::string, UsageTable *> phenomenas;
  std::vector<const UsageTable *> list;
  Interaction *previousEnacted;
  Interaction *previousIntended;
  Interactions &interactions;
  std::vector<const UsageTable *> modifiedNoema;
  UsageTable *unknownInBelieveState;
};

#endif // PHENOMENAS_H
