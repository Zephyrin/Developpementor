#ifndef TRACE_HPP
#define TRACE_HPP

#include "interaction.hpp"
#include "usagetable.hpp"
#include "anticipation.hpp"
#include "mood.hpp"

#include <vector>

class Trace
{
public:

  static void finish();

  static void startNewEvent();

  static void closeEvent();

  static bool isClose();

  static void addNewInteraction(const Interaction *interaction);

  static void addActivated(const Interaction *interaction);

  static void addProposed(const Anticipation *anticipation);

  static void addReinforce(const Interaction *interaction);

  static void setIntended(const Interaction *interaction);

  static void setEnacted(const Interaction *interaction);

  static void addLearned(const Interaction *interaction);

  static const std::vector<const Interaction *> *getLearned();

  static void addPhenomena(const UsageTable *phenomena);

  static void setMood(MOOD mood);

  static std::string stepToKtbs(const std::string &id);

  static std::vector<std::string> newInteractionsToKtbs(const std::string &id);

  static std::vector<std::string> activatedToKtbs(const std::string &id);

  static std::vector<std::string> proposedToKtbs(const std::string &id);

  static std::string intendedToKtbs(const std::string &id);

  static std::string enactedToKtbs(const std::string &id);

  static std::vector<std::string> learnedToKtbs(const std::string &id);

  static std::vector<std::string> phenomenasToKtbs(const std::string &id);

  static std::vector<const UsageTable *> *getPhenomenas();

  static std::ostream &getForKtbs(std::ostream &os);
  static std::string getForPrint();

  static std::string getForPrintMecaIntended();
  static std::string getForPrintPhenomenas();
  static std::string getForPrintMecaEnacted();

  static bool getCActivated();
  static void setCActivated(bool value);

  static bool getCIntended();
  static void setCIntended(bool value);

  static bool getCProposed();
  static void setCProposed(bool value);

  static bool getCEnacted();
  static void setCEnacted(bool value);

  static bool getCLearned();
  static void setCLearned(bool value);

  static bool getCReinforced();
  static void setCReinforced(bool value);

  static bool getCPhenomena();
  static void setCPhenomenas(bool value);

  static std::size_t getStep();
protected:
  static std::size_t getTimeSave();

private:
  static std::vector<const Interaction *> newInteractions;
  static std::vector<const Interaction *> activated;
  static std::vector<Anticipation *> proposed;
  static std::vector<const Interaction *> reinforce;
  static std::vector<const UsageTable *> phenomenas;
  static std::vector<const UsageTable *> phenomenasPrint;
  static const Interaction * intended;
  static const Interaction * enacted;
  static std::vector<const Interaction *> learned;
  static std::size_t timeStep;
  static std::size_t timeSave;
  static MOOD mood;
  static bool run;
  static bool cActivated;
  static bool cIntended;
  static bool cProposed;
  static bool cEnacted;
  static bool cLearned;
  static bool cReinforced;
  static bool cPhenomena;
};

#endif // TRACE_H
