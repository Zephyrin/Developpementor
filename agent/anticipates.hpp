#ifndef ANTICIPATES_HPP
#define ANTICIPATES_HPP

#include "interaction.hpp"
#include "anticipation.hpp"

#include <vector>

class Anticipates
{
public:
  Anticipates();

  void addAnticipation(Interaction *interaction);
  Interaction *getIntended();
private:
  std::vector<Anticipation *> anticipates;

};

#endif // ANTICIPATES_HPP
