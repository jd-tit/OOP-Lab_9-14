//
// Created by jdt on 12/05/22.
//

#include "user_interface.h"
#include "contract_window.h"

ContractWindow::ContractWindow(MainProjectWindow* parentWindow) : parentWindow(parentWindow) {
    this->setWindowTitle("Contract Manager v1.0");

    auto* container = new QHBoxLayout;
    leftSide = new QVBoxLayout;
    auto* rightSide = new QVBoxLayout;
    listing = new QTableWidget;

    leftSide->addWidget(listing);
    listing->setSelectionBehavior(QAbstractItemView::SelectRows);


    auto* clearContractButton = new QPushButton("Clear contract");
    auto* exportContractButton = new QPushButton("Export contract to file");
    auto* filenameInput = new QLineEdit;
    auto* generateContractButton = new QPushButton("Generate random contracts");
    auto* generateInput = new QLineEdit;
    auto* generateVal = new QIntValidator(1, 50);
    generateInput->setValidator(generateVal);

    generateInput->setPlaceholderText("How many?");
    filenameInput->setPlaceholderText("Filename to export as (no extension)");

    rightSide->addWidget(clearContractButton);
    rightSide->addWidget(exportContractButton);
    rightSide->addWidget(filenameInput);
    rightSide->addWidget(generateContractButton);
    rightSide->addWidget(generateInput);

    container->addLayout(leftSide);
    container->addLayout(rightSide);
    this->setLayout(container);

    // CLEAR CONTRACT
    connect(clearContractButton, &QPushButton::clicked, this, [this, parentWindow](){
       UserInterface::ctrl.clear_contract();
       UserInterface::print_message("Contract cleared.");
       show_all();
       parentWindow->show_all();
    });

    // EXPORT CONTRACT
    connect(exportContractButton, &QPushButton::clicked, this, [filenameInput](){
        auto filename = filenameInput->text();
        if(filename.isEmpty()){
            UserInterface::print_error("Filename can't be empty.");
            return;
        }
        UserInterface::ctrl.export_contract_to_file(filename.toStdString());
        UserInterface::print_message("Exported successfully to file '" + filename.toStdString() + ".csv'");
    });

    // GENERATE CONTRACT
    connect(generateContractButton, &QPushButton::clicked, this, [this, generateInput, parentWindow](){
       auto count = generateInput->text().toStdString();
       if(count.empty()){
           UserInterface::print_error("Number of courses to generate can't be empty.");
           return;
       }
       UserInterface::ctrl.generate_contract(count);
        show_all();
        parentWindow->show_all();
    });

    show_all();
}

void ContractWindow::show_all() {
    auto result = UserInterface::ctrl.getContract();

    delete listing;
    listing = new QTableWidget(result->size(), 5);
    listing->setHorizontalHeaderLabels({"ID", "Course name", "Teacher", "Course type", "Hours per week"});
    leftSide->addWidget(listing);

    int i = 0;
    for(auto& e : *result) {
//        auto* e_item = new QListWidgetItem;
//        e_item->setText(QString::fromStdString(e.to_str()));
//        e_item->setData(Qt::UserRole, QString::fromStdString(e.to_csv()));
        auto* nameItem = new QTableWidgetItem(QString::fromStdString(e.get_name()));
        auto* typeItem = new QTableWidgetItem(QString::fromStdString(e.get_type()));
        auto* teacherItem = new QTableWidgetItem(QString::fromStdString(e.get_teacher()));
        auto* HPW_item = new QTableWidgetItem(QString::fromStdString(std::to_string(e.getHPW())));
        auto* ID_item = new QTableWidgetItem(QString::fromStdString(std::to_string(e.get_id())));
        listing->setItem(i, 0, ID_item);
        listing->setItem(i, 1, nameItem);
        listing->setItem(i, 2, teacherItem);
        listing->setItem(i, 3, typeItem);
        listing->setItem(i, 4, HPW_item);
//        listing->addItem(e_item);
        ++i;
    }
}