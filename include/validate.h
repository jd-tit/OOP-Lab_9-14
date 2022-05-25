//
// Created by jdt on 2022-03-31.
//

#ifndef LAB5_CONTRACT_DE_STUDII_VALIDATE_H
#define LAB5_CONTRACT_DE_STUDII_VALIDATE_H

#include <string>

#include "domain.h"

bool is_valid_string(const std::string &name);
bool is_valid_hours_per_week(unsigned num);
void validate_course(const Course& c);

#endif // LAB5_CONTRACT_DE_STUDII_VALIDATE_H
