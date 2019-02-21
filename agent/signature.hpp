#ifndef SIGNATURE_H
#define SIGNATURE_H

class Interaction;
#include <cstddef>

class Signature
{
public:
  Signature(Interaction *interaction);
  Signature(const Signature &signature);
  ~Signature();

  std::size_t getNbEnacted() const;

  std::size_t getNbIntended() const;

  std::size_t getNbTried() const;

  void addNbEnacted();
  void addNbIntended();
  void addNbEnacted(const std::size_t nbEnacted);
  void addNbIntended(const std::size_t nbIntended);

  bool isAchievable() const;
  float getReward() const;
  float getFactor() const;
  float getValence() const;
  bool needTest() const;

  std::size_t getAdditionEnactedIntended() const;

  const Interaction * getInteraction() const;
  bool isConfirm() const;
private:
  Interaction *interaction;
  std::size_t nbEnacted;
  std::size_t nbIntended;
};

#endif // SIGNATURE_H
