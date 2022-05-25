//
// Created by jdt on 11/05/22.
//

#include "main_window.hpp"
#include "contract_window.h"

void MainProjectWindow::show_all() {
    courseList->clear();

    std::unique_ptr<std::vector<Course>> result;
    auto start = hpwStartInput->text();
    auto stop = hpwStopInput->text();
    auto teacher = teacherFilterInput->text();
    switch(courseGetterSelector){
        case courseGetters::DEFAULT:
            result = UserInterface::ctrl.getAll();
            break;
        case courseGetters::HPW:
            result = UserInterface::ctrl.getCoursesMatchingHPW(start.toStdString(), stop.toStdString());
            break;
        case courseGetters::TEACHER:
            result = UserInterface::ctrl.getCoursesMatchingTeacher(teacher.toStdString());
            break;
        case courseGetters::SORTED:
            result = UserInterface::ctrl.getSortedBy(this->get_sort_criterion());
            break;

    }
    for(auto& e : *result) {
        auto* e_item = new QListWidgetItem;
        e_item->setText(QString::fromStdString(e.to_str()));
        e_item->setData(Qt::UserRole, QString::fromStdString(e.to_csv()));
        courseList->addItem(e_item);
    }

    add_type_buttons();
}

void MainProjectWindow::add_type_buttons() {
    auto type_data = UserInterface::ctrl.getCourseTypeData();

    delete dynamicButtonsBox;
    dynamicButtonsBox = new QGroupBox("Course type details");
    dynamicButtonsLayout = new QVBoxLayout;
    dynamicButtonsBox->setLayout(dynamicButtonsLayout);
    rightSide_p->addWidget(dynamicButtonsBox);

//    while(!dyn_buttons.empty()){
//        auto* e = dyn_buttons.back();
//        dyn_buttons.pop_back();
//        delete e;
//    }

    if(!type_data->empty()){
        dynamicButtonsBox->show();
    }

    for(const auto& it : *type_data){
        auto* button = new QPushButton(QString::fromStdString(it.first));
//        dyn_buttons.push_back(button);
        dynamicButtonsLayout->addWidget(button);

        connect(button, &QPushButton::clicked, button, [it] () {
            std::string message = "There are " + std::to_string(it.second) + " course(s) of type '" + it.first + "'";
            UserInterface::print_message(message);
        });


    }
}

