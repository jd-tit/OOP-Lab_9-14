#include "user_interface.h"
#include "test.h"

#include <QApplication>

ContractController UserInterface::ctrl;

int main(int argc, char** argv) {
    auto* app = new QApplication(argc, argv);
    ContractController::init();

    run_all_tests();
    UserInterface ui;
    ui.command_loop();
    return 0;
}