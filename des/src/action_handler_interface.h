#pragma once

#include <fwds.h>

#include <queue>
#include <string>

namespace actions
{
struct ActionHandlerI
{
  ActionHandlerI(int _caster_id, int _target_id, const state::State&)
      : caster_id(_caster_id), target_id(_target_id)
  {
  }
  virtual void operator()(
      std::priority_queue<events::Event, std::vector<events::Event>, decltype(&events::compareEvent)>&,
      state::State&, logging::CombatLog&) = 0;

  virtual ~ActionHandlerI(){};

  const int caster_id;
  const int target_id;
  const double cast_time = 0;
};
}  // namespace actions
