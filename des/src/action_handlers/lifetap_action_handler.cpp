#include <action_factory.h>
#include <actions.h>
#include <combat_log.h>
#include <effects.h>

namespace
{
static constexpr double cast_time = 0;
static constexpr double conversion = 1.0;
}  // namespace

namespace actions
{
struct LifetapActionHandler : public ActionHandlerI
{
  LifetapActionHandler(int _caster_id, int _target_id, const state::State& state)
      : actions::ActionHandlerI(_caster_id, _target_id, state)
  {
  }

  void operator()(std::priority_queue<events::Event, std::vector<events::Event>,
                                      decltype(&events::compareEvent)>& event_queue,
                  state::State& state, logging::CombatLog& log)
  {
    // log.addLogEvent(
    //     state.time, this->caster_id, std::string(" casted Lifetap. Gained: " + std::to_string(conversion)));
  }
};
}  // namespace actions

std::unique_ptr<actions::LifetapActionHandler> createLifeTapActionHandler(int caster_id, int target_id,
                                                                          const state::State& state)
{
  return std::make_unique<actions::LifetapActionHandler>(caster_id, target_id, state);
}
REGISTER_ACTION(LIFETAP, createLifeTapActionHandler);
