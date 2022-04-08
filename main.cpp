#include "user_interface.h"
#include "test.h"

int main() {
  run_all_tests();
  UserInterface ui;
  ui.command_loop();
//    return 0;
}