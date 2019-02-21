#ifndef OBSEL_H
#define OBSEL_H
#include <string>

enum class OBSEL
{
  INTERACTION
  , ANTICIPATION
  , EXSISTENCE
  , INTERACTIONS
  , MOOD
  , PHENOMENAS
  , SIGNATURE
  , SIGNATURES
  , USAGETABLE
};

class Obsel
{
public:
  Obsel();
  OBSEL type() const;
  std::string modification() const;
  std::string toolTip() const;
  std::string toKtbs(const std::string &id) const;
  friend std::ostream & operator << (std::ostream &os
                                     , const Obsel &obsel);
  friend std::istream & operator >> (std::istream &is
                                     , const Obsel &obsel);
protected:
  OBSEL type_;
  std::string modification_;
  std::string toolTip_;
};

#endif // OBSEL_H
