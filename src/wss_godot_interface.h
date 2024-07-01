#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <combat_log.h>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/sprite2d.hpp>

#include <memory>
#include <vector>

namespace godot
{
class GDExample2 : public Button
{
  GDCLASS(GDExample2, Button)
 private:
  double time_passed;
  double time_emit;

 protected:
  static void _bind_methods();

 public:
  GDExample2();
  ~GDExample2();
  void _process(double delta) override;

  void runSim();

  void _pressed() override;

  void handleLogEvent(logging::LogEvent*);

  void emitOwnerBattleEvent(int, const Dictionary&);
};

}  //namespace godot

#endif
