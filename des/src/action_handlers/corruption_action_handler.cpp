
#include <action_factory.h>
#include <actions.h>
#include <combat_log.h>
#include <effects.h>

namespace
{
static constexpr double coefficient = 1.0;
static constexpr int spell_school = spell_schools::SHADOW;
static constexpr int specialization = specialization_types::AFFLICTION;
static constexpr int tick_dmg = 15;
static constexpr double cast_time = 1.5;
}  // namespace

namespace actions
{
struct CorruptionActionHandler : public ActionHandlerI
{
  CorruptionActionHandler(int _caster_id, int _target_id, const state::State& state)
      : actions::ActionHandlerI(_caster_id, _target_id, state)
  {
  }

  void operator()(std::priority_queue<events::Event, std::vector<events::Event>,
                                      decltype(&events::compareEvent)>& event_queue,
                  state::State& state, logging::CombatLog& log)
  {
    auto& caster_state = state.units[this->caster_id];
    auto& target_state = state.units[this->target_id];
    utils::CastPointResult result = utils::nonBinaryCast(state.units[this->caster_id].spell_crit_chance,
                                                         state.units[this->caster_id].spell_hit_chance,
                                                         spell_school,
                                                         specialization);

    if (result.hit)
    {
      double dmg = utils::nonBinaryDamageCalculation(
          result, tick_dmg, tick_dmg, spell_school, specialization, coefficient, caster_state, target_state);

      // log.addLogEvent(state.time, this->caster_id, std::string(" casted  Corruption; hit."));
      const double snapshot_tick_dmg = tick_dmg;

      event_queue.push(events::Event(state.time + 3,
                                     std::make_unique<effects::PeriodicDamageTick>(
                                         caster_id, target_id, action_ids::CORRUPTION, state.time, dmg)));

      event_queue.push(events::Event(state.time + 6,
                                     std::make_unique<effects::PeriodicDamageTick>(
                                         caster_id, target_id, action_ids::CORRUPTION, state.time, dmg)));

      event_queue.push(events::Event(state.time + 9,
                                     std::make_unique<effects::PeriodicDamageTick>(
                                         caster_id, target_id, action_ids::CORRUPTION, state.time, dmg)));

      event_queue.push(events::Event(state.time + 12,
                                     std::make_unique<effects::PeriodicDamageTick>(
                                         caster_id, target_id, action_ids::CORRUPTION, state.time, dmg)));

      event_queue.push(events::Event(state.time + 15,
                                     std::make_unique<effects::PeriodicDamageTick>(
                                         caster_id, target_id, action_ids::CORRUPTION, state.time, dmg)));

      event_queue.push(
          events::Event(state.time + 18,
                        std::make_unique<effects::PeriodicDamageTick>(
                            caster_id, target_id, action_ids::CORRUPTION, state.time, dmg, true)));
    }
    else
    {
      // log.addLogEvent(state.time, this->caster_id, std::string(" casted R4 Corruption; resisted."));
    }
  }
};
}  // namespace actions

std::unique_ptr<actions::CorruptionActionHandler> createCorruptionActionHandler(int caster_id, int target_id,
                                                                                const state::State& state)
{
  return std::make_unique<actions::CorruptionActionHandler>(caster_id, target_id, state);
}
REGISTER_ACTION(CORRUPTION, createCorruptionActionHandler);
