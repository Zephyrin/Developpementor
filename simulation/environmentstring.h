#ifndef ENVIRONMENTSTRING_H
#define ENVIRONMENTSTRING_H

#include <environment.hpp>
#include <QObject>

class EnvironmentString : public QObject, public Environment
{
  Q_OBJECT
  // Environment interface
public:
  EnvironmentString(const std::vector<int> board
                    =
//  {2, 1, 5, 4, 1, 3, 5, 3, 1, 5, 6, 3, 5, 4, 7, 3, 5, 3, 9, 5});
  {9, 9, 8, 8, 7, 7, 5, 6, 6, 5, 4, 3, 3, 2, 2, 1, 1, 5, 9}
      , const bool equalAreDown = false);
  EnvironmentString(const QString &filename, const bool equalAreDown = false);

  ~EnvironmentString();
  const std::vector<int> &getBoard() const;

  void init();
  const Interaction *getResult(const Interaction *intended);
  std::size_t getAgentPosition() const;
protected:
  const Interaction *getResultEqualAreUp(const Interaction *intended);
  const Interaction *getResultEqualAreDown(const Interaction *intended);
signals:
  void stepSignal(const unsigned int , const Interaction *);
  void feelSignal(const unsigned int , const Interaction *);
  void swapSignal(const unsigned int ,
                  const unsigned int , const Interaction *);
  void initFinished();
  void errorInit();
public slots:
  void resetEnv();

protected:
  std::vector<int> board;
  const Interaction *stepUp;
  const Interaction *swapUp;
  const Interaction *feelUp;
  const Interaction *stepDown;
  const Interaction *swapDown;
  const Interaction *feelDown;
  std::size_t agentPosition;
  QString filename;
  const bool equalAreDown;
};

#endif // ENVIRONMENTSTRING_H
