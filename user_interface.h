//
// Created by jdt on 3/27/2022.
//

#ifndef LAB5_CONTRACT_DE_STUDII_USER_INTERFACE_H
#define LAB5_CONTRACT_DE_STUDII_USER_INTERFACE_H

#include <iostream>
#include <string>


#include "controller.h"

class UserInterface{
private:
  ContractController ctrl;

  /**
   * Show a message and then read from the console.
   * @tparam T
   * @param elem
   * @param message
   */
  template<typename T>
  static void read_w_message(T &elem, const char message[]);

  /**
   * Print an error message to the console.
   * @param message
   */
  static void print_error(const char message[]);

public:
    UserInterface() : ctrl(ContractController()) {}

    /**
     * The main command loop
     */
    [[noreturn]] void command_loop();

    /**
     * Print the help menu
     */
    static void print_help_menu();

    /**
     * Exit the program.
     */
    static void exit_gracefully();

    /**
     * Start dialogue to add a course.
     */
    void addCourse();

    /**
     * List all of the courses.
     */
    void listAll();

    /**
     *  Start dialogue to modify a course
     */
    void modifyCourse();

    /**
     * Start dialogue to delete a course;
     */
    void deleteCourse();

    /**
     *  Start dialogue to find a course by its name
     */
    void findCourseByName();

    /**
     *  Start dialogue to find courses by their number of hours per week.
     */
    void filterByHPW();

    /**
     *  Start dialogue to find courses by their teacher.
     */
    void filterByTeacher();

    /**
     *  Start dialogue to show courses sorted by a given criterion.
     */
    void getSortedBy();

    /**
     * Start dialogue to add a course to the current contract
     */
    void addCourseToContract();

    /**
     * User comand to clear the contract.
     */
    void clearContract();


    /**
     *  Start dialogue to export the current contract.
     */
    void exportContract();


    /**
     * Start dialogue to generate a random contract
     */
    void generateContract();

    /**
     * Print a message saying how many courses there are in the contract.
     */
    void printContractCourseCount();

    /*
     * Print a report about how many courses of each type there are in the database.
     * */
    void typeReport();

    /** Undo the last add, modify, or delete action. */
    void undoLast();
};

#endif //LAB5_CONTRACT_DE_STUDII_USER_INTERFACE_H
