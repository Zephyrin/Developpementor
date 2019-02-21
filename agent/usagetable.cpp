#include "usagetable.hpp"
#include "interaction.hpp"
#include "interactions.hpp"

#include <algorithm> // std::find
#include <sstream>
#include <climits> // int_min
#include <iostream>

UsageTable::UsageTable(Interaction *interaction, bool unknown)
  :
    interaction(interaction),
    nbEnacted(0),
    nbIntended(0),
    unknown(unknown),
    reward(0),
    status(STATUS::UNKNOWN),
    believe(NULL)
{

}

UsageTable::~UsageTable()
{

}

Interaction *UsageTable::getInteraction() const
{
  return interaction;
}

const Signatures &UsageTable::getPreInteractions() const
{
  return pre;
}

void UsageTable::addPreInteraction(UsageTable *preNoema)
{
  pre.add(preNoema->getInteraction());
}

const Signatures &UsageTable::getPostInteractions() const
{
  return post;
}

void UsageTable::addPostInteraction(UsageTable *postNoema)
{
  post.add(postNoema->getInteraction());
}

const Signatures &UsageTable::getEnacted() const
{
  return enacteds;
}

void UsageTable::addNbEnacted(Interaction *noema)
{
  if(this->interaction == noema)
    nbEnacted ++;
  else
    {
      if(!enacteds.has(noema))
        enacteds.add(noema);
      enacteds.addNbEnacted(noema);
      nbIntended ++;
    }
}

void UsageTable::prevIntendedPrevEnacted(Interaction *intended,
                                    Interaction *enacted)
{
  if(intended != enacted)
    {
      pre.addNbIntended(intended);
    }
  pre.addNbEnacted(enacted);
}

Interaction *UsageTable::getBestExperience(UsageTable *experience) const
{
  std::vector<UsageTable *> vector = experience->getOpposites();
  UsageTable *best = experience;
  float proclivity = 0;
  if(!post.isConfirm(best->getInteraction()->getId()))
    best = NULL;
  else
    proclivity = post.getFactor(best->getInteraction());
  for(UsageTable *noema : vector)
    {
      if(post.isConfirm(noema->getInteraction()->getId()))
        {
          float a = post.getFactor(noema->getInteraction());
          if(best == NULL || proclivity < a)
            {
              best = noema;
              proclivity = a;
            }
        }
    }
  if(best == NULL)
    return experience->getInteraction();
  if(!post.isConfirm(best->getInteraction()->getId()))
    std::cout << "Not confirm..." << std::endl;
  return best->getInteraction();
}

Interaction *UsageTable::getLeastExperience(UsageTable *experience) const
{
  std::vector<UsageTable *> vector = experience->getOpposites();
  UsageTable *least = experience;
  float proclivity = post.getFactor(least->getInteraction());
  for(UsageTable *noema : vector)
    {
      float a = post.getFactor(noema->getInteraction());
      if(proclivity > a)
        {
          least = noema;
          proclivity = a;
        }
    }
  return least->getInteraction();
}

void UsageTable::setPreInteraction(Interaction *preInteraction)
{
  pre += preInteraction->getUsageTable()->getPreInteractions();
}

void UsageTable::setPostInteraction(Interaction *postInteraction)
{
  post += postInteraction->getUsageTable()->getPostInteractions();
}

std::vector<UsageTable *> UsageTable::getOpposites() const
{
  std::vector<UsageTable *> vector;
  for(auto it : enacteds)
    {
      Signature *signature = it.second;
      if(signature->getInteraction()
         ->getUsageTable()->enacteds.has(interaction))
        vector.push_back(signature->getInteraction()->getUsageTable());
    }
  return vector;
}

bool UsageTable::isOpposite(const UsageTable *noema) const
{
  return noema->enacteds.has(interaction)
      || enacteds.has(noema->getInteraction());
}

void UsageTable::nextIntendedNextEnacted(Interaction *intended,
                                    Interaction *enacted)
{
  if(intended != enacted)
    {
      post.addNbIntended(intended);
    }
  post.addNbEnacted(enacted);
  if(status == STATUS::PERSISTENT)
    {
      reward = interaction->getValence() + post.getNextValence() / 2.;
    }
  else
    {
      updateBelieve();
      if(believe)
        {
          reward = interaction->getValence() + believe->getReward() / 2.;
        }
      else if(interaction)
        reward = interaction->getValence();
      else
        std::cerr << "no interaction" << std::endl;
    }
}

