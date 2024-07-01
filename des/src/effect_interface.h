#pragma once

#include <combat_log.h>

#include <fwds.h>
#include <queue>
#include <string>

namespace effects
{
struct EffectI
{
  EffectI(int _caster_id, int _target_id, int _spell_id)
      : caster_id(_caster_id), target_id(_target_id), spell_id(_spell_id)
  {
  }
  virtual void operator()(
      std::priority_queue<events::Event, std::vector<events::Event>, decltype(&events::compareEvent)>&,
      state::State&, logging::CombatLog&) = 0;

  virtual ~EffectI(){};

  const int caster_id;
  const int target_id;
  const int spell_id;
};
}  // namespace effects
