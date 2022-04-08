//
// Created by jdt on 3/26/2022.
//

#include <sstream>
#include "domain.h"
std::string Course::to_str() const{
  std::stringstream ss;
  ss << "ID: " << id << "; Name: " << name << "; Type: " << type << "; Teacher: " << teacher << "; Hours per week: " << hours_per_week;
  return ss.str();
}
size_t Course::get_id() const{
  return id;
}
