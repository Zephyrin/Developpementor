#ifndef EXISTENCEPHENOMENAL_HPP
#define EXISTENCEPHENOMENAL_HPP

#include "interactions.hpp"
#include "phenomenas.hpp"
#include "mood.hpp"

#include <cstddef>

namespace Phenomenal
{
  class Existence
  {
  public:
    Existence(const Interactions &interactions,
              const std::size_t &maxInteractionHeight);
    ~Existence();

    Interactions &getInteractions() ;

    const Phenomenas &getPhenomenas() const;

    Interaction *stepExperience();

    void stepResult(Interaction *enacted);
    const Interactions &getInteractions() const;
    Phenomenas &getPhenomenas();
    MOOD getMood() const;
  private:
    MOOD mood;

    Interactions interactions;

    Phenomenas phenomenas;

    Interaction *intended;

    Interaction *enacted;
  };
}

#endif // EXISTENCEPHENOMENAL_HPP
