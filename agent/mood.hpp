#ifndef MOOD_HPP
#define MOOD_HPP

#include <ostream>

enum class MOOD {
  NO_MOOD = 0,
  SELF_SATISFIED = 1,
  FRUSTRATED = 2,
  BORED = 4,
  PAINED = 8,
  PLEASED = 16,
  CURIOUS = 32,
  REFLEXION = 64,
  SIMULATION = 128,
  HEDONIST = 256,
  EXCITED = 512,
  MAXMOODVALUE = 1024
};

inline MOOD operator|(MOOD a, MOOD b)
{ return static_cast<MOOD>(static_cast<int>(a) | static_cast<int>(b)); }

inline MOOD operator &(MOOD a, MOOD b)
{ return static_cast<MOOD>(static_cast<int>(a) & static_cast<int>(b)); }

inline bool operator !=(MOOD a, int b)
{ return static_cast<int>(a) != b; }

inline bool moodAhasB(MOOD a, MOOD b)
{ return (a & b) != 0; }
//std::ostream & operator <<(std::ostream &os, MOOD m);

inline void clearMood(MOOD &mood)
{ mood = MOOD::NO_MOOD; }

inline void addMood(MOOD &mood, MOOD a)
{ mood = (mood | a); }

inline std::ostream & operator <<(std::ostream &os, MOOD m)
{
    if((m & MOOD::BORED) != MOOD::NO_MOOD)
        os << " Bored, ";
    if((m & MOOD::SELF_SATISFIED) != MOOD::NO_MOOD)
      os << " Satisfied, ";
    if((m & MOOD::CURIOUS) != MOOD::NO_MOOD)
      os << " Curious, ";
    if((m & MOOD::FRUSTRATED) != MOOD::NO_MOOD)
      os << " Frustrated, ";
    if((m & MOOD::PAINED) != MOOD::NO_MOOD)
      os << " Pained, ";
    if((m & MOOD::PLEASED ) != MOOD::NO_MOOD)
      os << " Pleased, ";
    if((m & MOOD::EXCITED ) != MOOD::NO_MOOD)
      os << " Excited, ";
    if((m & MOOD::HEDONIST) != MOOD::NO_MOOD)
      os << " Hedonist, ";
    return os;
}

#endif // MOOD_H

