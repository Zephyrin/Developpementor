#ifndef ENVIRRONEMENT1_H
#define ENVIRRONEMENT1_H

#include "environment.hpp"

class Environment1 : public Environment
{
public:
  Environment1();
  virtual ~Environment1();

  void init();

  const Interaction *getResult(const Interaction *enacted);

private :
  Interaction *i1;
  Interaction *i2;
  Interaction *i3;
  Interaction *i4;
};

#endif // ENVIRRONEMENT1_H
