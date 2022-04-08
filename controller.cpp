//
// Created by jdt on 3/27/2022.
//

#include "Vector.h"
#include <functional>
#include "controller.h"
#include "domain.h"
#include "validate.h"
#include "exception.h"

void quicksort(Vector<Course>& list, int l, int r, const std::function<bool (const Course& first, const Course& second)>& cmp);

void ContractController::add_course(const std::string& name, const std::string& teacher,
                                    const std::string& type,
                                    const std::string& hours_per_week)
{
  unsigned numerical_hpw;

  if(!is_valid_string(name)){
    throw(NameError("Error: Invalid name."));
  }

  if(!is_valid_string(teacher)){
    throw(NameError("Error: Invalid teacher name."));
  }

  if(!is_valid_string(type)){
    throw(CourseTypeError("Error: Invalid course type"));
  }

  try {
    numerical_hpw = std::stoi(hours_per_week);
  }
  catch (std::invalid_argument& e){
    throw(HoursPerWeekError("Error: Input is not a number."));
  }
  if(!is_valid_hours_per_week(numerical_hpw)){
    throw(HoursPerWeekError("Invalid number of hours per week."));
  }

  try{
    getCourse(name);
  }
  catch (std::out_of_range& e){
    Course course{name, type, teacher, numerical_hpw, i_repo.next_free_id};
    i_repo.add(course);
    return;
  }
  throw(std::invalid_argument("Error: Duplicate name."));
}
const Repo<Course>& ContractController::getAll(){
  return i_repo;
}
void ContractController::remove_course(const std::string& id_buff) {
  size_t target_id = std::stoi(id_buff);
  i_repo.remove_by_id(target_id);
}
void ContractController::modify_course(const std::string& id_buff,
                                       const std::string &name,
                                       const std::string &teacher,
                                       const std::string &type,
                                       const std::string &hours_per_week)
{
  auto& res = i_repo.find_by_id(std::stoi(id_buff));
  auto hpw = std::stoi(hours_per_week);
  res.set_name(name);
  res.set_teacher(teacher);
  res.set_type(type);
  res.set_hpw(hpw);
}
const Course& ContractController::getCourse(const std::string &name) {
  auto result = std::find_if(i_repo.begin(), i_repo.end(),
               [name](Course& e){return e.get_name() == name;}
 );

  if(result == i_repo.end())
    throw(std::out_of_range("Error: Name not found."));
  return *result;
}

std::unique_ptr<Vector<Course>>
ContractController::getCoursesMatchingHPW(const std::string& start_buff,
                                          const std::string& stop_buff)
{
  unsigned int start, stop;
  try{
    start = std::stoi(start_buff);
    stop = std::stoi(stop_buff);
  }
  catch(std::invalid_argument&){
    throw(std::invalid_argument("Invalid range"));
  }

  if(start > stop)
    throw(std::invalid_argument("Invalid range"));

  auto vector = std::make_unique<Vector<Course>>() ;

  for(auto & it : i_repo){
    if(it.getHPW() >= start && it.getHPW() <= stop){
      vector->push_back(it);
    }
  }
  return vector;
}
std::unique_ptr<Vector<Course>>
ContractController::getCoursesMatchingTeacher(const std::string &teacher_name) {
  auto vector = std::make_unique<Vector<Course>>() ;

  for(auto & it : i_repo){
    if(it.get_teacher() == teacher_name){
      vector->push_back(it);
    }
  }
  return vector;
}

std::unique_ptr<Vector<Course>>
ContractController::getSortedBy(const std::string &criterion) {
  std::function<bool (const Course& first, const Course& second)> cmp;
  if(criterion == "teacher"){
    cmp = [](const Course& first, const Course& second)
    {
        if(first.get_teacher() < second.get_teacher())
            return true;
        else if(first.get_teacher() == second.get_teacher())
            return first.get_type() < second.get_type();
        else
        return false;
    };
  } else if (criterion == "name") {
    cmp = [](const Course& first, const Course& second)
    {
        if(first.get_name() < second.get_name())
            return true;
        else if(first.get_name() == second.get_name())
            return first.get_type() < second.get_type();
        else
            return false;
    };
  } else if (criterion == "hours") {
    cmp = [](const Course& first, const Course& second)
    {
        return first.getHPW() < second.getHPW();

//        if(first.getHPW() < second.getHPW())
//            return true;
//        else if(first.getHPW() == second.getHPW())
//            return first.get_type() < second.get_type();
//        else
//            return false;
    };
  } else {
    throw(std::invalid_argument("Error: Invalid sort criterion."));
}

    auto result = std::make_unique<Vector<Course>>(this->i_repo.getVector());

    std::sort(result->begin(), result->end(), cmp);
//  quicksort(*result, 0, result->size()-1, cmp);
  return result;
}

int pivot(Vector<Course>& list, int l, int r, const std::function<bool (const Course& first, const Course& second)>& cmp) {
  Course p = list[r];
  int lt = l-1;
  for(int i = l; i < r; ++i){
    if(cmp(list[i], p)){
      ++lt;
      Course aux = list[lt];
      list[lt] = list[i];
      list[i] = aux;
    }
  }
  Course aux = list[r];
  list[r] = list[lt+1];
  list[lt+1] = aux;
  return lt + 1;
}

void quicksort(Vector<Course>& list, int l, int r, const std::function<bool (const Course& first, const Course& second)>& cmp){
  if(l < r){
    int p_index = pivot(list, l, r, cmp);

    quicksort(list, l, p_index - 1, cmp);
    quicksort(list, p_index + 1, r, cmp);
  }
}
