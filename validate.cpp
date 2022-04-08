//
// Created by jdt on 2022-03-31.
//

#include "validate.h"
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
