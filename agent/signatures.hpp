#ifndef SIGNATURES_H
#define SIGNATURES_H

#include "signature.hpp"
#include <map>
#include <string>

class Interaction;

class Signatures
{
public:
  Signatures();
  ~Signatures();

  void add(Interaction *interaction);
  std::size_t getNbEnacted(const Interaction *interaction) const;
  std::size_t getNbEnacted(const std::string &id) const;
  std::size_t getNbIntended(const Interaction *interaction) const;
  std::size_t getNbTried(const Interaction *interaction) const;

  void addNbEnacted(const Interaction *interaction);
  void addNbIntended(const Interaction *interaction);

  bool has(const Interaction *interaction) const;
  float getNextValence() const;

  float getFactor(const Interaction *interaction) const;

  bool needTest() const;

  Signatures & operator +=(const Signatures &b);

  bool isCompatibleWithInteraction(const Interaction *interaction) const;

  typedef const std::map<std::string, Signature *>::const_iterator
  signatures_iterator;

  signatures_iterator begin() const;

  signatures_iterator end() const;

  bool isAchievable(const std::string &id) const;
  bool isConfirm(const std::string &id) const;

  std::size_t nbElements() const;
private:
  std::map<std::string, Signature * > elements;
};

#endif // SIGNATURE_H
