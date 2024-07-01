#pragma once

#include <optional>
#include <unordered_map>
#include <vector>

namespace specialization_types
{
static constexpr int AFFLICTION = 1;
static constexpr int DESTRUCTION = 2;
static constexpr int HOLY = 3;
}  // namespace specialization_types

namespace spell_schools
{
static constexpr int FIRE = 1;
static constexpr int SHADOW = 2;
static constexpr int FROST = 3;
static constexpr int HOLY = 4;
static constexpr int NATURE = 5;
static constexpr int ARCANE = 6;
}  // namespace spell_schools

namespace aura_types
{
static constexpr int ON_HIT = 1;
}  // namespace aura_types

namespace debuff_types
{
static constexpr int ON_HIT = 1;
}  // namespace debuff_types

struct AuraI
{
};

struct DebuffI
{
};

namespace unit
{
struct AttackSlotData
{
  int slot = 0;
  int attack_dur = 1.0;
  int dmg_min = 10.0;
  int dmg_max = 10.0;
  double range = 1.0;
  double attack_cd = 1.0;
};

struct MoveDetails
{
  MoveDetails(double _start_x, double _end_x, double _start_t, double _end_t, int _target_id)
      : start_x(_start_x), end_x(_end_x), start_t(_start_t), end_t(_end_t), target_id(_target_id)
  {
  }
  double start_x = 0;
  double end_x = 0;
  double start_t = 0;
  double end_t = 0;
  int target_id = -1;
};

struct Unit
{
  int faction = 0;
  int hp = 0;
  int max_hp = 0;
  double x = 0;
  int alive = 1;
  int attack_speed_mod = 1;
  int cast_speed_mod = 1;
  int spell_power = 0;
  double movespeed = 1.0;
  double gcd = 0;
  std::optional<MoveDetails> move_details = std::nullopt;
  std::unordered_map<int, int> school_spell_power{
      {spell_schools::FIRE, 0},
      {spell_schools::FROST, 0},
      {spell_schools::HOLY, 0},
      {spell_schools::SHADOW, 0},
      {spell_schools::NATURE, 0},
      {spell_schools::ARCANE, 0},
  };
  std::vector<int> allies;
  std::vector<int> enemies;
  std::unordered_map<int, double> threat;
  double current_max_threat = 0;
  std::optional<int> current_hostile_target = std::nullopt;
  std::unordered_map<int, AttackSlotData> attack_slot_data;
  double spell_crit_chance = 1.05;
  double spell_crit_mod = 1.5;
  double spell_hit_chance = 0;
  double physical_crit_chance = 0;
  double physical_hit_chance = 0;

  std::vector<AuraI> auras;
  std::vector<DebuffI> debuffs;

  std::vector<int> action_space;
};

}  // namespace unit
