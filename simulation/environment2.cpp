#include "environment2.hpp"
#include <QDebug>

Environment2::Environment2()
  :
    Environment(),
    alternation(true)
{

}

Environment2::~Environment2()
{

}

void Environment2::init()
{
  i1 = interactions.addOrGetPrimitiveInteraction("e1r1", -1);
  i2 = interactions.addOrGetPrimitiveInteraction("e1r2", 1);
  i3 = interactions.addOrGetPrimitiveInteraction("e2r1", -1);
  i4 = interactions.addOrGetPrimitiveInteraction("e2r2", 1);
  lastExperience = NULL;
  last2Experience = NULL;
}

const Interaction *Environment2::getResult(const Interaction *intended)
{
  const Interaction *enacted = NULL;
  if(lastExperience == NULL || last2Experience == NULL)
    {
      this->last2Experience = lastExperience;
      if(lastExperience != NULL)
        {
          if(lastExperience == *i1 && (intended == *i1 || intended == *i2))
            {
              enacted = i2;
            }
          else if(lastExperience == *i1 && (intended == *i1 || intended == *i2))
            {
              enacted = i3;
            }
          else
            {
              if(intended == *i1 || intended == *i2)
                enacted = i1;
              else
                enacted = i3;
            }
        }
      else
        {
          if(intended == *i1 || intended == *i2)
            enacted = i1;
          else
            enacted = i2;
        }
      this->lastExperience = intended;
      return enacted;
    }
  if(!intended->isPrimitive())
    {
      qDebug() << "Experience : " << intended->toText().c_str()
               << " is not primitive  quit...";
      exit(1);
    }
  if(last2Experience->getId()[1] != lastExperience->getId()[1])
    {
      if(lastExperience == *i1 && (intended == *i1 || intended == *i2))
        {
          enacted = i2;
        }
      else if(lastExperience == *i3 && (intended == *i3 || intended == *i4))
        {
          enacted = i4;
        }
      else
        {
          if(intended == *i1 || intended == *i2)
            {
              enacted = i1;
            }
          else
            enacted = i3;
        }
    }
  else
    {
      if(intended == *i1 || intended == *i2)
        {
          enacted = i1;
        }
      else
        enacted = i3;
    }
  this->last2Experience = lastExperience;
  this->lastExperience = enacted;
  return enacted;
}

