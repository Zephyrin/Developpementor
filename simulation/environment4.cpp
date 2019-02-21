#include "environment4.h"
#include <iostream>
#include <QDebug>

Environment4::Environment4()
{

}

Environment4::~Environment4()
{

}

void Environment4::init()
{
  i1 = interactions.addOrGetPrimitiveInteraction("e1r1", -1);
  i2 = interactions.addOrGetPrimitiveInteraction("e1r2", 2);
  i3 = interactions.addOrGetPrimitiveInteraction("e2r1", -1);
  i4 = interactions.addOrGetPrimitiveInteraction("e2r2", 2);
  first = second = third = i1;
}

const Interaction *Environment4::getResult(const Interaction *intended)
{
  const Interaction *enacted = i1;
  if(!intended->isPrimitive())
    {
      qDebug() << "Interaction : " << intended->toText().c_str()
               << " is not primitive  quit...";
      exit(1);
    }
  if(!(first == *intended)
     && second == *intended
     && third == *intended)
    {
      if(intended == *i2)
        enacted = i2;
      else
        enacted = i4;
    }
  else if(intended == *i1 || intended == *i2)
    enacted = i1;
  else
    enacted = i3;
  first = second;
  second = third;
  third = intended;
  return enacted;
}

