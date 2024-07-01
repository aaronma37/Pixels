#include <actions.h>
#include <effects.h>

namespace
{
bool sameSign(int num1, int num2)
{
  if (num1 > 0 && num2 < 0)
    return false;
  if (num1 < 0 && num2 > 0)
    return false;
  return true;
}

void HealDamage(int caster_id, int target_id, int spell_id, double hp, state::State& state,
                logging::CombatLog& log)
{
  if (state.units.at(target_id).hp < 1)
    return;
  state.units.at(target_id).hp += hp;

  if (state.units.at(target_id).hp > state.units.at(target_id).max_hp)
    state.units.at(target_id).hp = state.units.at(target_id).max_hp;
}

void ReceiveDamage(int caster_id, int target_id, int spell_id, double dmg, state::State& state,
                   logging::CombatLog& log)
{
  if (state.units.at(target_id).hp > 0)
  {
    state.units.at(target_id).threat[caster_id] += dmg;
    state.units.at(target_id).hp -= dmg;
    log.addLogEvent(std::make_unique<logging::TookDamageLog>(
        state.time, target_id, dmg, state.units.at(target_id).hp, state.units.at(target_id).max_hp));
    if (state.units.at(target_id).threat[caster_id] > state.units.at(target_id).current_max_threat ||
        state.units.at(target_id).current_hostile_target == std::nullopt)
    {
      state.units.at(target_id).current_max_threat = state.units.at(target_id).threat[caster_id];
      state.units.at(target_id).current_hostile_target = caster_id;
    }
    if (state.units.at(target_id).hp < 1)
    {
      state.units.at(target_id).alive = 0;
      log.addLogEvent(std::make_unique<logging::FaintLog>(state.time, target_id));
      utils::resetThreat(state, target_id);
      utils::checkVictory(state);
      if (state.victor != std::nullopt)
      {
        log.addLogEvent(std::make_unique<logging::EndMatchLog>(state.time, -1, *state.victor));
      }

      bool t0 = false;
      bool t1 = false;
      for (auto& [u, v] : state.units)
      {
        if (v.alive == 1 && v.faction == 0)
          t0 = true;
        if (v.alive == 1 && v.faction == 1)
          t1 = true;
      }
      // if (t0 == false)
      // log.addLogEvent(state.time, 1, " Wins.", logging::Color::GRAY);
      // else if (t1 == false)
      // log.addLogEvent(state.time, 0, " Wins.", logging::Color::GRAY);
    }
  }
}

}  // namespace

namespace effect_ids
{
static constexpr int GCD = 0;
static constexpr int CAST = 1;
static constexpr int PERIODIC = 2;
static constexpr int PROJECTILE_HIT = 3;
}  // namespace effect_ids

