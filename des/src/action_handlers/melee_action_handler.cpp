#include <action_factory.h>
#include <actions.h>
#include <combat_log.h>
#include <effects.h>

namespace actions
{
struct MeleeActionHandler : public ActionHandlerI
{
  MeleeActionHandler(int _caster_id, int _target_id, const state::State& state)
      : actions::ActionHandlerI(_caster_id, _target_id, state)
  {
  }

  void operator()(std::priority_queue<events::Event, std::vector<events::Event>,
                                      decltype(&events::compareEvent)>& event_queue,
                  state::State& state, logging::CombatLog& log)
  {
    // log.addLogEvent(state.time, this->caster_id, std::string(" casted melee."));
  }
};
}  // namespace actions

std::unique_ptr<actions::MeleeActionHandler> createMeleeActionHandler(int caster_id, int target_id,
                                                                      const state::State& state)
{
  return std::make_unique<actions::MeleeActionHandler>(caster_id, target_id, state);
}
REGISTER_ACTION(NONE, createMeleeActionHandler);
