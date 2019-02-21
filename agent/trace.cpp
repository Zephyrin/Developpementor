#include "trace.hpp"

#include <sstream>

std::vector<const Interaction *> Trace::newInteractions;
std::vector<const Interaction *> Trace::activated;
std::vector<Anticipation *> Trace::proposed;
std::vector<const Interaction *> Trace::reinforce;
std::vector<const UsageTable *> Trace::phenomenas;
std::vector<const UsageTable *> Trace::phenomenasPrint;
const Interaction * Trace::intended;
const Interaction * Trace::enacted;
std::vector<const Interaction *> Trace::learned;
std::size_t Trace::timeStep = 0;
std::size_t Trace::timeSave = 0;
MOOD Trace::mood = MOOD::SELF_SATISFIED;
bool Trace::run = false;
bool Trace::cActivated = true;
bool Trace::cIntended = true;
bool Trace::cProposed = true;
bool Trace::cEnacted = true;
bool Trace::cLearned = true;
bool Trace::cReinforced = true;
bool Trace::cPhenomena = true;

void Trace::finish()
{
  activated.clear();
  newInteractions.clear();
  for(Anticipation *anticipation : proposed)
    delete anticipation;
  proposed.clear();
  reinforce.clear();
  intended = NULL;
  enacted = NULL;
  learned.clear();
  phenomenas.clear();
  phenomenasPrint.clear();
  timeSave = 0;
  timeStep = 0;
  run = false;
}

void Trace::startNewEvent()
{
  activated.clear();
  newInteractions.clear();
  for(Anticipation *anticipation : proposed)
    delete anticipation;
  reinforce.clear();
  proposed.clear();
  intended = NULL;
  enacted = NULL;
  learned.clear();
  phenomenas.clear();
  phenomenasPrint.clear();
  timeSave ++;
  timeStep ++;
  run = true;
}

void Trace::closeEvent()
{
  run = false;
}

bool Trace::isClose()
{
  return !run;
}

void Trace::addNewInteraction(const Interaction *interaction)
{
  newInteractions.push_back(interaction);
}

void Trace::addActivated(const Interaction *interaction)
{
  activated.push_back(interaction);
}

void Trace::addProposed(const Anticipation *anticipation)
{
  proposed.push_back(new Anticipation(*anticipation));
}

void Trace::addReinforce(const Interaction *interaction)
{
  reinforce.push_back(interaction);
}

void Trace::setIntended(const Interaction *interaction)
{
  intended = interaction;
}

void Trace::setEnacted(const Interaction *interaction)
{
  enacted = interaction;
}

void Trace::addLearned(const Interaction *interaction)
{
  learned.push_back(interaction);
}

const std::vector<const Interaction *> *Trace::getLearned()
{
  return &learned;
}

void Trace::addPhenomena(const UsageTable *phenomena)
{
  phenomenas.push_back(phenomena);
  phenomenasPrint.push_back(phenomena);
}

void Trace::setMood(MOOD m)
{
  mood = m;
}

std::string Trace::stepToKtbs(const std::string &id)
{
  std::stringstream os;
  os << "<" << timeStep << "_" << id << "> a m:Step;" << std::endl
     << ":hasTrace <.> ;" << std::endl
     << "s:label \"" << timeStep << "\";" << std::endl
     << ":hasBegin " << getTimeSave() << ";" << std::endl
     << "s:mood \"" << mood << "\"." << std::endl;
  return os.str();
}

std::vector<std::string> Trace::newInteractionsToKtbs(const std::string &id)
{

  std::vector<std::string> interactions;
  if(!newInteractions.empty())
    {
      for(const Interaction *interaction : newInteractions)
        {
          std::stringstream os;
          os << interaction->typeToKtbs(id);
          os << ":hasBegin " << getTimeSave() << " ;" << std::endl
             << "m:learnOnStep <" << timeStep << "_" << id << "> ."
             << std::endl;
          interactions.push_back(os.str());
        }
    }
  return interactions;
}

std::vector<std::string> Trace::activatedToKtbs(const std::string &id)
{
  std::vector<std::string> vector;
  if(!activated.empty())
    {
      for(const Interaction *interaction : activated)
        {
          vector.push_back(interaction->toKtbs(id, "m:Activated", timeStep));
        }
    }
  return vector;
}

std::vector<std::string> Trace::proposedToKtbs(const std::string &id)
{
  std::vector<std::string> vector;
  if(!proposed.empty())
    {
      std::stringstream osNumber;
      osNumber << timeStep;
      for(const Anticipation *anticipation : proposed)
        {
          std::stringstream os;
          anticipation->toKtbs(os, osNumber.str(), id);
          os << "m:onStep <" << timeStep << "_" << id << "> ." << std::endl;
          vector.push_back(os.str());
        }
    }
  return vector;
}

std::string Trace::intendedToKtbs(const std::string &id)
{
  std::stringstream os;
  if(intended != NULL)
    {
      os << intended->toKtbs(id, "m:Intended", timeStep);
    }
  return os.str();
}

std::string Trace::enactedToKtbs(const std::string &id)
{
  std::stringstream os;
  if(enacted != NULL)
    {
      os << enacted->toKtbs(id, "m:Enacted", timeStep);
    }
  return os.str();
}

std::vector<std::string> Trace::learnedToKtbs(const std::string &id)
{
  std::vector<std::string> vector;

  if(!activated.empty())
    {
      std::stringstream os;
//      ktbsPrintInteractions(os, learned, id, "l", "m:Learned");
      vector.push_back(os.str());
    }
  return vector;
}