namespace effects
{

// GCD::GCD(int caster_id, int target_id, int spell_id) : EffectI(caster_id, target_id, spell_id) {}
//
// void GCD::operator()(std::priority_queue<events::Event, std::vector<events::Event>,
//                                          decltype(&events::compareEvent)>& event_queue,
//                      state::State& state, logging::CombatLog& log)
// {
//   log.addLogEvent(state.time, caster_id, " is able to act.", logging::Color::GRAY);
//   actions::act(caster_id, state, event_queue, log);
// }

Cast::Cast(int caster_id, int target_id, int spell_id,
           std::unique_ptr<actions::ActionHandlerI> _action_handler, double _cast_time)
    : EffectI(caster_id, target_id, spell_id), action_handler(std::move(_action_handler))
{
}
void Cast::operator()(std::priority_queue<events::Event, std::vector<events::Event>,
                                          decltype(&events::compareEvent)>& event_queue,
                      state::State& state, logging::CombatLog& log)
{
  static constexpr double gcd = 1.5;
  state.units[this->caster_id].gcd = state.time + gcd;
  (*action_handler)(event_queue, state, log);
  ::effects::AttackIfAble p(this->caster_id, -1, 0, 0);
  p(event_queue, state, log);
  actions::act(this->caster_id, state, event_queue, log);
}

PeriodicDamageTick::PeriodicDamageTick(int _caster_id, int target_id, int spell_id, double time,
                                       double _snapshot_tick_dmg, bool _reset_after)
    : EffectI(_caster_id, target_id, spell_id),
      initial_cast_time(time),
      snapshot_tick_dmg(_snapshot_tick_dmg),
      reset_after(_reset_after)
{
}

void PeriodicDamageTick::operator()(std::priority_queue<events::Event, std::vector<events::Event>,
                                                        decltype(&events::compareEvent)>& event_queue,
                                    state::State& state, logging::CombatLog& log)
{
  // if (state.debuffs.corruption_ids[caster_id] == initial_cast_time)
  // {
  //   state.damage_dealt += snapshot_tick_dmg;
  //   std::string note = " dealt " + std::to_string(snapshot_tick_dmg) + " with a " + spell_name + " tick.";
  //   log.addLogEvent(state.time, this->caster_id, note, logging::Color::RED);
  //   if (reset_after)
  //   {
  //     state.debuffs.corruption_ids[caster_id] = 0;
  // log.addLogEvent(state.time, this->caster_id, " " + spell_name + " dissipates.", logging::Color::PURPLE);
  //   }
  // }
}

ProjectileHit::ProjectileHit(int _caster_id, int _target_id, int _spell_id, double _dmg,
                             utils::CastPointResult _cast_point_result)
    : EffectI(_caster_id, _target_id, _spell_id), dmg(_dmg), cast_point_result(_cast_point_result)
{
}

void ProjectileHit::operator()(std::priority_queue<events::Event, std::vector<events::Event>,
                                                   decltype(&events::compareEvent)>& event_queue,
                               state::State& state, logging::CombatLog& log)
{
  ReceiveDamage(caster_id, target_id, spell_id, dmg, state, log);
  std::string note = " dealt " + std::to_string(dmg);
  if (cast_point_result.crit)
    note += "*";
  note += " damage with " + std::to_string(this->spell_id) + " to " + std::to_string(target_id) +
          ". Remaining hp: " + std::to_string(state.units.at(target_id).hp);
  // for (auto& v : state.auras[effect_ids::PROJECTILE_HIT])
  //   v.(effect_ids::PROJECTILE_HIT, _caster_id, )

  // log.addLogEvent(state.time, this->caster_id, note, logging::Color::RED);
}

Heal::Heal(int _caster_id, int _target_id, int _spell_id, double _heal,
           utils::CastPointResult _cast_point_result)
    : EffectI(_caster_id, _target_id, _spell_id), heal(_heal), cast_point_result(_cast_point_result)
{
}

void Heal::operator()(std::priority_queue<events::Event, std::vector<events::Event>,
                                          decltype(&events::compareEvent)>& event_queue,
                      state::State& state, logging::CombatLog& log)
{
  HealDamage(caster_id, target_id, spell_id, heal, state, log);
  std::string note = " restored " + std::to_string(heal);
  if (cast_point_result.crit)
    note += "*";
  note += " damage with " + std::to_string(this->spell_id) + " to " + std::to_string(target_id) +
          ". Remaining hp: " + std::to_string(state.units.at(target_id).hp);
  // log.addLogEvent(state.time, this->caster_id, note, logging::Color::GREEN);
}

AttackIfAble::AttackIfAble(int _caster_id, int, int, int _slot) : EffectI(_caster_id, -1, -1), slot(_slot) {}
void AttackIfAble::operator()(std::priority_queue<events::Event, std::vector<events::Event>,
                                                  decltype(&events::compareEvent)>& event_queue,
                              state::State& state, logging::CombatLog& log)
{
  // Invalid slot; this shouldn't happen unless we allow changiong slots during battle
  if (state.units.at(this->caster_id).attack_slot_data.count(slot) < 1)
    return;

  if (state.units.at(this->caster_id).alive == 0)
    return;

  unit::AttackSlotData& asd = state.units.at(this->caster_id).attack_slot_data.at(slot);

  if (asd.attack_cd > state.time)
  {
    event_queue.push(events::Event(
        asd.attack_cd,
        std::make_unique<::effects::AttackIfAble>(this->caster_id, -1, ::action_ids::NONE, slot)));
    return;
  }

  std::optional<int> target = state.units.at(this->caster_id).current_hostile_target;
  if (target == std::nullopt)
  {
    return;
  }

  utils::getPos(state.units.at(this->caster_id), state.time);
  utils::getPos(state.units.at(*target), state.time);
  if (std::abs(state.units.at(this->caster_id).x - state.units.at(*target).x) <= asd.range)
  {
    //attack
    asd.attack_cd = state.time + asd.attack_dur;
    event_queue.push(events::Event(
        asd.attack_cd,
        std::make_unique<::effects::AttackIfAble>(this->caster_id, -1, ::action_ids::NONE, slot)));

    double damage = asd.dmg_min;
    ReceiveDamage(this->caster_id, *target, 0, damage, state, log);
    log.addLogEvent(
        std::make_unique<logging::AttackLog>(state.time, this->caster_id, *target, damage, false));
    return;
  }
  else
  {
    actions::act(this->caster_id, state, event_queue, log);
    // log.addLogEvent(state.time,
    //                 this->caster_id,
    //                 " auto attack out of range of " + std::to_string(*target) + " at " +
    //                     std::to_string(state.units.at(*target).x),
    //                 logging::Color::PURPLE);
  }
}

Move::Move(int _caster_id, int _target_id, int) : EffectI(_caster_id, _target_id, 0) {}
void Move::operator()(std::priority_queue<events::Event, std::vector<events::Event>,
                                          decltype(&events::compareEvent)>& event_queue,
                      state::State& state, logging::CombatLog& log)
{
  static constexpr double k_offset = .4;
  auto& u = state.units.at(this->caster_id);
  auto& t = state.units.at(this->target_id);

  double my_x = utils::getPos(u, state.time);
  double t_x = utils::getPos(t, state.time);

  double target_x;
  if (t.move_details)
  {
    double initial_x = my_x - t_x;
    bool incoming = (std::abs(t.move_details->end_x - my_x) < std::abs(t.move_details->start_x - my_x));
    double combined_vel;
    if (incoming)
      combined_vel = u.movespeed + t.movespeed;
    else
      combined_vel = u.movespeed - t.movespeed;
    if (combined_vel <= 0)
    {
      target_x = t.move_details->end_x;
    }
    else
    {
      double int_time = std::abs(initial_x / combined_vel);
      if (int_time > t.move_details->end_t)
      {
        if (initial_x < 0)  // Need to go negative
        {
          target_x = t.move_details->end_x - k_offset;
        }
        else  // Need to go positive
        {
          target_x = t.move_details->end_x + k_offset;
        }
      }
      else if (initial_x < 0)  // Need to go negative
      {
        target_x = my_x + int_time * u.movespeed - k_offset;
      }
      else  // Need to go positive
      {
        target_x = my_x - int_time * u.movespeed + k_offset;
      }
    }
  }
  else
    target_x = t_x;

  double distance = std::abs(my_x - target_x);
  if (distance <= k_offset)
  {
    // ::effects::Stop p(this->target_id, this->caster_id, ::action_ids::NONE);
    // p(event_queue, state, log);
    ::effects::AttackIfAble p(this->caster_id, -1, 0, 0);
    return;
  }

  double time = distance / u.movespeed;
  u.move_details = unit::MoveDetails(my_x, target_x, state.time, state.time + time, this->target_id);
  log.addLogEvent(std::make_unique<logging::MoveLog>(state.time, this->caster_id, *u.move_details));

  // Notify targets
  for (auto& [other_target_id, v] : state.units)
  {
    if (v.move_details && v.move_details->target_id == this->caster_id &&
        (/*other_target_id != this->target_id &&*/ std::abs(v.move_details->end_x - u.move_details->end_x) >
         k_offset))
    {
      ::effects::Move p(other_target_id, this->caster_id, ::action_ids::NONE);
      p(event_queue, state, log);
    }
  }

  event_queue.push(
      events::Event(state.time + time,
                    std::make_unique<::effects::Stop>(this->caster_id, this->target_id, ::action_ids::NONE)));
}

Stop::Stop(int _caster_id, int _target_id, int) : EffectI(_caster_id, _target_id, 0) {}
void Stop::operator()(std::priority_queue<events::Event, std::vector<events::Event>,
                                          decltype(&events::compareEvent)>& event_queue,
                      state::State& state, logging::CombatLog& log)
{
  auto& u = state.units.at(this->caster_id);
  u.x = utils::getPos(u, state.time);
  if (state.units.at(this->caster_id).move_details && state.time == u.move_details->end_t)
    state.units.at(this->caster_id).move_details = std::nullopt;

  ::effects::AttackIfAble p(this->caster_id, -1, 0, 0);
  p(event_queue, state, log);
  actions::act(this->caster_id, state, event_queue, log);
}
}  // namespace effects
