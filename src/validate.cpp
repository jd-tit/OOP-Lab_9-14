//
// Created by jdt on 2022-03-31.
//

#include "validate.h"
#include "domain.h"

bool is_valid_string(const std::string &name) {
    for(auto c : name){
      if(std::isalpha(c) ||  c == '\'' || c == ' '){
        continue;
      }
      return false;
    }
    if(name.length() < 2)
      return false;
    return true;
 }
bool is_valid_hours_per_week(unsigned int num) {
  constexpr unsigned MAX_HOURS_PER_WEEK = 24 * 7 / 4;
  if(num == 0 || num > MAX_HOURS_PER_WEEK)
    return false;
  return true;
}

void validate_course(const Course& c){
    if(!is_valid_hours_per_week(c.getHPW())){
        throw(std::invalid_argument("Invalid number of hours per week."));
    }
    if(!is_valid_string(c.get_name())){
        throw(std::invalid_argument("Invalid course name."));
    }
    if(!is_valid_string(c.get_teacher())){
        throw(std::invalid_argument("Invalid teacher name."));
    }
    if(!is_valid_string(c.get_type())){
        throw(std::invalid_argument("Invalid course type."));
    }
}
