#pragma once

#include <action_handler_interface.h>

#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>

#define REGISTER_ACTION(ActionID, MakeFunc)                                                                \
  namespace action_ids                                                                                     \
  {                                                                                                        \
  bool ActionID##_entry = actions::ActionFactory::add(                                                     \
      ActionID, std::bind(MakeFunc, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)); \
  }

namespace actions
{
class ActionFactory
{
 public:
  typedef std::unordered_map<int,
                             std::function<std::unique_ptr<ActionHandlerI>(int, int, const state::State&)>>
      FactoryMap;

  static bool add(
      const int action_id,
      std::function<std::unique_ptr<ActionHandlerI>(int caster_id, int target_id, const state::State& state)>
          makeFunc)
  {
    auto& map = getFactoryMap();
    if (map.find(action_id) != map.end())
      return false;
    map[action_id] = makeFunc;
    return true;
  }

  static std::unique_ptr<ActionHandlerI> create(int caster_id, int target_id, int action_id,
                                                const state::State& state)
  {
    FactoryMap& map = getFactoryMap();
    if (map.find(action_id) == map.end())
    {
      return nullptr;
    }
    return map[action_id](caster_id, target_id, state);
  }

 private:
  static FactoryMap& getFactoryMap()
  {
    static FactoryMap map;
    return map;
  }
};
}  // namespace actions
