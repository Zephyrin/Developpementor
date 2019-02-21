#include "environmentstring2.h"
#include <QSettings>
#include <QDebug>

EnvironmentString2::EnvironmentString2(const std::vector<int> board)
:
  EnvironmentString(board)
{

}

EnvironmentString2::EnvironmentString2(const QString &filename)
  :
    EnvironmentString(filename, false)
{
}


EnvironmentString2::~EnvironmentString2()
{
}

void EnvironmentString2::init()
{
  EnvironmentString::init();
  QSettings settings(filename, QSettings::IniFormat);
  settings.beginGroup("String2");
  feelEq = interactions.addOrGetPrimitiveInteraction(
        settings.value("feelEq", "feelEq").toString().toStdString(),
        settings.value("motivationFeelEq", -1).toInt());
  swapEq = interactions.addOrGetPrimitiveInteraction(
        settings.value("swapEq", "swapEq").toString().toStdString(),
        settings.value("motivationSwapEq", -5).toInt()
        );
  stepEq = interactions.addOrGetPrimitiveInteraction(
        settings.value("stepEq", "stepEq").toString().toStdString(),
        settings.value("motivationStepEq", 1).toInt()
        );
}

const Interaction *EnvironmentString2::getResult(const Interaction *intended)
{
  const Interaction *enacted = NULL;
  if(intended == *stepUp || intended == *stepDown || intended == *stepEq)
    {
      enacted = stepDown;
      if (agentPosition < board.size() - 1)
        {
          if(board.at(agentPosition) < board.at(agentPosition + 1))
            enacted = stepUp;
          else if(board.at(agentPosition) == board.at(agentPosition + 1))
            enacted = stepEq;
          agentPosition ++;
        }
      else
        agentPosition = 0;
      emit stepSignal(agentPosition, enacted);
    }
  else if(intended == *feelUp || intended == *feelDown || intended == *feelEq)
    {
      enacted = feelDown;
      if(agentPosition < board.size() - 1)
        {
         if(board.at(agentPosition) < board.at(agentPosition + 1))
           {
             enacted = feelUp;
           }
         else if(board.at(agentPosition) == board.at(agentPosition + 1))
           enacted = feelEq;
        }
      emit feelSignal(agentPosition + 1, enacted);
    }
  else if(intended == *swapUp || intended == *swapDown || intended == *swapEq)
    {
      enacted = swapDown;
      if(agentPosition < board.size() - 1)
        {
          int temp = board.at(agentPosition);
          board.at(agentPosition) = board.at(agentPosition + 1);
          board.at(agentPosition + 1 ) = temp;
          if(board.at(agentPosition) < board.at(agentPosition + 1))
            enacted = swapUp;
          else if(board.at(agentPosition) == board.at(agentPosition + 1))
            enacted = swapEq;
          emit swapSignal(agentPosition, agentPosition + 1, enacted);
        }
      else
        emit swapSignal(agentPosition, agentPosition, enacted);
    }
  else
    {
      qDebug() << "No experience find for this experience : "
               << intended->toText().c_str() << " quit...";
      exit(0);
    }
  return enacted;
}