MainProjectWindow::MainProjectWindow() {
    this->setWindowTitle("Editor de contracte de studii");

    auto* mainContainer = new QHBoxLayout;
    auto* leftSide = new QVBoxLayout;
    auto* rightSide = new QVBoxLayout;
    courseList = new QListWidget;

    mainContainer_p = mainContainer;
    rightSide_p = rightSide;
    leftSide_p = leftSide;


//    connect(this, &MainProjectWindow::widgetClosed, secWindow, &QLabel::close);
    courseGetterSelector = DEFAULT;

    courseList->setSelectionMode(QAbstractItemView::SingleSelection);

    auto *nameInput = new QLineEdit;
    auto *teacherInput = new QLineEdit;
    auto *typeInput = new QLineEdit;
    auto *hpwInput = new QLineEdit;

    auto* hpwValidator = new QIntValidator(2, 80);

    hpwInput->setValidator(hpwValidator);

    auto* nameLabel = new QLabel("Name:");
    auto* teacherLabel = new QLabel("Teacher:");
    auto* typeLabel = new QLabel("Type:");
    auto* hpwLabel = new QLabel("Hours per week:");

    // LEFT SIDE
    auto* sortBox = new QGroupBox("Sort");
    auto* primarySortLabel = new QLabel("Primary sort:");
    auto* secondarySortLabel = new QLabel("Secondary sort:");
    primarySortDropdown = new QComboBox;
    secondarySortDropdown = new QComboBox;
    auto* sortButton = new QPushButton("Sort");

    primarySortDropdown->addItem("Teacher");
    primarySortDropdown->addItem("Course name");
    primarySortDropdown->addItem("Hours per week");

    secondarySortDropdown->addItem("Type");


    leftSide->addWidget(courseList);
    auto* sortBoxGrid = new QGridLayout;

    sortBoxGrid->addWidget(primarySortLabel, 0, 0);
    sortBoxGrid->addWidget(primarySortDropdown, 0, 1);
    sortBoxGrid->addWidget(secondarySortLabel, 1, 0);
    sortBoxGrid->addWidget(secondarySortDropdown, 1, 1);
    sortBoxGrid->addWidget(sortButton, 2, 1);


    auto* filterBox = new QGroupBox("Filter");
    auto* filterHPW_Button = new QPushButton("Hours per week");
    auto* filterTeacher_Button = new QPushButton("Teacher");
    auto* filterBoxGrid = new QGridLayout;

    hpwStartInput = new QLineEdit;
    hpwStopInput = new QLineEdit;
    teacherFilterInput = new QLineEdit;

    hpwStartInput->setValidator(hpwValidator);
    hpwStopInput->setValidator(hpwValidator);


    filterBoxGrid->addWidget(filterHPW_Button, 0, 0);
    filterBoxGrid->addWidget(hpwStartInput, 0, 1);
    filterBoxGrid->addWidget(hpwStopInput, 0, 2);

    filterBoxGrid->addWidget(filterTeacher_Button, 1, 0);
    filterBoxGrid->addWidget(teacherFilterInput, 1, 1);

    auto* resetViewButton = new QPushButton("Reset view");

    sortBox->setLayout(sortBoxGrid);
    filterBox->setLayout(filterBoxGrid);
    leftSide->addWidget(sortBox);
    leftSide->addWidget(filterBox);
    leftSide->addWidget(resetViewButton);


    // RIGHT SIDE INPUTS
    auto* inputGridLayout = new QGridLayout;
    inputGridLayout->addWidget(nameLabel, 0, 0);
    inputGridLayout->addWidget(nameInput, 0, 1);

    inputGridLayout->addWidget(teacherLabel, 1, 0);
    inputGridLayout->addWidget(teacherInput, 1, 1);

    inputGridLayout->addWidget(typeLabel, 2, 0);
    inputGridLayout->addWidget(typeInput, 2, 1);

    inputGridLayout->addWidget(hpwLabel, 3, 0);
    inputGridLayout->addWidget(hpwInput, 3, 1);

    rightSide->addLayout(inputGridLayout);



    // RIGHT SIDE BUTTONS
    auto* buttonBox = new QGroupBox;
    auto* addButton = new QPushButton("Add");
    auto* modButton = new QPushButton("Modify");
    auto* deleteButton = new QPushButton("Delete");
    auto* undoButton = new QPushButton("Undo");
    auto* addToContractButton = new QPushButton("Add to contract");

    auto* buttonLayout = new QGridLayout;

    buttonLayout->addWidget(addButton, 0, 0);
    buttonLayout->addWidget(modButton, 0, 1);
    buttonLayout->addWidget(deleteButton, 0, 2);
    buttonLayout->addWidget(addToContractButton, 1, 0);
    buttonLayout->addWidget(undoButton, 1, 2);

    buttonBox->setLayout(buttonLayout);
    rightSide->addWidget(buttonBox);

    dynamicButtonsBox = new QGroupBox("Course type details");
    MainProjectWindow::dynamicButtonsLayout = new QVBoxLayout;
    dynamicButtonsBox->hide();
    rightSide->addWidget(dynamicButtonsBox);
    dynamicButtonsBox->setLayout(dynamicButtonsLayout);

    mainContainer->addLayout(leftSide);
    mainContainer->addLayout(rightSide);

    this->setLayout(mainContainer);

    show_all();


    auto* contractWindow = new ContractWindow(this);
    contractWindow->show();

    // ~~~~~~~~~~ ADD BUTTON ACTIONS ~~~~~~~~~~

    // ADD
    connect(addButton, &QPushButton::clicked, this, [
            nameInput, teacherInput, typeInput, hpwInput
    , this] () {
        UserInterface::addCourse(nameInput->text().toStdString(), teacherInput->text().toStdString(),
                                 typeInput->text().toStdString(), hpwInput->text().toStdString());
        this->show_all();
    });

    // ADD TO CONTRACT
    connect(addToContractButton, &QPushButton::clicked, this, [this, contractWindow] () {
        auto c = Course::from_csv(this->current_csv_course.toStdString());
        UserInterface::ctrl.add_course_to_contract(c->get_name());
        this->show_all();
        contractWindow->show_all();
    });

    // MODIFY
    connect(modButton, &QPushButton::clicked, this, [
            nameInput, teacherInput, typeInput, hpwInput, this
    ] () {
        if(this->current_csv_course.isEmpty()){
            UserInterface::print_error("No course has been selected");
            return;
        }
        if(hpwInput->text().isEmpty()){
            UserInterface::print_error("New number of hours per week can't be empty");
            return;
        }

        auto old = Course::from_csv(this->current_csv_course.toStdString());
        Course c(nameInput->text().toStdString(), typeInput->text().toStdString(),
                 teacherInput->text().toStdString(), std::stoi(hpwInput->text().toStdString()), old->get_id());
        UserInterface::modifyCourse(c);
        MainProjectWindow::show_all();
    });

    // DELETE
    connect(deleteButton, &QPushButton::clicked, this, [
            this
    ] () {
        if(this->current_csv_course.isEmpty())
            return;
        auto old = Course::from_csv(this->current_csv_course.toStdString());
        UserInterface::deleteCourse(std::to_string(old->get_id()));
        MainProjectWindow::show_all();
    });

    // UNDO
    connect(undoButton, &QPushButton::clicked, this, [this] () {
        UserInterface::undoLast();
        MainProjectWindow::show_all();
    });

    // SORT
    connect(sortButton, &QPushButton::clicked, this, [this] () {
        courseGetterSelector = courseGetters::SORTED;
        MainProjectWindow::show_all();
    });

    // ~~~~~~~ FILTER ~~~~~~~
    // FILTER BY HPW
    connect(filterHPW_Button, &QPushButton::clicked, this, [this] () {
        courseGetterSelector = courseGetters::HPW;
        MainProjectWindow::show_all();
    });

    // FILTER BY TEACHER
    connect(filterTeacher_Button, &QPushButton::clicked, this, [this] () {
        courseGetterSelector = courseGetters::TEACHER;
        MainProjectWindow::show_all();
    });

    // RESET VIEW
    connect(resetViewButton, &QPushButton::clicked, this, [this] () {
        courseGetterSelector = courseGetters::DEFAULT;
        MainProjectWindow::show_all();
    });

    // UPDATE CURRENTLY SELECTED COURSE
    connect(courseList, &QListWidget::currentItemChanged, courseList, [this] (QListWidgetItem *current, QListWidgetItem *previous) {
        if(current != nullptr)
            this->current_csv_course = current->data(Qt::UserRole).toString();
        else this->current_csv_course = "";
    });
}

std::string MainProjectWindow::get_sort_criterion() {
    auto selectedText = this->primarySortDropdown->currentText();
    if(selectedText == "Teacher"){
        return "teacher";
    } else if (selectedText == "Course name"){
        return "name";
    } else if (selectedText == "Hours per week") {
        return "hours";
    } else {
        throw(std::invalid_argument("Invalid sort criterion"));
    }
}