std::vector<std::string> Trace::phenomenasToKtbs(const std::string &id)
{
  std::vector<std::string> vector;
  if(!phenomenas.empty())
    {
      std::stringstream osNumber;
      osNumber << timeStep;
      for(const UsageTable *phenomena : phenomenas)
        {
          vector.push_back(phenomena->toKtbs(id, osNumber.str()));
        }
    }
  return vector;
}

std::vector<const UsageTable *> *Trace::getPhenomenas()
{
  return &phenomenas;
}

std::ostream& Trace::getForKtbs(std::ostream &os)
{
//  if(!activated.empty())
//    ktbsPrintInteractions(os, activated);
//  if(!reinforce.empty())
//    ktbsPrintInteractions(os, reinforce);
//  if(!learned.empty())
//    ktbsPrintInteractions(os, learned);
//  if(intended != NULL)
//    {
//      os << "{" << std::endl;
//      Trace::getContext(os);
//      intended->toKtbs(os);
//      os << "}" << std::endl;
//    }
//  if(enacted != NULL)
//    {
//      os << "{" << std::endl;
//      Trace::getContext(os);
//      enacted->toKtbs(os);
//      os << "}" << std::endl;
//    }
//  if(!proposed.empty())
//    {
//      os << "{" << std::endl;
//      Trace::getContext(os);
//      os << "\"@id\": " << "\"./\"," << std::endl
//         << "\"hasObselList\": {\"@id\", \"@type\": \"StoredTraceObsels\" },"
//         << std::endl
//         << "\"obsels\": [" << std::endl;
//      for(const Anticipation *anticipation : proposed)
//        {

//          os << "{" << std::endl;
//          anticipation->toKtbs(os);
//          os << "}," << std::endl;
//        }
//      os << "]}";
//    }
  return os;
}

std::string Trace::getForPrint()
{
  std::stringstream os;
  os << getForPrintMecaEnacted();
  if(cPhenomena)
    {
      os << "------------------------------------------------------------------"
            "----"
         << std::endl << "Phenomenas : " << std::endl;
      for(const UsageTable *phenomena : phenomenas)
        os << "\t" << phenomena->toText() << std::endl;
    }
  os << getForPrintMecaEnacted();
  return os.str();
}

std::string Trace::getForPrintMecaIntended()
{
  std::stringstream os;
  os << std::endl
     << "######################################################################"
     << std::endl << "Step : " << timeStep << ", mood : " << mood << std::endl;
  if(cActivated)
    {
      os << "------------------------------------------------------------------"
            "----"
         << std::endl << "Activated : " << std::endl ;
      for(const Interaction *interaction : activated)
        {
          os << '\t' << interaction->toText() << std::endl;
        }
    }
  if(cProposed)
    {
      os << "------------------------------------------------------------------"
            "----"
         << std::endl << "Proposed : " << std::endl;
      for(const Anticipation *anticipation : proposed)
        os << "\t" << anticipation->toText() << std::endl;
    }
  if(cIntended)
    {
      os << "------------------------------------------------------------------"
            "----"
         << std::endl << "Intended : " << std::endl
         << "\t" << intended->toText() << std::endl;
    }
  if(cPhenomena)
    {
      os << "------------------------------------------------------------------"
            "----"
         << std::endl << "Phenomenas : " << std::endl;
    }
  return os.str();
}

std::string Trace::getForPrintPhenomenas()
{
  std::stringstream os;
  if(cPhenomena)
    {
      for(const UsageTable *phenomena : phenomenasPrint)
        os << phenomena->toText() << std::endl;
    }
  phenomenasPrint.clear();
  return os.str();
}

std::string Trace::getForPrintMecaEnacted()
{
  std::stringstream os;
  if(cEnacted)
    {
      if(enacted != NULL)
        os << "----------------------------------------------------------------"
              "------"
           << std::endl << "Enacted : " << std::endl
           << "\t" << enacted->toText() << std::endl;
      else
        os << "\t" << "NULL" << std::endl;
    }
  if(cLearned)
    {
      os << "------------------------------------------------------------------"
            "----"
         << std::endl << "Learned : " << std::endl;
      for(const Interaction *interaction : learned)
        os << "\t" << interaction->toText() << std::endl;
    }
  if(cReinforced)
    {
      os << "------------------------------------------------------------------"
            "----"
         << std::endl << "Reinforced : " << std::endl;
      for(const Interaction *interaction : reinforce)
        os << "\t" << interaction->toText() << std::endl;
    }
  os << std::endl << "Step : " << timeStep << ", mood : " << mood << std::endl;
  return os.str();
}

std::size_t Trace::getTimeSave()
{
  return timeSave ++;
}
bool Trace::getCPhenomena()
{
  return cPhenomena;
}

void Trace::setCPhenomenas(bool value)
{
  cPhenomena = value;
}

std::size_t Trace::getStep()
{
  return timeStep;
}

bool Trace::getCReinforced()
{
  return cReinforced;
}

void Trace::setCReinforced(bool value)
{
  cReinforced = value;
}

bool Trace::getCLearned()
{
    return cLearned;
}

void Trace::setCLearned(bool value)
{
    cLearned = value;
}

bool Trace::getCEnacted()
{
    return cEnacted;
}

void Trace::setCEnacted(bool value)
{
    cEnacted = value;
}

bool Trace::getCProposed()
{
    return cProposed;
}

void Trace::setCProposed(bool value)
{
    cProposed = value;
}

bool Trace::getCIntended()
{
    return cIntended;
}

void Trace::setCIntended(bool value)
{
    cIntended = value;
}

bool Trace::getCActivated()
{
    return cActivated;
}

void Trace::setCActivated(bool value)
{
    cActivated = value;
}


