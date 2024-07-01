#include "wss_godot_interface.h"

#include <combat_log.h>
#include <godot_cpp/core/class_db.hpp>

#include <runner.h>

#include <filesystem>

#include <chrono>
#include <ctime>
#include <fstream>
#include <functional>
#include <memory>
#include <omp.h>
#include <queue>
#include <random>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

using namespace godot;

thread_local bool pressed = false;

namespace
{
logging::CombatLog current_log;
runner::Playback playback;
Dictionary resultDict(const logging::CombatLog& log)
{
  Array arr;
  for (const auto& e : log.log)
  {
    Dictionary entry;
    // entry["t"] = e.time;
    entry["owner"] = e->owner;
    entry["note"] = e->note.c_str();
    arr.push_back(entry);
  }
  Dictionary d;
  d["events"] = arr;
  return d;
}

}  // namespace

void GDExample2::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("run_sim"), &GDExample2::runSim);
  ADD_SIGNAL(MethodInfo(
      "position_changed", PropertyInfo(Variant::OBJECT, "node"), PropertyInfo(Variant::VECTOR2, "new_pos")));
  ADD_SIGNAL(MethodInfo(
      "sim_complete", PropertyInfo(Variant::OBJECT, "node"), PropertyInfo(Variant::DICTIONARY, "result")));
  ADD_SIGNAL(
      MethodInfo("time_out", PropertyInfo(Variant::OBJECT, "node"), PropertyInfo(Variant::FLOAT, "timeout")));
  ADD_SIGNAL(MethodInfo(
      "battle_event", PropertyInfo(Variant::OBJECT, "node"), PropertyInfo(Variant::DICTIONARY, "event")));
  ADD_SIGNAL(MethodInfo("owner_battle_event_0",
                        PropertyInfo(Variant::OBJECT, "node"),
                        PropertyInfo(Variant::DICTIONARY, "event")));
  ADD_SIGNAL(MethodInfo("owner_battle_event_1",
                        PropertyInfo(Variant::OBJECT, "node"),
                        PropertyInfo(Variant::DICTIONARY, "event")));
  ADD_SIGNAL(MethodInfo("owner_battle_event_2",
                        PropertyInfo(Variant::OBJECT, "node"),
                        PropertyInfo(Variant::DICTIONARY, "event")));
  ADD_SIGNAL(MethodInfo("owner_battle_event_3",
                        PropertyInfo(Variant::OBJECT, "node"),
                        PropertyInfo(Variant::DICTIONARY, "event")));
  ADD_SIGNAL(MethodInfo("owner_battle_event_4",
                        PropertyInfo(Variant::OBJECT, "node"),
                        PropertyInfo(Variant::DICTIONARY, "event")));
  ADD_SIGNAL(MethodInfo("owner_battle_event_5",
                        PropertyInfo(Variant::OBJECT, "node"),
                        PropertyInfo(Variant::DICTIONARY, "event")));
}

GDExample2::GDExample2()
{
  time_passed = 0.0;
}

GDExample2::~GDExample2() {}

void GDExample2::runSim()
{
  std::cout << "RUNSIM:";
  current_log = runner::runSim();
  emit_signal("sim_complete", this, resultDict(current_log));
  playback.playback(&current_log, std::bind(&GDExample2::handleLogEvent, this, std::placeholders::_1));
  pressed = true;
}

void GDExample2::_pressed()
{
  current_log = runner::runSim();
  emit_signal("sim_complete", this, resultDict(current_log));
  playback.playback(&current_log, std::bind(&GDExample2::handleLogEvent, this, std::placeholders::_1));
  pressed = true;
}

void GDExample2::emitOwnerBattleEvent(int owner, const Dictionary& entry)
{
  if (owner == 0)
    call_deferred("emit_signal", "owner_battle_event_0", this, entry);
  if (owner == 1)
    call_deferred("emit_signal", "owner_battle_event_1", this, entry);
  if (owner == 2)
    call_deferred("emit_signal", "owner_battle_event_2", this, entry);
  if (owner == 3)
    call_deferred("emit_signal", "owner_battle_event_3", this, entry);
  if (owner == 4)
    call_deferred("emit_signal", "owner_battle_event_4", this, entry);
  if (owner == 5)
    call_deferred("emit_signal", "owner_battle_event_5", this, entry);
}

