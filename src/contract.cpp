//
// Created by jdt on 4/10/2022.
//

#include <fstream>

#include "../include/contract.h"

void Contract::clear() {
    contents.clear();
}


void Contract::saveToFile(const std::string& filename) {
    std::fstream out(filename + ".csv", std::ios::out | std::ios::trunc);

    for(const auto& course : contents){
        out << course.to_csv() << "\n";
    }
}
