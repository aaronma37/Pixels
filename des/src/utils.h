#pragma once
#include <fwds.h>
#include <state.h>

#include <queue>
#include <random>
#include <set>

namespace policies
{
inline void act(int caster_id,
                std::priority_queue<events::Event, std::vector<events::Event>,
                                    decltype(&events::compareEvent)>& event_queue,
                state::State& state, logging::CombatLog& os)
{
}
}  // namespace policies

namespace utils
{
struct CastPointResult
{
  bool hit;
  double partial_resist_mod;
  bool crit;
};

inline CastPointResult nonBinaryCast(double crit_chance, double hit_chance, int spell_school_idx,
                                     int specialization_idx)
{
  CastPointResult result;
  result.hit = (((double)rand() / (RAND_MAX)) <= hit_chance);
  result.crit = (((double)rand() / (RAND_MAX)) <= crit_chance);
  auto partial_roll = ((double)rand() / (RAND_MAX));
  if (partial_roll <= .63)
    result.partial_resist_mod = 1.0;
  else if (partial_roll <= 90)
    result.partial_resist_mod = .75;
  else if (partial_roll <= 99)
    result.partial_resist_mod = .5;
  else
    result.partial_resist_mod = .25;
  return result;
}

inline double nonBinaryDamageCalculation(CastPointResult& result, double lb, double ub, int spell_school_idx,
                                         int specialization_idx, double coeff, unit::Unit& caster,
                                         unit::Unit& target)
{
  double total_spell_power = caster.spell_power + caster.school_spell_power[spell_school_idx];
  double dmg = lb + (rand() % static_cast<int>(ub - lb + 1)) + coeff * total_spell_power;
  if (result.crit)
    dmg *= caster.spell_crit_mod;
  dmg *= result.partial_resist_mod;
  return dmg;
}

inline double nonBinaryHealCalculation(CastPointResult& result, double lb, double ub, int spell_school_idx,
                                       int specialization_idx, double coeff, unit::Unit& caster,
                                       unit::Unit& target)
{
  double total_spell_power = caster.spell_power + caster.school_spell_power[spell_school_idx];
  double dmg = lb + (rand() % static_cast<int>(ub - lb + 1)) + coeff * total_spell_power;
  if (result.crit)
    dmg *= caster.spell_crit_mod;
  return dmg;
}

inline double getPos(unit::Unit& u, double t)
{
  if (u.move_details == std::nullopt)
    return u.x;
  double dur = u.move_details->end_t - u.move_details->start_t;
  u.x = u.move_details->start_x +
        (u.move_details->end_x - u.move_details->start_x) * (t - u.move_details->start_t) / dur;
  return u.x;
}

inline void checkVictory(state::State& state)
{
  std::set<int> living_faction;
  if (state.victor == std::nullopt)
  {
    for (const auto& [t, v] : state.units)
    {
      if (v.alive)
        living_faction.insert(v.faction);
    }
  }

  if (living_faction.size() == 1)
  {
    state.victor = *living_faction.begin();
  }
}

inline std::optional<int> refreshHostileTarget(state::State& state, int caster_id)
{
  unit::Unit& u = state.units.at(caster_id);
  std::optional<int> target = std::nullopt;
  double max_threat = -1;

  for (auto& [t, v] : state.units)
  {
    if (v.faction == u.faction || t == caster_id || !v.alive)
      continue;

    if (u.threat.count(t) < 1)
      u.threat[t] = 0;

    if ((target == std::nullopt || u.threat.at(t) > max_threat) && v.alive)
    {
      target = t;
      max_threat = u.threat.at(t);
    }
  }

  u.current_hostile_target = target;
  u.current_max_threat = max_threat;
  return target;
}

inline void resetThreat(state::State& state, int caster_id)
{
  for (auto& [t, v] : state.units)
  {
    v.threat[caster_id] = 0;
    if (v.current_hostile_target && v.current_hostile_target.value() == caster_id)
    {
      v.current_hostile_target = std::nullopt;
      v.current_max_threat = 0;
      refreshHostileTarget(state, t);
    }
  }
}

}  // namespace utils
