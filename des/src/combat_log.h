#pragma once

#include <unit.h>

#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <typeindex>
#include <vector>

static constexpr bool k_record = true;
static constexpr bool k_print = true;

namespace logging
{
enum class Color
{
  WHITE,
  RED,
  PURPLE,
  CYAN,
  GREEN,
  GRAY,
};

inline std::string colorMap(Color color)
{
  const std::string red("\033[0;31m");
  const std::string purple("\033[0;35m");
  const std::string cyan("\033[0;36m");
  const std::string green("\033[0;32m");
  const std::string gray("\e[38;5;250m");
  const std::string reset("\033[0m");
  if (color == Color::WHITE)
    return reset;
  else if (color == Color::RED)
    return red;
  else if (color == Color::CYAN)
    return cyan;
  else if (color == Color::PURPLE)
    return purple;
  else if (color == Color::GREEN)
    return green;
  else if (color == Color::GRAY)
    return gray;
  return reset;
}

struct LogEvent
{
  LogEvent(double _time, int _owner, const std::string& _note, Color _color, std::type_index _tid)
      : time(_time), owner(_owner), note(_note), color(_color), tid(_tid)
  {
  }
  LogEvent() = delete;

  virtual ~LogEvent() = default;

  virtual void printBase(const std::string& str)
  {
    if (this->owner > -1)
      std::cout << std::fixed << std::setprecision(1) << std::setw(5) << std::right << this->time << ": "
                << colorMap(this->color) << "Unit " << this->owner << " " << str << colorMap(Color::WHITE)
                << "\n";
    else
      std::cout << std::fixed << std::setprecision(1) << std::setw(5) << std::right << this->time << ": "
                << colorMap(this->color) << str << colorMap(Color::WHITE) << "\n";
  }

  virtual void print() = 0;

  double time;
  int owner;
  std::string note;
  Color color;
  std::type_index tid;
};

struct MoveLog : public LogEvent
{
  MoveLog(double _time, int _owner, const unit::MoveDetails _move_details)
      : LogEvent(_time, _owner, "", Color::GRAY, std::type_index(typeid(MoveLog))),
        move_details(_move_details)
  {
  }

  void print() override
  {
    printBase("moves to target: " + std::to_string(move_details.target_id) +
              ", at: " + std::to_string(move_details.end_x));
  }

  const unit::MoveDetails move_details;
};

struct AttackLog : public LogEvent
{
  AttackLog(double _time, int _owner, int _target, int _damage, bool _crit)
      : LogEvent(_time, _owner, "", Color::GRAY, std::type_index(typeid(AttackLog))),
        target(_target),
        damage(_damage),
        crit(_crit)
  {
  }

  void print() override
  {
    if (crit)
      printBase("attacks target: " + std::to_string(target) + " for *" + std::to_string(damage) + " damage.");
    else
      printBase("attacks target: " + std::to_string(target) + " for " + std::to_string(damage) + " damage.");
  }

  const int target;
  const int damage;
  const bool crit;
};

struct FaintLog : public LogEvent
{
  FaintLog(double _time, int _owner)
      : LogEvent(_time, _owner, "", Color::RED, std::type_index(typeid(FaintLog)))
  {
  }
  void print() override { printBase("faints."); }
};

struct TookDamageLog : public LogEvent
{
  TookDamageLog(double _time, int _owner, int _damage, int _remaining_hp, int _max_hp)
      : LogEvent(_time, _owner, "", Color::WHITE, std::type_index(typeid(TookDamageLog))),
        damage(_damage),
        remaining_hp(_remaining_hp),
        max_hp(_max_hp)
  {
  }
  void print() override
  {
    printBase("took " + std::to_string(damage) + " damage. Remaining hp: " + std::to_string(remaining_hp) +
              "/" + std::to_string(max_hp));
  }

  const int damage;
  const int remaining_hp;
  const int max_hp;
};

struct EndMatchLog : public LogEvent
{
  EndMatchLog(double _time, int /*_owner*/, int _winner)
      : LogEvent(_time, -1, "", Color::GREEN, std::type_index(typeid(EndMatchLog))), winner(_winner)
  {
  }
  void print() override { printBase(std::to_string(winner) + " wins."); }

  const int winner;
};

struct StartMatchLog : public LogEvent
{
  StartMatchLog(double _time, int /*_owner*/)
      : LogEvent(_time, -1, "", Color::GREEN, std::type_index(typeid(StartMatchLog)))
  {
  }
  void print() override { printBase("Starting Match."); }
};

struct CharacterUpdateLog : public LogEvent
{
  CharacterUpdateLog(double _time, int _owner, const unit::Unit _unit)
      : LogEvent(_time, _owner, "", Color::GREEN, std::type_index(typeid(CharacterUpdateLog))), unit(_unit)
  {
  }
  void print() override { printBase("Updating character info."); }

  const unit::Unit unit;
};

struct CombatLog
{
  std::vector<std::unique_ptr<LogEvent>> log;

  void addLogEvent(std::unique_ptr<LogEvent> log_event)
  {
    if constexpr (k_record)
      log.push_back(std::move(log_event));
  }

  void print()
  {
    if constexpr (k_print)
      for (const auto& e : log)
        e->print();
  }
};
}  // namespace logging
