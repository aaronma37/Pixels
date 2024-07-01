#include <actions.h>
#include <combat_log.h>
#include <effect_interface.h>
#include <effects.h>
#include <event.h>
#include <fwds.h>
#include <runner.h>
#include <state.h>
#include <unit.h>
#include <utils.h>

#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <functional>
#include <memory>
#include <omp.h>
#include <queue>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

logging::CombatLog current_log;
runner::Playback playback;
int main(int argc, char** argv)
{
  std::srand(1);
#pragma omp parallel num_threads(k_iters)
  {
    current_log = runner::runSim();
    playback.playback(&current_log);
    playback.worker.join();
  }
  return 0;
}
