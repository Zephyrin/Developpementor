#include "environment1.h"

Environment1::Environment1()
{

}

Environment1::~Environment1()
{

}

void Environment1::init()
{
  i1 = interactions.addOrGetPrimitiveInteraction("e1r1", -1);
  i2 = interactions.addOrGetPrimitiveInteraction("e1r2", 1);
  i3 = interactions.addOrGetPrimitiveInteraction("e2r1", -1);
  i4 = interactions.addOrGetPrimitiveInteraction("e2r2", 1);
}

const Interaction *Environment1::getResult(const Interaction *enacted)
{
  if(enacted == *i1 || enacted == *i2)
    return i1;
  return i4;
}
