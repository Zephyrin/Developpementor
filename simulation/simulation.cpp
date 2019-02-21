#include "simulation.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QThread>

#include <iostream>
#include <sstream>

Simulation::Simulation(QObject *parent)
  :
    QObject(parent),
    stooge(),
    env(NULL),
    agent(NULL),
    ktbs(NULL),
    simulationId("0"),
    sync(),
    pauseCond(),
    pause(false),
    run(false),
    toConsole(true),
    toFile(false),
    emitPhenomenas(false),
    file(),
    speed(20),
    numberOfLoop(100)
{
  connect(this, SIGNAL(startSimulationSignal()), this, SLOT(startSimulation()));
}

Simulation::~Simulation()
{
  if(agent != NULL)
    delete agent;
  if(ktbs != NULL)
    delete ktbs;
  if(env != NULL)
    delete env;
}

void Simulation::start(const QString &domaine,
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
                       , const std::size_t excitementThreshold)
{
  init(env, toConsole, toFile, filename, speed, numberOfLoop,
       maxInteractionHeight, sameInteractionBeforeBored,
       selectAgent, usePhenomenas, excitementThreshold);
  if(run)
    {
      ktbs = new KTBS(domaine, port, base, model, trace);
      connect(ktbs, SIGNAL(initDone()), this, SLOT(startSimulation()));
      connect(ktbs, SIGNAL(logSignal(const QString &)), this,
              SLOT(logSlot(const QString &)));
      ktbs->init();
      emit startSimulationSignal();
    }
}

void Simulation::start(Environment *env,
                       const bool toConsole,
                       const bool toFile,
                       const QString &filename,
                       const std::size_t speed,
                       const std::size_t numberOfLoop,
                       const std::size_t maxInteractionHeight,
                       const std::size_t &sameInteractionBeforeBored,
                       const int selectAgent,
                       const bool usePhenomenas
                       , const std::size_t excitementThreshold)
{
  init(env, toConsole, toFile, filename, speed, numberOfLoop,
       maxInteractionHeight,
       sameInteractionBeforeBored,
       selectAgent,
       usePhenomenas
       , excitementThreshold);
  if(run)
    emit startSimulationSignal();
}

void Simulation::forceCloseKtbs()
{
  if(run)
    {
      stop();
    }
  else
    simulationDoneSlot();
}

void Simulation::init(Environment *env,
                      const bool toConsole,
                      const bool toFile,
                      const QString &filename,
                      const std::size_t speed,
                      const std::size_t numberOfLoop,
                      const std::size_t maxInteractionHeight,
                      const std::size_t &sameInteractionBeforeBored,
                      const int selectAgent,
                      const bool usePhenomenas
                      , const std::size_t excitementThreshold)
{
  clear();
  this->env = env;
  this->env->init();
  if(this->env->getWellForm())
    {
      AGENT type = AGENT::HEDONISM;
      if(selectAgent == 1)
        type = AGENT::MINIMUMALGORITHM;
      else if(selectAgent == 2)
        type = AGENT::PHENOMENA;
      agent = stooge.createAgent(
            type,
            env->getInteractions(),
            maxInteractionHeight,
            sameInteractionBeforeBored,
            usePhenomenas
            , excitementThreshold);
      simulationId = QDateTime::currentDateTime()
          .toString("yyyy-MM-ddT-HH-mm-sst").toStdString();
      run = true;
      pause = false;
      this->toConsole = toConsole;
      this->toFile = toFile;
      if(toFile)
        {
          this->file.setFileName(filename);
          if(!this->file.open(QIODevice::WriteOnly))
            {
              this->toFile = false;
              emit failedToOpenFile(filename);
              run = false;
            }
        }
      this->speed = speed;
      this->numberOfLoop = numberOfLoop;
      if(emitPhenomenas)
        emit showPhenomenas(agent->getPhenomenas());
      emit initPhenomenas(agent->getPhenomenas());
    }
  else
    {
      run = false;
      emit logSignal("Environment not well form.");
    }
}

void Simulation::clear()
{
  if(agent != NULL)
    {
      delete agent;
      agent = NULL;
    }
  if(ktbs != NULL)
    {
      delete ktbs;
      ktbs = NULL;
    }

  if(env != NULL)
    {
      delete env;
      env = NULL;
    }
  pause = false;
  run = false;
}

void Simulation::resume()
{
  sync.lock();
  pause = false;
  sync.unlock();
  pauseCond.wakeAll();
}

