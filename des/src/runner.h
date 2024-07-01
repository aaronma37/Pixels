
#include <actions.h>
#include <combat_log.h>
#include <effect_interface.h>
#include <effects.h>
#include <event.h>
#include <fwds.h>
#include <state.h>
#include <unit.h>
#include <utils.h>

#include <chrono>
#include <condition_variable>
#include <ctime>
#include <filesystem>
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

static constexpr int k_iters = 1;
static constexpr double k_time_sec = 120;
std::array<state::State, k_iters> g_final_state_arr;

namespace runner
{
using EventQueue =
    std::priority_queue<events::Event, std::vector<events::Event>, decltype(&events::compareEvent)>;

inline logging::CombatLog runSim()
{
  const int caster_id = 0;
  const int caster_id_2 = 1;
  const int caster_id_3 = 2;
  const int caster_id_4 = 3;
  const int caster_id_5 = 4;
  const int caster_id_6 = 5;
  logging::CombatLog combat_log;
  state::State state;

  unit::Unit u;
  u.faction = 0;
  u.hp = 100;
  u.max_hp = 100;
  u.alive = 1;
  u.x = 0;
  u.attack_speed_mod = 1;
  u.cast_speed_mod = 1;
  u.spell_power = 10;
  u.movespeed = 2.0;
  u.spell_crit_chance = .1;
  u.spell_crit_mod = 1.5;
  u.spell_hit_chance = .83;
  u.physical_hit_chance = .83;
  u.physical_crit_chance = .1;
  u.school_spell_power[spell_schools::FIRE] = 10;
  u.school_spell_power[spell_schools::FROST] = 10;
  u.school_spell_power[spell_schools::HOLY] = 10;
  u.school_spell_power[spell_schools::SHADOW] = 10;
  u.school_spell_power[spell_schools::ARCANE] = 10;
  u.school_spell_power[spell_schools::NATURE] = 10;
  unit::AttackSlotData asd;
  asd.slot = 0;
  asd.attack_dur = 1.0;
  asd.dmg_min = 10.0;
  asd.dmg_max = 10.0;
  asd.range = 1.0;
  u.attack_slot_data[0] = asd;
  // u.action_space.push_back(1);
  // u.action_space.push_back(2);
  // u.action_space.push_back(3);
  // u.action_space.push_back(4);
  u.action_space.push_back(5);
  for (int i = 0; i < 3; i++)
  {
    u.current_hostile_target = 3;
    u.x = -2.5;
    state.units[i] = u;
  }
  u.faction = 1;
  for (int i = 0; i < 3; i++)
  {
    u.current_hostile_target = 0;
    u.x = 2.5;
    u.action_space.push_back(4);
    state.units[i + 3] = u;
  }

  EventQueue event_queue(events::compareEvent);
  actions::act(caster_id, state, event_queue, combat_log);
  event_queue.push(
      events::Event(1, std::make_unique<::effects::AttackIfAble>(caster_id, caster_id, action_ids::NONE, 0)));

  actions::act(caster_id_2, state, event_queue, combat_log);
  event_queue.push(events::Event(
      1, std::make_unique<::effects::AttackIfAble>(caster_id_2, caster_id_2, action_ids::NONE, 0)));

  actions::act(caster_id_3, state, event_queue, combat_log);
  event_queue.push(events::Event(
      1, std::make_unique<::effects::AttackIfAble>(caster_id_3, caster_id_3, action_ids::NONE, 0)));

  actions::act(caster_id_4, state, event_queue, combat_log);
  event_queue.push(events::Event(
      1, std::make_unique<::effects::AttackIfAble>(caster_id_4, caster_id_4, action_ids::NONE, 0)));

  actions::act(caster_id_5, state, event_queue, combat_log);
  event_queue.push(events::Event(
      1, std::make_unique<::effects::AttackIfAble>(caster_id_5, caster_id_5, action_ids::NONE, 0)));

  actions::act(caster_id_6, state, event_queue, combat_log);
  event_queue.push(events::Event(
      1, std::make_unique<::effects::AttackIfAble>(caster_id_6, caster_id_6, action_ids::NONE, 0)));

  int c = 0;
  auto start = std::chrono::steady_clock::now();
  combat_log.addLogEvent(std::make_unique<logging::StartMatchLog>(state.time, -1));
  combat_log.addLogEvent(std::make_unique<logging::CharacterUpdateLog>(state.time, 0, state.units[0]));
  combat_log.addLogEvent(std::make_unique<logging::CharacterUpdateLog>(state.time, 1, state.units[1]));
  combat_log.addLogEvent(std::make_unique<logging::CharacterUpdateLog>(state.time, 2, state.units[2]));
  combat_log.addLogEvent(std::make_unique<logging::CharacterUpdateLog>(state.time, 3, state.units[3]));
  combat_log.addLogEvent(std::make_unique<logging::CharacterUpdateLog>(state.time, 4, state.units[4]));
  combat_log.addLogEvent(std::make_unique<logging::CharacterUpdateLog>(state.time, 5, state.units[5]));
  while (state.time < k_time_sec && !event_queue.empty() && state.victor == std::nullopt)
  {
    c++;
    auto& event = event_queue.top();
    event.resolve(event_queue, state, combat_log);
    event_queue.pop();
  }
  combat_log.print();
  auto end = std::chrono::steady_clock::now();
  auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

  if (state.victor)
    std::cout << "Victor: " << *state.victor << "\n";
  std::cout << "Total duration: " << dur / 1000000.0 << "ms, Speedup x"
            << state.time / static_cast<double>(dur) * 1000000000.0 << "\n";
  std::cout << "Total events: " << c << ", Speedup x" << c / static_cast<double>(dur) * 1000000000.0 << "\n";

  g_final_state_arr[0] = state;
  return combat_log;
};

struct Playback
{
  std::thread worker;
  std::atomic<bool> stop;
  logging::CombatLog* log;
  std::condition_variable cv;
  std::mutex cv_m;

  ~Playback()
  {
    stop = true;
    cv.notify_all();
    if (worker.joinable())
      worker.join();
  }

  void playback(
      logging::CombatLog* _log,
      std::function<void(logging::LogEvent*)> f = [](logging::LogEvent*) { return; })
  {
    stop = true;
    cv.notify_all();
    if (worker.joinable())
      worker.join();
    stop = false;
    log = _log;

    worker = std::thread(
        [this, f = f]
        {
          std::unique_lock<std::mutex> lk(cv_m);
          double t = 0;
          for (size_t i = 0; i < this->log->log.size(); i++)
          {
            if (this->stop)
              return;
            const std::chrono::duration<double> wait_time(this->log->log[i]->time - t);
            t = this->log->log[i]->time;
            cv.wait_for(lk, wait_time, [&] { return (this->stop == true); });
            // std::this_thread::sleep_for(wait_time);
            f(this->log->log[i].get());
            this->log->log[i]->print();
          }
          lk.unlock();
        });
  }
};
}  // namespace runner