void UsageTable::prevEnactedWithFalseIntended(Interaction *prevEnacted)
{
  pre.addNbIntended(prevEnacted);
}

void UsageTable::believeNotTrue()
{
  if(believe)
    {
      if(believe == this)
        {
          updateBelieve();
          return;
        }
      if(believe->getBelieve() != NULL)
        believe->believeNotTrue();
      else
        post.addNbIntended(believe->getInteraction());
//      post.addNbEnacted(believe->getEnacted().begin()->second->getInteraction());
    }
}

bool UsageTable::needTest() const
{
  return post.needTest();
}

bool UsageTable::isUnknown() const
{
  return unknown;
}

UsageTable::STATUS UsageTable::getStatus() const
{
  return status;
}

std::size_t UsageTable::getNbEnacted() const
{
  return nbEnacted;
}

std::size_t UsageTable::getNbIntented() const
{
  return nbIntended;
}

float UsageTable::getReward() const
{
  return reward;
}

std::size_t UsageTable::calcNbTried() const
{
  return nbEnacted + nbIntended;
}

void UsageTable::unpersistent()
{
  status = STATUS::UNPERSISTENT;
  believe = NULL;
}

void UsageTable::setPersistent()
{
  status = STATUS::PERSISTENT;
  believe = NULL;
}

const UsageTable *UsageTable::getBelieve() const
{
  return believe;
}


void UsageTable::updateBelieve()
{
  std::size_t count = 0;
//  if(!needTest())
//    {
      believe = NULL;
      std::vector<UsageTable *> believes;
      for(auto const &it : post)
        {
          const Signature *signature = it.second;
          if(signature->getInteraction()->getUsageTable()->getStatus()
             == STATUS::PERSISTENT)
            {
              if(signature->isConfirm())
                {
                  std::size_t tmp_count = 0;
                  for(auto const &itOpp
                      : signature->getInteraction()
                      ->getUsageTable()->getEnacted())
                    {
                      if(post.getNbEnacted(itOpp.first) != 0)
                        tmp_count ++;
                    }
                  if(tmp_count == 0)
                    {
                      count ++;
                      believe = signature->getInteraction()->getUsageTable();
                    }
                  believes.push_back(signature
                                     ->getInteraction()->getUsageTable());
                }
            }
        }
      if(count > 1)
        believe = NULL;
      if(believes.size() > 0)
        {
          believe = believes.front();
          for(std::size_t i = 1; i < believes.size(); i ++)
            {
              if(believes.at(i)->getReward() > believe->getReward())
                believe = believes.at(i);
            }
        }
//    }
}

std::string UsageTable::toTextTip() const
{
  std::stringstream str;
  str << interaction->getId()
      << " reward : "
      << reward
      << " valence : "
      << interaction->getValence()
      << std::endl;
  for(auto const & pair : post)
    {
      const Signature *signature = pair.second;

      if(signature->isConfirm() && signature->getInteraction() != interaction)
        {
          str << pair.first << ": "
              << signature->getNbEnacted()
              << " & "
              << signature->getNbIntended()
              << std::endl;
        }
    }
  return str.str();
}

std::string UsageTable::toText() const
{
  std::stringstream str;
  str << "*********************************************************************"
      << std::endl;
  str << "\tPersistente : "  << interaction->getId();
//  if(!preInteractions.empty())
//    {
//      str << std::endl << "\t\tPre interaction : " << std::endl;
//      for(std::pair<std::string, std::pair<std::size_t, std::size_t> > pair
//          : preInteractions)
//        str << "\t\t\t" << pair.first << " : success :"
//            << pair.second.first << ", failure :" << pair.second.second
//            << std::endl;
//    }
//  else
//    {
//      str << std::endl << "\t\tPre interaction : None" << std::endl;
//    }
//  if(!postInteractions.empty())
//    {
//      str << "\t\tPost interaction : " << std::endl;
//      for(std::pair<std::string, std::pair<std::size_t, std::size_t> >
//          pair : postInteractions)
//        str << "\t\t\t" << pair.first
//            << " : enacted " << pair.second.first
//            << " and try : " << pair.second.second<< std::endl;
//    }
//  else
//    str << std::endl << "\t\tPost interaction : None" << std::endl;
  return str.str();
}

std::string UsageTable::toKtbs(const std::string &id,
                              const std::string &timeStep) const
{
  return std::string(id + "" + timeStep);
}

