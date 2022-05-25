//
// Created by jdt on 11/05/22.
//

#ifndef LAB9_QT_MAIN_WINDOW_HPP
#define LAB9_QT_MAIN_WINDOW_HPP

#include <QtWidgets>

#include "domain.h"
#include "user_interface.h"

class MainProjectWindow : public QWidget {
    Q_OBJECT
private:
    QHBoxLayout *mainContainer_p;
    QVBoxLayout *leftSide_p;
    QVBoxLayout *rightSide_p;
    QVBoxLayout *dynamicButtonsLayout;
    QGroupBox* dynamicButtonsBox;
    QComboBox* primarySortDropdown;
    QComboBox* secondarySortDropdown;
    QLineEdit* hpwStartInput;
    QLineEdit* hpwStopInput;
    QLineEdit* teacherFilterInput;
    QListWidget* courseList;

    unsigned char courseGetterSelector;
    enum courseGetters{DEFAULT, HPW, TEACHER, SORTED};

signals:
    void widgetClosed();
public:
    void closeEvent( QCloseEvent* event ) override
    {
        emit widgetClosed();
        event->accept();
    }


    QString current_csv_course;

    void show_all();
    void add_type_buttons();
    std::string get_sort_criterion();

    MainProjectWindow();
};

#endif //LAB9_QT_MAIN_WINDOW_HPP
