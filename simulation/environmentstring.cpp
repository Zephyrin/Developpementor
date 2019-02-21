#include "environmentstring.h"
#include <QSettings>
#include <QTextStream>

#include <QDebug>
#include <iostream>

EnvironmentString::EnvironmentString(const std::vector<int> board, const bool equalAreDown)
  :
    board(board)
  , equalAreDown(equalAreDown)
{

}

EnvironmentString::EnvironmentString(const QString &filename
                                     , const bool equalAreDown)
  :
    filename(filename)
  , equalAreDown(equalAreDown)
{
}

EnvironmentString::~EnvironmentString()
{

}

const std::vector<int> &EnvironmentString::getBoard() const
{
  return board;
}

void EnvironmentString::init()
{
  QSettings settings(filename, QSettings::IniFormat);
  settings.beginGroup("String");
  feelUp = interactions.addOrGetPrimitiveInteraction(
        settings.value("feelUp", "feelUp").toString().toStdString(),
        settings.value("motivationFeelUp", -1).toInt());
  swapUp = interactions.addOrGetPrimitiveInteraction(
        settings.value("swapUp", "swapUp").toString().toStdString(),
        settings.value("motivationSwapUp", -1).toInt());
  stepUp = interactions.addOrGetPrimitiveInteraction(
        settings.value("stepUp", "stepUp").toString().toStdString(),
        settings.value("motivationStepUp", 10).toInt());

  feelDown = interactions.addOrGetPrimitiveInteraction(
        settings.value("feelDown", "feelDown").toString().toStdString(),
        settings.value("motivationFeelDown", -5).toInt());
  swapDown = interactions.addOrGetPrimitiveInteraction(
        settings.value("swapDown", "swapDown").toString().toStdString(),
        settings.value("motivationSwapDown", -5).toInt());
  stepDown = interactions.addOrGetPrimitiveInteraction(
        settings.value("stepDown", "stepDown").toString().toStdString(),
        settings.value("motivationStepDown", -10).toInt());

  agentPosition = settings.value("agentPosition", 0).toInt();
  QString board(settings.value("board",
                               "9 0 8 8 7 7 5 6 6 5 4 3 3 2 2 1 1 5 9")
                .toString());
  settings.endGroup();
  QTextStream text(&board);
  while(!text.atEnd())
    {
      int i;
      text >> i;
      this->board.push_back(i);
    }
  if(board.size() > 0)
    emit initFinished();
  else
    wellForm = false;
}

const Interaction *EnvironmentString::getResult(const Interaction *intended)
{
  if(equalAreDown)
    return getResultEqualAreDown(intended);

  return getResultEqualAreUp(intended);
}

std::size_t EnvironmentString::getAgentPosition() const
{
  return agentPosition;
}

const Interaction *EnvironmentString::getResultEqualAreUp(
    const Interaction *intended)
{
  const Interaction *enacted = NULL;
  if(intended == *stepUp || intended == *stepDown)
    {
      enacted = stepUp;
      if (agentPosition < board.size() - 1)
        {
          if(board.at(agentPosition) > board.at(agentPosition + 1))
            enacted = stepDown;
          agentPosition ++;
        }
      else
        agentPosition = 0;
      emit stepSignal(agentPosition, enacted);
    }
  else if(intended == *feelUp || intended == *feelDown)
    {
      enacted = feelUp;
      if(agentPosition < board.size() - 1
         && board.at(agentPosition) > board.at(agentPosition + 1))
        {
          enacted = feelDown;
        }
      emit feelSignal(agentPosition + 1, enacted);
    }
  else if(intended == *swapUp || intended == *swapDown)
    {
      enacted = swapUp;
      if(agentPosition < board.size() - 1)
        {
          int temp = board.at(agentPosition);
          board.at(agentPosition) = board.at(agentPosition + 1);
          board.at(agentPosition + 1 ) = temp;
          if(board.at(agentPosition) > board.at(agentPosition + 1))
            enacted = swapDown;
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

const Interaction *EnvironmentString::getResultEqualAreDown(
    const Interaction *intended)
{
  const Interaction *enacted = NULL;
  if(intended == *stepUp || intended == *stepDown)
    {
      enacted = stepUp;
      if (agentPosition < board.size() - 1)
        {
          if(board.at(agentPosition) >= board.at(agentPosition + 1))
            enacted = stepDown;
          agentPosition ++;
        }
      else
        agentPosition = 0;
      emit stepSignal(agentPosition, enacted);
    }
  else if(intended == *feelUp || intended == *feelDown)
    {
      enacted = feelUp;
      if(agentPosition < board.size() - 1
         && board.at(agentPosition) >= board.at(agentPosition + 1))
        {
          enacted = feelDown   ;
        }
      emit feelSignal(agentPosition + 1, enacted);
    }
  else if(intended == *swapUp || intended == *swapDown)
    {
      enacted = swapUp;
      if(agentPosition < board.size() - 1)
        {
          int temp = board.at(agentPosition);
          board.at(agentPosition) = board.at(agentPosition + 1);
          board.at(agentPosition + 1 ) = temp;
          if(board.at(agentPosition) >= board.at(agentPosition + 1))
            enacted = swapDown;
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

void EnvironmentString::resetEnv()
{
  QSettings settings(filename, QSettings::IniFormat);
  settings.beginGroup("String");
  QString board(settings.value("board",
                               "9 0 8 8 7 7 5 6 6 5 4 3 3 2 2 1 1 5 9")
                .toString());
  settings.endGroup();
  QTextStream text(&board);
  this->board.clear();
  while(!text.atEnd())
    {
      int i;
      text >> i;
      this->board.push_back(i);
    }
  if(board.size() == 0)
    wellForm = false;
}
