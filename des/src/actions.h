#pragma once

#include <action_factory.h>
#include <combat_log.h>
#include <effects.h>
#include <state.h>

#include <memory.h>
#include <optional>

namespace action_ids
{
static constexpr int NONE = 0;
static constexpr int SHADOWBOLT = 1;
static constexpr int CORRUPTION = 2;
static constexpr int LIFETAP = 3;
static constexpr int MINOR_HEAL = 4;
static constexpr int MOVE = 5;
}  // namespace action_ids

namespace actions
{

inline int rewardFuncion(int action_id, int caster_id, int target_id, const state::State& state)
{
  if (action_id == action_ids::MOVE)
  {
    if (state.units.at(caster_id).faction == state.units.at(target_id).faction)
      return 0;
    return 1;
  }
  if (action_id == action_ids::SHADOWBOLT)
  {
    if (state.units.at(caster_id).gcd > state.time)
      return std::numeric_limits<int>::lowest();
    if (state.units.at(caster_id).faction == state.units.at(target_id).faction)
      return 0;
    if (state.units.at(target_id).hp > 0)
      return 1;
    else
      return 0;
  }
  else if (action_id == action_ids::MINOR_HEAL)
  {
    if (state.units.at(caster_id).gcd > state.time)
      return std::numeric_limits<int>::lowest();
    if (state.units.at(caster_id).faction == state.units.at(target_id).faction &&
        state.units.at(target_id).hp > 0 && state.units.at(target_id).hp < 50)
      return 2;
    else
      return 0;
  }
  else
    return 0;
}

inline std::tuple</*action_id*/ int, /*target_id*/ int> policy(
    int caster_id, state::State& state,
    std::priority_queue<events::Event, std::vector<events::Event>, decltype(&events::compareEvent)>&
        event_queue,
    logging::CombatLog& log)
{
  int best_reward = 0;
  int best_action = action_ids::NONE;
  int best_target = caster_id;
  unit::Unit& u = state.units.at(caster_id);

  std::optional<int> hostile_target = u.current_hostile_target;
  if (hostile_target == std::nullopt || state.units.at(*u.current_hostile_target).alive == false)
  {
    hostile_target = utils::refreshHostileTarget(state, caster_id);
  }
  const std::vector<int>& action_space = u.action_space;

  for (auto& [t, v] : state.units)
  {
    if (v.faction == u.faction || (hostile_target && hostile_target == t))
    {
      for (auto a : action_space)
      {
        int r = rewardFuncion(a, caster_id, t, state);
        if (r > best_reward)
        {
          best_reward = r;
          best_action = a;
          best_target = t;
        }
      }
    }
  }

  return std::make_tuple(best_action, best_target);
};

inline void cast(int caster_id, int target_id, double cast_time, const state::State& state,
                 std::priority_queue<events::Event, std::vector<events::Event>,
                                     decltype(&events::compareEvent)>& event_queue,
                 logging::CombatLog& log, int action_id)
{
  // log.addLogEvent(state.time, caster_id, std::string(" started casting .") + std::to_string(action_id));
  event_queue.push(events::Event(
      state.time + cast_time,
      std::make_unique<::effects::Cast>(caster_id,
                                        target_id,
                                        action_id,
                                        ActionFactory::create(caster_id, target_id, action_id, state),
                                        cast_time)));
}

inline void act(int caster_id, state::State& state,
                std::priority_queue<events::Event, std::vector<events::Event>,
                                    decltype(&events::compareEvent)>& event_queue,
                logging::CombatLog& log)
{
  if (state.units.at(caster_id).hp > 0)
  {
    int _action_id, _target_id;
    std::tie(_action_id, _target_id) = policy(caster_id, state, event_queue, log);
    if (_action_id != action_ids::NONE && _action_id != action_ids::MOVE)
    {
      double cast_time = 5.0;
      cast(caster_id, _target_id, cast_time, state, event_queue, log, _action_id);
    }
    else if (_action_id == action_ids::MOVE)
    {
      event_queue.push(
          events::Event(state.time, std::make_unique<::effects::Move>(caster_id, _target_id, _action_id)));
    }
  }
}

}  // namespace actions