void Simulation::paused()
{
  sync.lock();
  pause = true;
  sync.unlock();
}

void Simulation::stop()
{
  sync.lock();
  run = false;
  pause = false;
  sync.unlock();
  pauseCond.wakeAll();
}

void Simulation::step()
{
  sync.lock();
  pause = true;
  sync.unlock();
  pauseCond.wakeAll();
}

void Simulation::setEmitPhenomena(const bool automatique)
{
  bool inRun = false;
  sync.lock();
  emitPhenomenas = automatique;
  inRun = run && pause;
  sync.unlock();
  if(inRun)
    pauseCond.wakeAll();
}

void Simulation::getInteractions()
{
  if(agent != NULL)
    emit showInteractions(agent->getInteractions());
  else
    emit showInteractions(std::vector<const Interaction *>());
}

const Phenomenas *Simulation::getPhenomenas() const
{
  if(agent != NULL)
    return agent->getPhenomenas();
  return NULL;
}

void Simulation::setSpeed(std::size_t speed)
{
  sync.lock();
  this->speed = speed;
  sync.unlock();
}

bool Simulation::isRunning() const
{
  return run;
}

void Simulation::startSimulation()
{
  if(run)
    {
      if(ktbs != NULL && ktbs->canSent())
        {
          emit logSignal("Simulation start with Ktbs.");
          ktbs->postToTrace(env->toKtbs(simulationId));
          ktbs->postToTrace(Trace::newInteractionsToKtbs(simulationId));
          QCoreApplication::processEvents();
        }
      else
        emit logSignal("Simulation start.");
    }
  std::size_t i = 0;
  bool lastTraceStatus = false;
  while(run)
    {
      sync.lock();
      if(i >= numberOfLoop && numberOfLoop != 0)
        {
          i = 0;
          emit simulationPause();
          pauseCond.wait(&sync);
        }
      if(pause)
        pauseCond.wait(&sync);
      const Interaction *enacted = env->getResult(agent->getExperience());
      agent->resultExperience(enacted);
      sync.unlock();
//      if(emitPhenomenas)
//        emit showPhenomenas(agent->getPhenomenas());

      if(lastTraceStatus == false)
        {
          lastTraceStatus = true;
          if(toConsole)
            {
              emit toConsoleSignal(Trace::getForPrintMecaIntended().c_str());
            }
          emit intendedInteraction(agent->getIntendedInteraction());
        }

      if(toConsole)
        {
          std::string str = Trace::getForPrintPhenomenas();
          if(!str.empty())
            emit toConsoleSignal(str.c_str());
        }
      if(Trace::isClose())
        {
          lastTraceStatus = false;
          emit enactedInteraction(agent->getEnactedInteraction());
          emit intendedEnactedSignal(agent->getIntendedInteraction(),
                                     agent->getEnactedInteraction(),
                                     agent->getModifiedNoema()
                                     , (int)agent->getMood()
                                     , Trace::getStep()
                                     );
          if(numberOfLoop != 0)
            i ++;
          if(ktbs != NULL && ktbs->canSent())
            {
              ktbs->postToTrace(Trace::stepToKtbs(simulationId));
              ktbs->postToTrace(Trace::newInteractionsToKtbs(simulationId));
              ktbs->postToTrace(Trace::activatedToKtbs(simulationId));
              ktbs->postToTrace(Trace::proposedToKtbs(simulationId));
              ktbs->postToTrace(Trace::intendedToKtbs(simulationId));
              ktbs->postToTrace(Trace::enactedToKtbs(simulationId));
              //          ktbs->postToTrace(Trace::learnedToKtbs(simulationId));
              QCoreApplication::processEvents();
            }
          if(toConsole)
            {
              emit toConsoleSignal(Trace::getForPrintMecaEnacted().c_str());
            }
          if(toFile)
            {
              file.write(Trace::getForPrint().c_str());
              file.write("\n\n");
            }
        }
      if(speed != 0)
        QThread::usleep((unsigned long)speed);

    }
  Trace::finish();
  run = false;
  if(ktbs != NULL && ktbs->canSent())
    connect(ktbs, SIGNAL(allRecieved()), this, SLOT(simulationDoneSlot()));
  else
    {
      simulationDoneSlot();
    }
}

void Simulation::simulationDoneSlot()
{
  emit logSignal("Simulation done.");
  emit simulationDone();
}

void Simulation::logSlot(const QString &message)
{
  emit logSignal(message);
}

