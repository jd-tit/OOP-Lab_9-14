#include "../include/user_interface.h"
#include "../include/test.h"

#include <QApplication>

ContractController UserInterface::ctrl("repo.csv");


int main(int argc, char** argv) {
    new QApplication(argc, argv);
    ContractController::init();

    run_all_tests();
//    UserInterface ui;

    return UserInterface::execute();
}