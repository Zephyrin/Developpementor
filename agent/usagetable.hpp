#ifndef PHENOMENA_H
#define PHENOMENA_H

class Interactions;
class Interaction;
#include "signatures.hpp"
#include <map>
#include <vector>

class UsageTable
{
public:
  UsageTable(Interaction *interaction
        , bool unknown = false);
  ~UsageTable();

  Interaction *getInteraction() const;

  const Signatures &getPreInteractions() const;
  void addPreInteraction(UsageTable *preNoema);

  const Signatures &getPostInteractions() const;

  void addPostInteraction(UsageTable *postNoema);

  const Signatures &getEnacted() const;

  void addNbEnacted(Interaction *noema);

  void enacted(Interaction *intended, Interaction *enacted);

  void prevIntendedPrevEnacted(Interaction *intended,
                               Interaction *enacted);

  Interaction * getBestExperience(UsageTable *noema) const;
  std::vector<UsageTable *> getOpposites() const;
  bool isOpposite(const UsageTable *noema) const;

  std::size_t getNbEnacted() const;

  std::size_t getNbIntented() const;

  float getReward() const;

  std::size_t calcNbTried() const;

  void unpersistent();
  void setPersistent();

  const UsageTable *getBelieve() const;

  void nextIntendedNextEnacted(Interaction *intended
                               , Interaction *enacted
                               );

  void prevEnactedWithFalseIntended(Interaction *prevEnacted);

  void believeNotTrue();

  bool needTest() const;

  bool isUnknown() const;

  Interaction *getLeastExperience(UsageTable *experience) const;

  enum class STATUS
  {
    UNKNOWN,
    PERSISTENT,
    UNPERSISTENT
  };

  void setPreInteraction(Interaction *preInteraction);

  void setPostInteraction(Interaction *postInteraction);

  STATUS getStatus() const;

  std::string toTextTip() const;
  std::string toText() const;
  std::string toKtbs(const std::string &id, const std::string &timeStep) const;
protected:

  void updateBelieve();

  Interaction *interaction;
  Signatures post;
  Signatures pre;
  Signatures enacteds;
  std::size_t nbEnacted;
  std::size_t nbIntended;
  bool unknown;
  float reward;
  STATUS status;
  UsageTable *believe;
};

#endif // PHENOMENA_H
