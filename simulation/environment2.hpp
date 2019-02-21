#ifndef ENVIRONMENT2_HPP
#define ENVIRONMENT2_HPP

#include "environment.hpp"

class Environment2 : public Environment
{
public:
  Environment2();
  ~Environment2();

  // Environment interface
  void init();
  const Interaction *getResult(const Interaction *intended);
private:
  const Interaction *i1;
  const Interaction *i2;
  const Interaction *i3;
  const Interaction *i4;
  const Interaction *lastExperience;
  const Interaction *last2Experience;
  bool alternation;
};

#endif // ENVIRONMENT2_HPP
