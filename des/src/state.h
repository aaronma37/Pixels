#pragma once

#include <policy_ids.h>
#include <unit.h>

#include <iostream>
#include <list>
#include <string>
#include <unordered_map>

namespace state
{
struct State
{
  std::unordered_map<int, unit::Unit> units;
  std::unordered_map<int, std::list<AuraI>> auras;
  std::unordered_map<int, std::list<DebuffI>> debuffs;
  double time = 0;

  std::optional<int> victor = std::nullopt;
};

inline std::ostream& operator<<(std::ostream& os, const state::State& state)
{
  os << "\n---Summary---\n";
  return os;
}
}  // namespace state
