#ifndef SIMULATION_H
#define SIMULATION_H

#include "environment1.h"
#include "environment4.h"

#include <agent.hpp>
#include <environment.hpp>
#include <stooge.hpp>

#include <trace.hpp>
#include <ktbs.h>

#include <QObject>
#include <QString>
#include <QMutex>
#include <QWaitCondition>
#include <QFile>

class Simulation : public QObject
{
  Q_OBJECT
public:
  explicit Simulation(QObject *parent = 0);

  ~Simulation();

  void start(const QString &domaine,
             const int port,
             const QString &base,
             const QString &model,
             const QString &trace,
             Environment *env,
             const bool toConsole,
             const bool toFile,
             const QString &filename,
             const std::size_t speed,
             const std::size_t numberOfLoop,
             const std::size_t maxInteractionHeight,
             const std::size_t &sameInteractionBeforeBored,
             const int selectAgent,
             const bool usePhenomenas
             , const std::size_t excitementThreshold);

  void start(Environment *env,
             const bool toConsole,
             const bool toFile,
             const QString &filename,
             const std::size_t speed,
             const std::size_t numberOfLoop,
             const std::size_t maxInteractionHeight,
             const std::size_t &sameInteractionBeforeBored,
             const int selectAgent,
             const bool usePhenomenas
             , const std::size_t excitementThreshold);

  void forceCloseKtbs();

  void resume();

  void paused();

  void stop();

  void setSpeed(std::size_t speed);

  bool isRunning() const;

  void step();

  void getInteractions();

  const Phenomenas *getPhenomenas() const;

  void setEmitPhenomena(const bool automatique);

signals:
  void simulationDone();

  void simulationPause();

  void toConsoleSignal(const QString &text);

  void failedToOpenFile(const QString &filename);

  void startSimulationSignal();

  void logSignal(const QString &message);

  void intendedInteraction(const Interaction *interaction);

  void enactedInteraction(const Interaction *interaction);

  void showInteractions(const std::vector<const Interaction *> vector);

  void intendedEnactedSignal(const Interaction *, const Interaction *
                             , const std::vector<const UsageTable *> *
                             , const int
                             , const int);

  void showPhenomenas(const Phenomenas *);

  void initPhenomenas(const Phenomenas *);

public slots:

protected:
  void init(Environment *env,
            const bool toConsole,
            const bool toFile,
            const QString &filename,
            const std::size_t speed,
            const std::size_t numberOfLoop,
            const std::size_t maxInteractionHeight,
            const std::size_t &sameInteractionBeforeBored
            , const int selectAgent,
            const bool usePhenomenas, const std::size_t excitementThreshold);

  void clear();

private slots:
  void startSimulation();
  void simulationDoneSlot();

  void logSlot(const QString &message);

private:
  Stooge stooge;
  Environment *env;
  RadicalInteractionism::Agent *agent;
  KTBS *ktbs;
  std::string simulationId;
  QMutex sync;
  QWaitCondition pauseCond;
  bool pause;
  bool run;
  bool toConsole;
  bool toFile;
  bool emitPhenomenas;
  QFile file;
  std::size_t speed;
  std::size_t numberOfLoop;

};

#endif // SIMULATION_H
