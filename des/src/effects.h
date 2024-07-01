#pragma once

#include <action_handler_interface.h>
#include <combat_log.h>
#include <effect_interface.h>
#include <event.h>
#include <fwds.h>
#include <memory>
#include <state.h>
#include <utils.h>

static constexpr double gcd = 1.5;

namespace effects
{
struct RemoveISB : public EffectI
{
  RemoveISB(int, int, int);
  virtual void operator()(std::priority_queue<events::Event, std::vector<events::Event>,
                                              decltype(&events::compareEvent)>& event_queue,
                          state::State& state, logging::CombatLog& log) override;
};

// struct AutoAttack : public EffectI
// {
//   AutoAttack(int, int, int);
//   AutoAttack() = delete;
//
//   virtual void operator()(std::priority_queue<events::Event, std::vector<events::Event>,
//                                               decltype(&events::compareEvent)>& event_queue,
//                           state::State& state, logging::CombatLog& log) override;
// };

// struct GCD : public EffectI
// {
//   GCD(int, int, int);
//   GCD() = delete;
//
//   virtual void operator()(std::priority_queue<events::Event, std::vector<events::Event>,
//                                               decltype(&events::compareEvent)>& event_queue,
//                           state::State& state, logging::CombatLog& log) override;
// };

struct Cast : public EffectI
{
  Cast(int, int, int, std::unique_ptr<actions::ActionHandlerI> _action_handler, double _cast_time);
  virtual void operator()(std::priority_queue<events::Event, std::vector<events::Event>,
                                              decltype(&events::compareEvent)>& event_queue,
                          state::State& state, logging::CombatLog& log) override;

  std::unique_ptr<actions::ActionHandlerI> action_handler;
};

struct PeriodicDamageTick : public EffectI
{
  PeriodicDamageTick(int, int, int, double time, double _snapshot_tick_dmg, bool _reset_after = false);

  double initial_cast_time;
  double snapshot_tick_dmg;
  bool reset_after;

  virtual void operator()(std::priority_queue<events::Event, std::vector<events::Event>,
                                              decltype(&events::compareEvent)>& event_queue,
                          state::State& state, logging::CombatLog& log) override;
};

struct ProjectileHit : public EffectI
{
  ProjectileHit(int, int, int, double _dmg, utils::CastPointResult _cast_point_result);

  virtual void operator()(std::priority_queue<events::Event, std::vector<events::Event>,
                                              decltype(&events::compareEvent)>& event_queue,
                          state::State& state, logging::CombatLog& log) override;

  double dmg;
  utils::CastPointResult cast_point_result;
};

struct Heal : public EffectI
{
  Heal(int, int, int, double _heal, utils::CastPointResult _cast_point_result);

  virtual void operator()(std::priority_queue<events::Event, std::vector<events::Event>,
                                              decltype(&events::compareEvent)>& event_queue,
                          state::State& state, logging::CombatLog& log) override;

  double heal;
  utils::CastPointResult cast_point_result;
};

struct AttackIfAble : public EffectI
{
  AttackIfAble(int caster_id, int /*target_id*/, int /*spell_id*/, int slot);

  virtual void operator()(std::priority_queue<events::Event, std::vector<events::Event>,
                                              decltype(&events::compareEvent)>& event_queue,
                          state::State& state, logging::CombatLog& log) override;

  int slot;
};

struct Stop : public EffectI
{
  Stop(int caster_id, int target_id, int);
  virtual void operator()(std::priority_queue<events::Event, std::vector<events::Event>,
                                              decltype(&events::compareEvent)>& event_queue,
                          state::State& state, logging::CombatLog& log) override;
};

struct Move : public EffectI
{
  Move(int caster_id, int target_id, int);
  virtual void operator()(std::priority_queue<events::Event, std::vector<events::Event>,
                                              decltype(&events::compareEvent)>& event_queue,
                          state::State& state, logging::CombatLog& log) override;
};
}  // namespace effects
