//
// Created by jdt on 12/05/22.
//

#ifndef LAB9_QT_CONTRACT_WINDOW_H
#define LAB9_QT_CONTRACT_WINDOW_H

#include <QtWidgets>

class ContractWindow : public QWidget {
    Q_OBJECT

private:
    QTableWidget* listing;
    MainProjectWindow* parentWindow;
    QVBoxLayout* leftSide;

public:
    explicit ContractWindow(MainProjectWindow* parentWindow);

    void show_all();
};


#endif //LAB9_QT_CONTRACT_WINDOW_H
