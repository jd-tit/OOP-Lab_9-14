//
// Created by jdt on 3/27/2022.
//

#include <functional>
#include "controller.h"
#include "domain.h"
#include "validate.h"
#include "exception.h"

std::vector<std::string> ContractController::people_names, ContractController::course_names, ContractController::course_types;
std::default_random_engine ContractController::rng;

void quicksort(std::vector<Course> &list, int l, int r,
               const std::function<bool(const Course &first, const Course &second)> &cmp);

void ContractController::add_course(const std::string &name, const std::string &teacher,
                                    const std::string &type,
                                    const std::string &hours_per_week) {
    unsigned numerical_hpw;

    // VALIDATION
    if (!is_valid_string(name)) {
        throw (NameError("Error: Invalid name."));
    }

    if (!is_valid_string(teacher)) {
        throw (NameError("Error: Invalid teacher name."));
    }

    if (!is_valid_string(type)) {
        throw (CourseTypeError("Error: Invalid course type"));
    }

    try {
        numerical_hpw = std::stoi(hours_per_week);
    }
    catch (std::invalid_argument &e) {
        throw (HoursPerWeekError("Error: Input is not a number."));
    }
    if (!is_valid_hours_per_week(numerical_hpw)) {
        throw (HoursPerWeekError("Invalid number of hours per week."));
    }


    // ADD TO REPO
    try {
        getCourse(name);
    }
    catch (std::out_of_range &e) {
        Course course{name, type, teacher, numerical_hpw, i_repo.next_free_id};
        i_repo.add(course);

        UndoData undoData{course, i_repo.get_size(), i_repo};
        auto* new_undoAction = new UndoAdd{undoData};
        undoList.emplace_back(new_undoAction);
        i_repo.write_to_file();
        return;
    }
    throw (std::invalid_argument("Error: Duplicate name."));
}

std::unique_ptr<std::vector<Course>> ContractController::getAll() {
    return std::make_unique<std::vector<Course>>(i_repo.getVector());
}

void ContractController::remove_course(const std::string &id_buff) {
    // RECORD ACTION FOR UNDO
    size_t target_id = std::stoi(id_buff);
    auto it = i_repo.find_iterator_by_id(target_id);

    size_t index = std::distance(i_repo.begin(), it);
    UndoData undoData{*it, index, i_repo};
    auto* new_undoAction = new UndoDelete{undoData};
    undoList.emplace_back(new_undoAction);

    // REMOVE
    i_repo.remove_at(it);
    i_repo.write_to_file();
}

void ContractController::modify_course(const std::string &id_buff, const Course& modified) {
    //VALIDATE
    validate_course(modified);

    // FIND
    auto res = i_repo.find_iterator_by_id(std::stoi(id_buff));

    // RECORD FOR UNDO
    size_t index = std::distance(i_repo.begin(), res);
    UndoData undoData{*res, index, i_repo};
    auto* new_undoAction = new UndoModify{undoData};
    undoList.emplace_back(new_undoAction);

    // MODIFY
    *res = modified;
    i_repo.write_to_file();
}

const Course &ContractController::getCourse(const std::string &name) {
    auto result = std::find_if(i_repo.begin(), i_repo.end(),
                               [name](Course &e) { return e.get_name() == name; }
    );

    if (result == i_repo.end())
        throw (std::out_of_range("Error: Name not found."));
    return *result;
}

std::unique_ptr<std::vector<Course>>
ContractController::getCoursesMatchingHPW(const std::string &start_buff,
                                          const std::string &stop_buff) {
    unsigned int start, stop;
    try {
        start = std::stoi(start_buff);
        stop = std::stoi(stop_buff);
    }
    catch (std::invalid_argument &) {
        throw (std::invalid_argument("Invalid range"));
    }

    if (start > stop)
        throw (std::invalid_argument("Invalid range"));

    auto vector = std::make_unique<std::vector<Course>>();

    for (auto &it: i_repo) {
        if (it.getHPW() >= start && it.getHPW() <= stop) {
            vector->push_back(it);
        }
    }
    return vector;
}

std::unique_ptr<std::vector<Course>>
ContractController::getCoursesMatchingTeacher(const std::string &teacher_name) {
    auto vector = std::make_unique<std::vector<Course>>();

    for (auto &it: i_repo) {
        if (it.get_teacher() == teacher_name) {
            vector->push_back(it);
        }
    }
    return vector;
}

