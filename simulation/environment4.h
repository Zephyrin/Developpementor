#ifndef ENVIRONNEMENT4_H
#define ENVIRONNEMENT4_H

#include "environment.hpp"

class Environment4 : public Environment
{
public:
  Environment4();
  ~Environment4();

  // Environnement interface
public:
  void init();
  const Interaction *getResult(const Interaction *intended);
private:
  const Interaction *i1;
  const Interaction *i2;
  const Interaction *i3;
  const Interaction *i4;
  const Interaction *first;
  const Interaction *second;
  const Interaction *third;
};

#endif // ENVIRONNEMENT4_H
