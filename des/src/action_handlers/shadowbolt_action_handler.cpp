
#include <action_factory.h>
#include <actions.h>
#include <combat_log.h>
#include <effects.h>

namespace
{
static constexpr int spell_school = spell_schools::SHADOW;
static constexpr int specialization = specialization_types::DESTRUCTION;
static constexpr int min = 20;
static constexpr int max = 30;
static constexpr double cast_time = 1.5;
static constexpr double coefficient = cast_time / 3.5;
}  // namespace

namespace actions
{
struct ShadowboltActionHandler : public ActionHandlerI
{
  ShadowboltActionHandler(int _caster_id, int _target_id, const state::State& state)
      : actions::ActionHandlerI(_caster_id, _target_id, state)
  {
  }

  void operator()(std::priority_queue<events::Event, std::vector<events::Event>,
                                      decltype(&events::compareEvent)>& event_queue,
                  state::State& state, logging::CombatLog& log) override
  {
    auto& caster_state = state.units[this->caster_id];
    auto& target_state = state.units[this->target_id];
    utils::CastPointResult result = utils::nonBinaryCast(state.units[this->caster_id].spell_crit_chance,
                                                         state.units[this->caster_id].spell_hit_chance,
                                                         spell_school,
                                                         specialization);

    if (result.hit)
    {
      // log.addLogEvent(state.time, this->caster_id, std::string(" casted Shadowbolt; hit."));

      double dmg = utils::nonBinaryDamageCalculation(
          result, min, max, spell_school, specialization, coefficient, caster_state, target_state);
      event_queue.push(
          events::Event(state.time,
                        std::make_unique<::effects::ProjectileHit>(
                            this->caster_id, this->target_id, action_ids::SHADOWBOLT, dmg, result)));
    }
    else
    {
      // log.addLogEvent(state.time, this->caster_id, std::string(" casted R4 Shadowbolt; resisted."));
    }
  }
};
}  // namespace actions

std::unique_ptr<actions::ShadowboltActionHandler> createShadowboltActionHandler(int caster_id, int target_id,
                                                                                const state::State& state)
{
  return std::make_unique<actions::ShadowboltActionHandler>(caster_id, target_id, state);
}
REGISTER_ACTION(SHADOWBOLT, createShadowboltActionHandler);
