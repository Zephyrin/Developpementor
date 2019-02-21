#ifndef ENVIRONMENTSMALLLOOP_H
#define ENVIRONMENTSMALLLOOP_H

#include <environment.hpp>
#include <QObject>

class EnvironmentSmallLoop : public QObject, public Environment
{
  Q_OBJECT
public:
  enum LOOP{
    WALL,
    EMPTY
  };

  enum ANGLE {
    NORTH,
    SOUTH,
    EAST,
    WEST
  };

  enum DIRECTION {
    LEFT,
    RIGHT,
    BEHIND
  };

  EnvironmentSmallLoop(const QString &filename
                       = "../../IA_Dev/Embodied_Paradigm/smallLoop.env");
  ~EnvironmentSmallLoop();

  // Environment interface
  void init();
  const Interaction *getResult(const Interaction *intended);

  const std::vector<LOOP> &getLoop() const;

  std::size_t getAgentPositionX() const;

  std::size_t getAgentPositionY() const;

  std::size_t getX() const;

  std::size_t getY() const;

  ANGLE getAgentAngle() const;
public slots:
  void changeItem(const int pos);

protected:
  const Interaction *checkedWall(int fact, int &newPos);

signals:
  void logSignal(const QString &);

  void moveSignal(const unsigned int position, const int);
  void turnSignal(const int);
  void feelSignal(const int loop, const int dir);

  void initFinished();

private:
  std::vector<LOOP> loop;
  std::size_t X;
  std::size_t Y;
  const Interaction *feelBehindEmpty;
  const Interaction *feelLeftEmpty;
  const Interaction *feelRightEmpty;
  const Interaction *moveEmpty;

  const Interaction *feelBehindWall;
  const Interaction *feelLeftWall;
  const Interaction *feelRightWall;
  const Interaction *moveWall;

  const Interaction *turnLeft;
  const Interaction *turnRight;
  std::size_t agentPosition;
  std::size_t agentPositionX;
  std::size_t agentPositionY;
  ANGLE agentAngle;
  QString filename;
};

#endif // ENVIRONMENTSMALLLOOP_H
