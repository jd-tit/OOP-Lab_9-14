//
// Created by jdt on 3/27/2022.
//

#ifndef LAB5_CONTRACT_DE_STUDII_USER_INTERFACE_H
#define LAB5_CONTRACT_DE_STUDII_USER_INTERFACE_H

#include <iostream>
#include <string>

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QMessageBox>

#include "controller.h"

class UserInterface {
private:
    /**
     * Show a message and then read from the console.
     * @tparam T
     * @param elem
     * @param message
     */
    template<typename T>
    static void read_w_message(T &elem, const char message[]);

    /**
     * Print an error message to the console.
     * @param message
     */
    static void print_error(const char message[]);

public:
    static ContractController ctrl;
    //UserInterface() : ctrl(ContractController()) {
        ctrl.fill_repo_from_file();
    }

    /**
     * The main command loop
     */
    void command_loop();

    /**
     * Print the help menu
     */
    static void print_help_menu();

    /**
     * Exit the program.
     */
    static void exit_gracefully();

    /**
     * Start dialogue to add a course.
     */
    static void addCourse(std::string name, std::string teacher, std::string type, std::string hours_string);

    /**
     * List all of the courses.
     */
    void listAll();

    /**
     *  Start dialogue to modify a course
     */
    void modifyCourse();

    /**
     * Start dialogue to delete a course;
     */
    void deleteCourse();

    /**
     *  Start dialogue to find a course by its name
     */
    void findCourseByName();

    /**
     *  Start dialogue to find courses by their number of hours per week.
     */
    void filterByHPW();

    /**
     *  Start dialogue to find courses by their teacher.
     */
    void filterByTeacher();

    /**
     *  Start dialogue to show courses sorted by a given criterion.
     */
    void getSortedBy();

    /**
     * Start dialogue to add a course to the current contract
     */
    void addCourseToContract();

    /**
     * User comand to clear the contract.
     */
    void clearContract();


    /**
     *  Start dialogue to export the current contract.
     */
    void exportContract();


    /**
     * Start dialogue to generate a random contract
     */
    void generateContract();

    /**
     * Print a message saying how many courses there are in the contract.
     */
    void printContractCourseCount();

    /*
     * Print a report about how many courses of each type there are in the database.
     * */
    void typeReport();

    /** Undo the last add, modify, or delete action. */
    void undoLast();
};

class MainWindow : public QWidget {
public:
    MainWindow() {
        this->setWindowTitle("Editor de contracte de studii");

        auto *mainContainer = new QHBoxLayout;
        auto *leftSide = new QVBoxLayout;
        auto *rightSide = new QVBoxLayout;
        auto *courseList = new QListWidget;

        auto *nameInput = new QLineEdit;
        auto *teacherInput = new QLineEdit;
        auto *typeInput = new QLineEdit;
        auto *hpwInput = new QLineEdit;

        auto* idLabel = new QLabel("ID (only for retrieval):");
        auto* nameLabel = new QLabel("Name:");
        auto* teacherLabel = new QLabel("Teacher:");
        auto* typeLabel = new QLabel("Type:");
        auto* hpwLabel = new QLabel("Hours per week:");

        leftSide->addWidget(courseList);

        rightSide->addWidget(nameLabel);
        rightSide->addWidget(nameInput);

        rightSide->addWidget(teacherLabel);
        rightSide->addWidget(teacherInput);

        rightSide->addWidget(typeLabel);
        rightSide->addWidget(typeInput);

        rightSide->addWidget(hpwLabel);
        rightSide->addWidget(hpwInput);


        // RIGHT SIDE BUTTONS
        auto* buttonBox = new QGroupBox;
        auto* addButton = new QPushButton("Add");
        auto* modButton = new QPushButton("Modify");
        auto* deleteButton = new QPushButton("Delete");

        auto* buttonLayout = new QHBoxLayout;

        buttonLayout->addWidget(addButton);
        buttonLayout->addWidget(modButton);
        buttonLayout->addWidget(deleteButton);

        buttonBox->setLayout(buttonLayout);
        rightSide->addWidget(buttonBox);

        mainContainer->addLayout(leftSide);
        mainContainer->addLayout(rightSide);

        this->setLayout(mainContainer);


        // ADD BUTTON ACTIONS
        connect(addButton, &QPushButton::clicked, this, [
                nameInput, teacherInput, typeInput, hpwInput, courseList
                ] () {
            UserInterface::addCourse(nameInput->text().toStdString(), teacherInput->text().toStdString(),
                                     typeInput->text().toStdString(), hpwInput->text().toStdString());

            courseList->clear();
            for(auto& e : UserInterface::ctrl.getAll()) {
                auto* e_item = new QListWidgetItem;
                e_item->setText(QString::fromStdString(e.to_str()));
                courseList->addItem(e_item);
            }
        });

    }
};

#endif //LAB5_CONTRACT_DE_STUDII_USER_INTERFACE_H
