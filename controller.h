//
// Created by jdt on 3/27/2022.
//

#ifndef LAB5_CONTRACT_DE_STUDII_CONTROLLER_H
#define LAB5_CONTRACT_DE_STUDII_CONTROLLER_H

#include <string>
#include <memory>

#include "domain.h"
#include "repo.h"

class ContractController {
private:
  Repo<Course> i_repo;
public:
  ContractController() : i_repo(Repo<Course>()){}

  /**
   * Add a course to the internal repository.
   * @param name
   * @param teacher
   * @param type
   * @param hours_per_week
   */
  void add_course(const std::string& name, const std::string& teacher,
                         const std::string& type,
                         const std::string& hours_per_week);

  /**
   * Get const-restricted acces to the internal repository.
   * @return
   */
  [[nodiscard]] const Repo<Course> & getAll();


/**
 * Remove a course from the internal repo.
 * @param id_buff Buffer containing the id of the course to be removed.
 */
  void remove_course(const std::string& id_buff);

  /**
   * Modify a course.
   * @param id_buff Buffer containing the id of the course to be edited.
   * @param name The new name
   * @param teacher The new Teacher's name
   * @param type The new type of the course
   * @param hours_per_week The new number of hours per week.
   */
  void modify_course(const std::string& id_buff, const std::string& name,
                     const std::string& teacher,
                     const std::string& type,
                     const std::string& hours_per_week);

  /**
   * Get the course matching a given course name.
   * @param name
   * @return
   */
  const Course &getCourse(const std::string& name);

  /**
   * Get all courses that have a number of hours per week in the range [start, stop]
   * @param start_buff Buffer containing the start number
   * @param stop_buff Buffer containing the stop number.
   * @return
   */
  std::unique_ptr<Vector<Course>>
  getCoursesMatchingHPW(const std::string& start_buff, const std::string& stop_buff);

/**
 * Get all courses that have a given teacher.
 * @param teacher_name
 * @return
 */
  std::unique_ptr<Vector<Course>>
  getCoursesMatchingTeacher(const std::string& teacher_name);

  /**
   * Get all of the courses, sorted by a given criterion
   * @param criterion The criterion to sort by.
   * @return A copy of the internal repository, sorted accordingly.
   */
  std::unique_ptr<Vector<Course>> getSortedBy(const std::string& criterion);
};

#endif //LAB5_CONTRACT_DE_STUDII_CONTROLLER_H
