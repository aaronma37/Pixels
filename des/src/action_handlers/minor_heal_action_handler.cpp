#include <action_factory.h>
#include <actions.h>
#include <combat_log.h>
#include <effects.h>

namespace
{
static constexpr int action_id = action_ids::MINOR_HEAL;
static constexpr int spell_school = spell_schools::HOLY;
static constexpr int specialization = specialization_types::HOLY;
static constexpr int min = 20;
static constexpr int max = 50;
static constexpr double cast_time = .25;
static constexpr double coefficient = cast_time / 3.5;
}  // namespace

namespace actions
{
struct MinorHealActionHandler : public ActionHandlerI
{
  MinorHealActionHandler(int _caster_id, int _target_id, const state::State& state)
      : actions::ActionHandlerI(_caster_id, _target_id, state)
  {
  }

  void operator()(std::priority_queue<events::Event, std::vector<events::Event>,
                                      decltype(&events::compareEvent)>& event_queue,
                  state::State& state, logging::CombatLog& log)
  {
    auto& caster_state = state.units[this->caster_id];
    auto& target_state = state.units[this->target_id];
    utils::CastPointResult result = utils::nonBinaryCast(
        state.units[this->caster_id].spell_crit_chance, 1.0, spell_school, specialization);
    // log.addLogEvent(
    //     state.time, this->caster_id, std::string(" casted Minor Heal on ") + std::to_string(this->target_id));
    double heal = utils::nonBinaryHealCalculation(
        result, min, max, spell_school, specialization, coefficient, caster_state, target_state);

    event_queue.push(events::Event(
        state.time,
        std::make_unique<::effects::Heal>(this->caster_id, this->target_id, action_id, heal, result)));
  }
};
}  // namespace actions

std::unique_ptr<actions::MinorHealActionHandler> createMinorHealActionHandler(int caster_id, int target_id,
                                                                              const state::State& state)
{
  return std::make_unique<actions::MinorHealActionHandler>(caster_id, target_id, state);
}
REGISTER_ACTION(MINOR_HEAL, createMinorHealActionHandler);
