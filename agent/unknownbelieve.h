#ifndef UNKNOWNBELIEVE_H
#define UNKNOWNBELIEVE_H

#include "interaction.hpp"
#include "interactions.hpp"
#include "usagetable.hpp"
#include "anticipation.hpp"
#include <map>

class UnknownBelieve : public UsageTable
{
public:
  UnknownBelieve();
  ~UnknownBelieve();
  void addNoema(UsageTable *noema);
  bool isUnknown(Interaction *interaction) const;
  bool isUnknown(const UsageTable *noema) const;
  void prevIntendedPrevEnacted(Interaction *previousIntended
                               , Interaction *previousEnacted
                               , Interaction *enacted);

  void nextIntendedNextEnacted(Interaction *intended
                               , Interaction *enacted
                               , Interaction *previousEnacted);
  void addNbEnacted(UsageTable *noemaEnacted, Interaction *interaction);
  void prevEnactedWithFalseIntended(Interaction *previousEnacted
                                    , Interaction *intended);

  bool isEmpty() const;
  const std::map<std::string, UsageTable *> &getNoemas() const;

  UsageTable *getNoema(const Interaction *interaction) const;
  bool hasNoema(const Interaction *interaction) const;
  void getActivatedInteraction(std::vector<Anticipation *> &anticipates,
                               Interaction *enacted) const;
  bool isCompatibleWithNoema(const Interaction *interaction) const;
protected:
  float calcProclivity(const std::size_t nbEnacted
                     , const std::size_t nbIntended, const Interaction *interaction) const;
  void addAnticiptation(std::vector<Anticipation *> &anticipates
                        , Interaction *interaction
                        , const float proclivity) const;
private:
  std::map<std::string, UsageTable *> noemas;
};

#endif // UNKNOWNBELIEVE_H
