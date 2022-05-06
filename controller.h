//
// Created by jdt on 3/27/2022.
//

#ifndef LAB5_CONTRACT_DE_STUDII_CONTROLLER_H
#define LAB5_CONTRACT_DE_STUDII_CONTROLLER_H

#include <string>
#include <memory>
#include <fstream>
#include <random>
#include <unordered_map>
#include <utility>

#include "domain.h"
#include "repo.h"
#include "contract.h"
#include "undo_action.hpp"

class ContractController {
private:
    Repo<Course> i_repo;
    Contract contract;
    std::vector<std::unique_ptr<UndoAction>> undoList;

    static std::vector<std::string> people_names, course_names, course_types;
    static std::default_random_engine rng;

    static std::string select_randomly(std::vector<std::string>::iterator start, const std::vector<std::string>::iterator end){
        auto dist = std::distance(start, end)-1;
        std::uniform_int_distribution<> uni_dist(0, static_cast<int>(dist));
        auto val= uni_dist(rng);
        std::advance(start, val);
        return *start;
    }

public:
    explicit ContractController(std::string repo_filename = "test_data.csv") : i_repo(Repo<Course>(std::move(repo_filename))), contract{Contract()} {}

    /**
     * Initialize static members of ContractController
     */
    static void init(){
        std::fstream names("names.txt", std::ios::in);
        std::fstream courses("course_names.txt", std::ios::in);

        if(!names.is_open())
            throw(std::invalid_argument("Error: File 'names.txt' couldn't be found."));

        if(!courses.is_open())
            throw(std::invalid_argument("Error: File 'course_names.txt' couldn't be found."));

        while(names.good()){
            std::string line;
            std::getline(names, line);
            people_names.emplace_back(line);
        }

        while(courses.good()){
            std::string line;
            std::getline(courses, line);
            erase_if(line, [](char c){return c == '\r';});
            course_names.emplace_back(line);
        }

        course_types = {"Mandatory", "Optional"};
    }

    /**
     * Add a course to the internal repository.
     * @param name
     * @param teacher
     * @param type
     * @param hours_per_week
     */
    void add_course(const std::string &name, const std::string &teacher,
                    const std::string &type,
                    const std::string &hours_per_week);

    /**
     * Get const-restricted acces to the internal repository.
     * @return
     */
    [[nodiscard]] const Repo<Course> &getAll();


/**
 * Remove a course from the internal repo.
 * @param id_buff Buffer containing the id of the course to be removed.
 */
    void remove_course(const std::string &id_buff);

    /**
     * Modify a course.
     * @param id_buff Buffer containing the id of the course to be edited.
     * @param name The new name
     * @param teacher The new Teacher's name
     * @param type The new type of the course
     * @param hours_per_week The new number of hours per week.
     */
    void modify_course(const std::string &id_buff, const Course& modified_course);

    /**
     * Get the course matching a given course name.
     * @param name
     * @return
     */
    const Course &getCourse(const std::string &name);

    /**
     * Get all courses that have a number of hours per week in the range [start, stop]
     * @param start_buff Buffer containing the start number
     * @param stop_buff Buffer containing the stop number.
     * @return
     */
    std::unique_ptr<std::vector<Course>>
    getCoursesMatchingHPW(const std::string &start_buff, const std::string &stop_buff);

/**
 * Get all courses that have a given teacher.
 * @param teacher_name
 * @return
 */
    std::unique_ptr<std::vector<Course>>
    getCoursesMatchingTeacher(const std::string &teacher_name);

    /**
     * Get all of the courses, sorted by a given criterion
     * @param criterion The criterion to sort by.
     * @return A copy of the internal repository, sorted accordingly.
     */
    std::unique_ptr<std::vector<Course>> getSortedBy(const std::string &criterion);

    /**
     * Add a course to the contract, by name.
     * @param name The name of the course to be added
     */
    void add_course_to_contract(const std::string &name);

    /**
     * Clear the student contract.
     */
    void clear_contract();

    /**
     * Export the current contract to a file.
     */
    void export_contract_to_file(const std::string& filename);

    /**
     * Generate `count` random courses and add them to the contract.
     * @param count_buff
     */
    void generate_contract(const std::string& count_buff);


    /**
     * Get the number of courses in the current contract
     */
    size_t get_number_of_courses_in_contract();

    /**
     * Get a map linking course types to the number of courses of that type.
     * @return
     */
    std::unique_ptr<std::unordered_map<std::string, size_t>> getCourseTypeData();

    /**
     * Undo last add, remove, or modify action.
     */
    void undo_last();

    /**
     * Get Courses from file and add them to the repo
     */
    void fill_repo_from_file();
};

#endif //LAB5_CONTRACT_DE_STUDII_CONTROLLER_H
