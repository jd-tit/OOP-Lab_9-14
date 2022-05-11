//
// Created by jdt on 3/27/2022.
//

#ifndef LAB5_CONTRACT_DE_STUDII_USER_INTERFACE_H
#define LAB5_CONTRACT_DE_STUDII_USER_INTERFACE_H

#include <iostream>
#include <string>

#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QMessageBox>
#include <QPointer>
#include <QButtonGroup>

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


public:
    static void print_error(const char message[]);
    static ContractController ctrl;

    UserInterface() {
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
    static void modifyCourse(Course modded);

    /**
     * Start dialogue to delete a course;
     */
    static void deleteCourse(std::string id_buff);

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

    void typeReportGUI(const Course &crt);
};

class MainWindow : public QWidget {
private:
    static QHBoxLayout *mainContainer_p;
    static QVBoxLayout *leftSide_p;
    static QVBoxLayout *rightSide_p;
    static QButtonGroup* dyn_buttons_group;

    static std::vector<QPushButton*> dyn_buttons;

public:
    QString current_csv_course;

    static void show_all(QListWidget* courseList){
        courseList->clear();
        for(auto& e : UserInterface::ctrl.getAll()) {
            auto* e_item = new QListWidgetItem;
            e_item->setText(QString::fromStdString(e.to_str()));
            e_item->setData(Qt::UserRole, QString::fromStdString(e.to_csv()));
            courseList->addItem(e_item);
        }
        add_type_buttons();
    }

    static void add_type_buttons(){
        auto type_data = UserInterface::ctrl.getCourseTypeData();

//        auto kids = dyn_buttons_group->findChildren<QPushButton*>();
//        for(auto e : kids){
//            e->deleteLater();
//        }

        for(int i = dyn_buttons.size(); i > 0; --i){
            auto* e = dyn_buttons.back();
            dyn_buttons.pop_back();
            delete e;
        }

        for(const auto& it : *type_data){
            auto* button = new QPushButton(QString::fromStdString(it.first));
            dyn_buttons.push_back(button);
            mainContainer_p->addWidget(button);

            connect(button, &QPushButton::clicked, button, [it] () {
                UserInterface::print_error(std::to_string(it.second).c_str());

            });
        }
    }

    MainWindow() {
        this->setWindowTitle("Editor de contracte de studii");

        auto* mainContainer = new QHBoxLayout;
        auto* leftSide = new QVBoxLayout;
        auto* rightSide = new QVBoxLayout;
        auto *courseList = new QListWidget;

        mainContainer_p = mainContainer;
        rightSide_p = rightSide;
        leftSide_p = leftSide;
//        dyn_buttons_group = new QButtonGroup;

        auto *nameInput = new QLineEdit;
        auto *teacherInput = new QLineEdit;
        auto *typeInput = new QLineEdit;
        auto *hpwInput = new QLineEdit;

        auto* nameLabel = new QLabel("Name:");
        auto* teacherLabel = new QLabel("Teacher:");
        auto* typeLabel = new QLabel("Type:");
        auto* hpwLabel = new QLabel("Hours per week:");

        courseList->setSelectionMode(QAbstractItemView::SingleSelection);


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

        show_all(courseList);
        // ADD BUTTON ACTIONS
        connect(addButton, &QPushButton::clicked, this, [
                nameInput, teacherInput, typeInput, hpwInput, courseList
                ] () {
            UserInterface::addCourse(nameInput->text().toStdString(), teacherInput->text().toStdString(),
                                     typeInput->text().toStdString(), hpwInput->text().toStdString());
            MainWindow::show_all(courseList);
        });


        connect(courseList, &QListWidget::currentItemChanged, courseList, [this] (QListWidgetItem *current, QListWidgetItem *previous) {
            if(current != nullptr)
                this->current_csv_course = current->data(Qt::UserRole).toString();
            else this->current_csv_course = "";
        });

        connect(modButton, &QPushButton::clicked, this, [
                nameInput, teacherInput, typeInput, hpwInput, courseList, this
                ] () {
            if(this->current_csv_course.isEmpty())
                return;
            auto old = Course::from_csv(this->current_csv_course.toStdString());
            Course c(nameInput->text().toStdString(), typeInput->text().toStdString(),
                   teacherInput->text().toStdString(), std::stoi(hpwInput->text().toStdString()), old->get_id());
            UserInterface::modifyCourse(c);
            MainWindow::show_all(courseList);
        });

        connect(deleteButton, &QPushButton::clicked, this, [
                courseList, this
        ] () {
            if(this->current_csv_course.isEmpty())
                return;
            auto old = Course::from_csv(this->current_csv_course.toStdString());
            UserInterface::deleteCourse(std::to_string(old->get_id()));
            MainWindow::show_all(courseList);
        });
    }
};

#endif //LAB5_CONTRACT_DE_STUDII_USER_INTERFACE_H
