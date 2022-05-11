#include "user_interface.h"
#include "test.h"

#include <QApplication>

ContractController UserInterface::ctrl("repo.csv");
QHBoxLayout *MainWindow::mainContainer_p;
QVBoxLayout *MainWindow::leftSide_p;
QVBoxLayout *MainWindow::rightSide_p;
std::vector<QPushButton*> MainWindow::dyn_buttons;
//QWidget* MainWindow::dyn_buttons_group;
//QVBoxLayout* MainWindow::dyn_buttons_group_layout;


int main(int argc, char** argv) {
    auto* app = new QApplication(argc, argv);
    ContractController::init();

    run_all_tests();
    UserInterface ui;
    ui.command_loop();
    return 0;
}