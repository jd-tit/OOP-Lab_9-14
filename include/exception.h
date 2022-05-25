//
// Created by jdt on 2022-04-07.
//

#ifndef LAB5_CONTRACT_DE_STUDII_EXCEPTION_H
#define LAB5_CONTRACT_DE_STUDII_EXCEPTION_H

#include <stdexcept>

#include <stdexcept>
struct NameError: public std::invalid_argument{
  explicit NameError(const std::string& msg): std::invalid_argument(msg){}
};
struct HoursPerWeekError: public std::invalid_argument{
  explicit HoursPerWeekError(const std::string& msg): std::invalid_argument(msg){}
};
struct CourseTypeError: public std::invalid_argument{
  explicit CourseTypeError(const std::string& msg): std::invalid_argument(msg){}
};

#endif // LAB5_CONTRACT_DE_STUDII_EXCEPTION_H
