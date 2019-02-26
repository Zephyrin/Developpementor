#include "environmentsmallloop.h"
#include <QFile>
#include <QSettings>

EnvironmentSmallLoop::EnvironmentSmallLoop(const QString &filename)
  :
    filename(filename)
{

}

EnvironmentSmallLoop::~EnvironmentSmallLoop()
{

}

void EnvironmentSmallLoop::init()
{
  QSettings settings(filename, QSettings::IniFormat);
  settings.beginGroup("Loop");
  feelBehindEmpty = interactions.addOrGetPrimitiveInteraction(
        settings.value("feelBehindEmpty", "feelBehindEmpty")
        .toString().toStdString(),
        settings.value("motivationFeelBehindEmpty", -1).toInt());
  feelLeftEmpty = interactions.addOrGetPrimitiveInteraction(
        settings.value("feelLeftEmpty", "feelLeftEmpty")
        .toString().toStdString(),
        settings.value("motivationFeelLeftEmpty", -1).toInt());
  feelRightEmpty = interactions.addOrGetPrimitiveInteraction(
        settings.value("feelRightEmpty", "feelRightEmpty")
        .toString().toStdString(),
        settings.value("motivationFeelRightEmpty", -1).toInt());
  moveEmpty = interactions.addOrGetPrimitiveInteraction(
        settings.value("moveEmpty", "moveEmpty").toString().toStdString(),
        settings.value("motivationMoveEmpty", 10).toInt());

  feelBehindWall = interactions.addOrGetPrimitiveInteraction(
        settings.value("feelBehindWall", "feelBehindWall")
        .toString().toStdString(),
        settings.value("motivationFeelBehindWall", -1).toInt());
  feelLeftWall = interactions.addOrGetPrimitiveInteraction(
        settings.value("feelLeftWall", "feelLeftWall")
        .toString().toStdString(),
        settings.value("motivationFeelLeftWall", -1).toInt());
  feelRightWall = interactions.addOrGetPrimitiveInteraction(
        settings.value("feelRightWall", "feelRightWall")
        .toString().toStdString(),
        settings.value("motivationFeelRightWall", -1).toInt());
  moveWall = interactions.addOrGetPrimitiveInteraction(
        settings.value("moveWall", "moveWall").toString().toStdString(),
        settings.value("motivationMoveWall", 10).toInt());

  turnLeft = interactions.addOrGetPrimitiveInteraction(
        settings.value("turnLeft", "turnLeft").toString().toStdString(),
        settings.value("motivationTurnLeft", -3).toInt());
  turnRight = interactions.addOrGetPrimitiveInteraction(
        settings.value("turnRight", "turnRight").toString().toStdString(),
        settings.value("motivationTurnRight", -3).toInt());

  QString filenameLoop = settings.value("loopFile", ":/loop.loop").toString();
  settings.endGroup();
  QFile file(filenameLoop);
  Y = 0;
  X = 0;
  if(file.open(QIODevice::ReadOnly))
    {
      while(!file.atEnd())
        {
          QByteArray b = file.readLine();
          if(b.size() == 0)
            continue;
          if(b.at(0) != '#')
            {
              std::size_t x = b.size();
              if(X == 0)
              {
                  if(b.endsWith("\r\n"))
                    X = x - 2;
                  else
                    X = x - 1;
              }
              else if(!(b.endsWith("\r\n") && X == x - 2))
                {
                  wellForm = false;
                  return;
                }
              else if((!b.endsWith("\r\n") && X == x - 1))
              {
                  wellForm = false;
                  return;
              }
              x = 0;
              for(char a : b)
                {
                  if(a == '\r')
                    continue;
                  if(a == '\n')
                    continue;
                  if(a == '*')
                    loop.push_back(WALL);
                  else if(a == '-')
                    loop.push_back(EMPTY);
                  else
                    {
                      agentPosition = loop.size();
                      agentPositionX = x;
                      agentPositionY = Y;
                      loop.push_back(EMPTY);
                      if(a == '^')
                        {
                          agentAngle = NORTH;
                        }
                      else if(a == '>')
                        {
                          agentAngle = EAST;
                        }
                      else if(a == '<')
                        agentAngle = WEST;
                      else
                        agentAngle = SOUTH;
                    }
                  x ++;
                }
              Y ++;
            }
        }
      emit initFinished();
    }
  else
    {
      wellForm = false;
    }
}