std::unique_ptr<std::vector<Course>>
ContractController::getSortedBy(const std::string &criterion) {
    std::function<bool(const Course &first, const Course &second)> cmp;
    if (criterion == "teacher") {
        cmp = [](const Course &first, const Course &second) {
            if (first.get_teacher() < second.get_teacher())
                return true;
            else if (first.get_teacher() == second.get_teacher())
                return first.get_type() < second.get_type();
            else
                return false;
        };
    } else if (criterion == "name") {
        cmp = [](const Course &first, const Course &second) {
            if (first.get_name() < second.get_name())
                return true;
            else if (first.get_name() == second.get_name())
                return first.get_type() < second.get_type();
            else
                return false;
        };
    } else if (criterion == "hours") {
        cmp = [](const Course &first, const Course &second) {
            return first.getHPW() < second.getHPW();

//        if(first.getHPW() < second.getHPW())
//            return true;
//        else if(first.getHPW() == second.getHPW())
//            return first.get_type() < second.get_type();
//        else
//            return false;
        };
    } else {
        throw (std::invalid_argument("Error: Invalid sort criterion."));
    }

    auto result = std::make_unique<std::vector<Course>>(this->i_repo.getVector());

    std::sort(result->begin(), result->end(), cmp);
//  quicksort(*result, 0, result->size()-1, cmp);
    return result;
}

void ContractController::add_course_to_contract(const std::string &name) {
    auto result = std::find_if(i_repo.cbegin(), i_repo.cend(), [name](const Course &c) {
        return c.get_name() == name;
    });

    if (result == i_repo.cend()) {
        throw (std::out_of_range("Error: Course not found."));
    }

    contract.add(*result);
}

size_t ContractController::get_number_of_courses_in_contract() {
    return contract.get_size();
}

void ContractController::clear_contract() {
    contract.clear();
}

void ContractController::export_contract_to_file(const std::string &filename) {
    contract.saveToFile(filename);
}

void ContractController::generate_contract(const std::string &count_buff) {
    size_t count;
    try {
        count = std::stoi(count_buff);
    }
    catch (std::invalid_argument &) {
        throw (std::invalid_argument("Invalid number of courses."));
    }

    std::uniform_int_distribution<> uni_dist(1, 10);
    for (size_t i = 0; i < count; ++i) {
        auto teacher = select_randomly(people_names.begin(), people_names.end());
        auto name = select_randomly(course_names.begin(), course_names.end());
        auto type = select_randomly(course_types.begin(), course_types.end());
        auto hpw = uni_dist(rng) * 2;

        Course c{name, type, teacher, static_cast<unsigned int>(hpw), i_repo.next_free_id};
        i_repo.add(c);
        contract.add(c);
    }
}

std::unique_ptr<std::unordered_map<std::basic_string<char>, unsigned long>> ContractController::getCourseTypeData() {
    auto data = std::make_unique<std::unordered_map<std::string, size_t>>();
    for(const auto& course : i_repo){
        if(data->contains(course.get_type())){
            (*data)[course.get_type()] += 1;
        } else {
            (*data)[course.get_type()] = 1;
        }
    }
    return data;
}
void ContractController::undo_last() {
    if(undoList.empty()){
        throw(std::out_of_range{"Error: Nothing to undo."});
    }

    auto undo_action = std::move(undoList.back());
    undoList.pop_back();
    undo_action->doUndo();
    i_repo.write_to_file();
}
void ContractController::fill_repo_from_file() {
    i_repo.read_from_file();
}

std::unique_ptr<std::vector<Course>> ContractController::getContract() {
    return std::make_unique<std::vector<Course>>(contract.getVector());
}

//int pivot(std::vector<Course> &list, int l, int r,
//          const std::function<bool(const Course &first, const Course &second)> &cmp) {
//    Course p = list[r];
//    int lt = l - 1;
//    for (int i = l; i < r; ++i) {
//        if (cmp(list[i], p)) {
//            ++lt;
//            Course aux = list[lt];
//            list[lt] = list[i];
//            list[i] = aux;
//        }
//    }
//    Course aux = list[r];
//    list[r] = list[lt + 1];
//    list[lt + 1] = aux;
//    return lt + 1;
//}
//
//void quicksort(std::vector<Course> &list, int l, int r,
//               const std::function<bool(const Course &first, const Course &second)> &cmp) {
//    if (l < r) {
//        int p_index = pivot(list, l, r, cmp);
//
//        quicksort(list, l, p_index - 1, cmp);
//        quicksort(list, p_index + 1, r, cmp);
//    }
//}
