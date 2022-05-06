//
// Created by jdt on 3/26/2022.
//

#include <fstream>

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

std::string Course::to_csv() const {
    std::stringstream ss;
    ss<< id << "," << name << "," << type << "," << teacher << "," << hours_per_week;
    return ss.str();
}

std::unique_ptr<Course> Course::from_csv(const std::string& csv) {
    std::stringstream  ss;
    ss << csv;
    std::string id_t, name_tmp, type_tmp, teacher_tmp, hpw_tmp;

    std::getline(ss, id_t, ',');
    std::getline(ss, name_tmp, ',');
    std::getline(ss, type_tmp, ',');
    std::getline(ss, teacher_tmp, ',');
    std::getline(ss, hpw_tmp, ',');

    auto res = std::make_unique<Course>(name_tmp, type_tmp, teacher_tmp, stoi(hpw_tmp), stoi(id_t));
    return res;
}