const Interaction *EnvironmentSmallLoop::getResult(const Interaction *intended)
{
  const Interaction *enacted = nullptr;

  if(intended == *feelBehindWall || intended == *feelBehindEmpty )
    {
      enacted = feelBehindWall;
      switch(agentAngle)
        {
        case NORTH:
          if(agentPosition >= X
             && loop.at(agentPosition - X) == EMPTY)
            {
              enacted = feelBehindEmpty;
            }
          break;
        case SOUTH:
          if(agentPosition < loop.size() - X
             && loop.at(agentPosition + X) == EMPTY)
            {
              enacted = feelBehindEmpty;
            }
          break;
        case EAST:
          if(agentPosition % X < X - 1
             && agentPosition + 1 < loop.size()
             && loop.at(agentPosition + 1) == EMPTY)
            {
              enacted = feelBehindEmpty;
            }
          break;
        case WEST:
          if(agentPosition % X > 0
             && agentPosition >= 1
             && loop.at(agentPosition - 1) == EMPTY)
            {
              enacted = feelBehindEmpty;
            }
          break;
        }
      emit feelSignal(enacted == feelBehindWall ? WALL : EMPTY, BEHIND);
    }
  else if(intended == *feelLeftWall || intended == *feelLeftEmpty)
    {
      enacted = feelLeftWall;
      switch(agentAngle)
        {
        case NORTH:
          if(agentPosition % X > 0
             && agentPosition >= 1
             && loop.at(agentPosition - 1) == EMPTY)
            {
              enacted = feelLeftEmpty;
            }
          break;
        case SOUTH:
          if(agentPosition % X < X - 1
             && agentPosition < loop.size() - 1
             && loop.at(agentPosition + 1) == EMPTY)
            {
              enacted = feelLeftEmpty;
            }
          break;
        case EAST:
          if(agentPosition >= X && loop.at(agentPosition - X) == EMPTY)
            {
              enacted = feelLeftEmpty;
            }
          break;
        case WEST:
          if(agentPosition < loop.size() - X
             && loop.at(agentPosition + X) == EMPTY)
            {
              enacted = feelLeftEmpty;
            }
          break;
        }
      emit feelSignal(enacted == feelLeftWall ? WALL : EMPTY, LEFT);

    }
  else if(intended == *feelRightWall || intended == *feelRightEmpty)
    {
      enacted = feelRightWall;
      switch(agentAngle)
        {
        case NORTH:
          if(agentPosition % X < X - 1
             && agentPosition < loop.size() - 1
             && loop.at(agentPosition + 1) == EMPTY)
            {
              enacted = feelRightEmpty;
            }
          break;
        case SOUTH:
          if(agentPosition % X > 0
             && agentPosition >= 1
             && loop.at(agentPosition - 1) == EMPTY)
            {
              enacted = feelRightEmpty;
            }
          break;
        case EAST:
          if(agentPosition < loop.size() - X
             && loop.at(agentPosition + X) == EMPTY)
            {
              enacted = feelRightEmpty;
            }
          break;
        case WEST:
          if(agentPosition >= X && loop.at(agentPosition - X) == EMPTY)
            {
              enacted = feelRightEmpty;
            }
          break;
        }
      emit feelSignal(enacted == feelRightWall ? WALL : EMPTY, RIGHT);
    }
  else if(intended == *turnLeft)
    {
      enacted = turnLeft;
      switch(agentAngle)
        {
        case NORTH:
          agentAngle = WEST;
          break;
        case SOUTH:
          agentAngle = EAST;
          break;
        case EAST:
          agentAngle = NORTH;
          break;
        case WEST:
          agentAngle = SOUTH;
          break;
        }
      emit turnSignal(agentAngle);
    }
  else if(intended == *turnRight)
    {
      enacted = turnRight;
      switch(agentAngle)
        {
        case NORTH:
          agentAngle = EAST;
          break;
        case SOUTH:
          agentAngle = WEST;
          break;
        case EAST:
          agentAngle = SOUTH;
          break;
        case WEST:
          agentAngle = NORTH;
          break;
        }
      emit turnSignal(agentAngle);
    }
  else if(intended == *moveEmpty || intended == *moveWall)
    {
      enacted = moveWall;
      switch(agentAngle)
        {
        case NORTH:
          if(agentPosition >= X && loop.at(agentPosition - X) == EMPTY)
            {
              agentPosition -= X;
              enacted = moveEmpty;
            }
          break;
        case SOUTH:
          if(agentPosition < loop.size() - X
             && loop.at(agentPosition + X) == EMPTY)
            {
              agentPosition += X;
              enacted = moveEmpty;
            }
          break;
        case EAST:
          if(agentPosition % X < X - 1
             && agentPosition < loop.size() - 1
             && loop.at(agentPosition + 1) == EMPTY)
            {
              agentPosition += 1;
              enacted = moveEmpty;
            }
          break;
        case WEST:
          if(agentPosition % X > 0
             && agentPosition >= 1
             && loop.at(agentPosition - 1) == EMPTY)
            {
              agentPosition -= 1;
              enacted = moveEmpty;
            }
          break;
        }
      emit moveSignal(agentPosition, enacted == moveWall ? WALL : EMPTY);
    }
//  emit moveSignal(14, EMPTY);
//  emit turnSignal(EnvironmentSmallLoop::WEST);
//  emit moveSignal(7, EMPTY);
  return enacted;
}

const std::vector<EnvironmentSmallLoop::LOOP> &EnvironmentSmallLoop::getLoop()
const
{
  return loop;
}

std::size_t EnvironmentSmallLoop::getAgentPositionX() const
{
  return agentPositionX;
}

std::size_t EnvironmentSmallLoop::getAgentPositionY() const
{
  return agentPositionY;
}

std::size_t EnvironmentSmallLoop::getX() const
{
  return X;
}

std::size_t EnvironmentSmallLoop::getY() const
{
  return Y;
}

EnvironmentSmallLoop::ANGLE EnvironmentSmallLoop::getAgentAngle() const
{
  return agentAngle;
}

void EnvironmentSmallLoop::changeItem(const int pos)
{
  if(pos >= 0)
    loop.at(pos) = loop.at(pos) == EMPTY ? WALL : EMPTY;
}