void GDExample2::handleLogEvent(logging::LogEvent* log_event)
{
  std::cout << "HANDLING LOG EVENT";
  if (log_event->tid == std::type_index(typeid(logging::MoveLog)))
  {
    Dictionary entry;
    logging::MoveLog* l = dynamic_cast<logging::MoveLog*>(log_event);
    entry["type"] = "move";
    entry["owner"] = log_event->owner;
    entry["start_x"] = l->move_details.start_x;
    entry["end_x"] = l->move_details.end_x;
    entry["end_t"] = l->move_details.end_t;
    entry["target_id"] = l->move_details.target_id;
    emitOwnerBattleEvent(log_event->owner, entry);
    std::cout << l->move_details.target_id << " " << l->move_details.end_x;
  }
  if (log_event->tid == std::type_index(typeid(logging::FaintLog)))
  {
    logging::FaintLog* l = dynamic_cast<logging::FaintLog*>(log_event);
    Dictionary entry;
    entry["type"] = "faint";
    entry["owner"] = log_event->owner;
    emitOwnerBattleEvent(log_event->owner, entry);
  }
  if (log_event->tid == std::type_index(typeid(logging::AttackLog)))
  {
    logging::AttackLog* l = dynamic_cast<logging::AttackLog*>(log_event);
    Dictionary entry;
    entry["type"] = "attack";
    entry["owner"] = log_event->owner;
    emitOwnerBattleEvent(log_event->owner, entry);
  }
  if (log_event->tid == std::type_index(typeid(logging::TookDamageLog)))
  {
    logging::TookDamageLog* l = dynamic_cast<logging::TookDamageLog*>(log_event);
    Dictionary entry;
    entry["type"] = "took_damage";
    entry["owner"] = log_event->owner;
    entry["damage"] = l->damage;
    entry["remaining_hp"] = l->remaining_hp;
    entry["max_hp"] = l->max_hp;
    emitOwnerBattleEvent(log_event->owner, entry);
  }
  if (log_event->tid == std::type_index(typeid(logging::EndMatchLog)))
  {
    logging::EndMatchLog* l = dynamic_cast<logging::EndMatchLog*>(log_event);
    Dictionary entry;
    entry["type"] = "end_match";
    entry["owner"] = log_event->owner;
    entry["winner"] = l->winner;
    call_deferred("emit_signal", "battle_event", this, entry);
  }
  if (log_event->tid == std::type_index(typeid(logging::StartMatchLog)))
  {
    logging::StartMatchLog* l = dynamic_cast<logging::StartMatchLog*>(log_event);
    Dictionary entry;
    entry["type"] = "start_match";
    entry["owner"] = log_event->owner;
    call_deferred("emit_signal", "battle_event", this, entry);
  }
  if (log_event->tid == std::type_index(typeid(logging::CharacterUpdateLog)))
  {
    logging::CharacterUpdateLog* l = dynamic_cast<logging::CharacterUpdateLog*>(log_event);
    Dictionary entry;
    entry["type"] = "update_character";
    entry["owner"] = log_event->owner;
    entry["max_hp"] = l->unit.max_hp;
    entry["hp"] = l->unit.hp;
    emitOwnerBattleEvent(log_event->owner, entry);
  }
}

void GDExample2::_process(double delta)
{
  if (!pressed)
    return;
  time_passed += delta;
  Vector2 new_position =
      Vector2(10.0 + (10.0 * sin(time_passed * 2.0)), 10.0 + (10.0 * cos(time_passed * 1.5)));

  set_position(new_position);

  time_emit += delta;
  if (time_emit > .1)
  {
    emit_signal("position_changed", this, new_position);
    emit_signal("time_out", this, delta);

    time_emit = 0.0;
  }
}
