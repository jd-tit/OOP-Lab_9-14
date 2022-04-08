//
// Created by jdt on 3/27/2022.
//

#include "user_interface.h"
#include "validate.h"

#include <unordered_map>
#include <functional>

[[noreturn]] void UserInterface::command_loop() {
    std::string cmd;
    std::unordered_map<std::string, std::function<void()>> valid_commands;

    while(true){
        print_help_menu();
        read_w_message(cmd, "-->");


        if(cmd == "add"){
          addCourse();
        } else if ( cmd == "exit") {
          exit_gracefully();
        } else if (cmd == "list") {
          listAll();
        } else if(cmd == "rm") {
          deleteCourse();
        } else if(cmd == "mod") {
          modifyCourse();
        } else if(cmd == "find name") {
          findCourseByName();
        } else if(cmd == "filter hpw") {
          filterByHPW();
        } else if(cmd == "filter teacher"){
          filterByTeacher();
        } else if(cmd == "sorted by"){
          getSortedBy();
        } else {
          print_error("Invalid command.");
        }
    }
}


void UserInterface::print_help_menu() {
    std::cout <<    "'exit': Exit the program.\n" <<
                    "'add':  Add a course.\n" <<
                    "'rm':   Remove a course.\n" <<
                    "'list': List all courses.\n" <<
                    "'mod':  Modify a course.\n" <<
                    "'filter hpw': Filter courses by how many hours per week they take.\n" <<
                    "'filter teacher': Filter courses by teacher.\n"
                    "'find name': Find a course by its name.\n" <<
                    "'sorted by': Get the list of courses, sorted by a given criterion." <<
                    std::endl;
}


void UserInterface::addCourse() {
  std::string name, teacher, type, hours_string;
  std::cout << "Add a course" << std::endl;
  read_w_message(name, "Course name: ");
  read_w_message(teacher, "Teacher's name: ");
  read_w_message(type, "Course type: ");
  read_w_message(hours_string, "How many hours per week will this course take: ");

  try{
    UserInterface::ctrl.add_course(name, teacher, type, hours_string);
  }
  catch(std::out_of_range& e){
    print_error(e.what());
    return;
  }
  catch (std::invalid_argument& e){
    print_error(e.what());
    return;
  }

  std::cout << "Course added successfully!\n";
}

void UserInterface::deleteCourse() {
  std::string id_buff;
  read_w_message(id_buff, "ID of course to delete:");

  try {
    ctrl.remove_course(id_buff);
  }
  catch(std::out_of_range& e){
    print_error(e.what());
    return;
  }
  std::cout << "Course removed successfully!\n";
}

void UserInterface::modifyCourse() {
  std::string id_buff;
  read_w_message(id_buff, "ID of course to modify:");

  std::string name, teacher, type, hours_string;
  std::cout << "Add a course" << std::endl;
  read_w_message(name, "Course name: ");
  read_w_message(teacher, "Teacher's name: ");
  read_w_message(type, "Course type: ");
  read_w_message(hours_string, "How many hours per week will this course take: ");

  try{
    UserInterface::ctrl.modify_course(id_buff, name, teacher, type, hours_string);
  }
  catch(std::out_of_range& e){
    print_error(e.what());
    return;
  }
  catch (std::invalid_argument& e){
    print_error(e.what());
    return;
  }
}

template <typename T>
void UserInterface::read_w_message(T &elem, const char message[]) {
  std::cout << message;
  std::getline(std::cin, elem);
  std::cout << std::endl;
}


void UserInterface::exit_gracefully() {
  std::cout << "Goodbye!";
  exit(EXIT_SUCCESS);
}


void UserInterface::print_error(const char *message) {
  std::cout << message << std::endl;
}


void UserInterface::listAll() {
  const auto& result = ctrl.getAll();
  if(result.get_size() == 0){
    print_error("There are no courses.");
    return;
  }

  for(const auto& e : result){
    std::cout << e.to_str() << std::endl;
  }
}

void UserInterface::findCourseByName() {
  std::string target_name;
  read_w_message(target_name, "Name of course to search for: ");

  try {
    const auto &result = ctrl.getCourse(target_name);
    std::cout << result.to_str() << std::endl;
  }
  catch(std::out_of_range& e){
    print_error(e.what());
    return;
  }
}

void UserInterface::filterByHPW() {
  std::string start_buff, stop_buff;
  std::cout << "Get courses with a number of hours per week in the interval [start, stop]\n";
  read_w_message(start_buff, "Start: ");
  read_w_message(stop_buff, "Stop: ");

  try {
    auto result_vector = ctrl.getCoursesMatchingHPW(start_buff, stop_buff);
    std::cout << "The matching courses are: " << std::endl;

    if(result_vector->size() == 0){
      std::cout << "No courses have an amount of hours per week in the given range.\n";
      return;
    }

    for(auto& course : *result_vector){
      std::cout << course.to_str() << std::endl;
    }
  }
  catch(std::invalid_argument& e){
    print_error(e.what());
  }
}
void UserInterface::filterByTeacher() {
  std::string teacher_name;
  read_w_message(teacher_name, "Teacher name: ");


  auto result_vector = ctrl.getCoursesMatchingTeacher(teacher_name);

  if(result_vector->size() == 0){
    std::cout << "No courses have this teacher.\n";
    return;
  }

  for(auto& course : *result_vector){
    std::cout << course.to_str() << std::endl;
  }

}
void UserInterface::getSortedBy() {
  std::string criterion;
  std::cout << "The valid criteria are: teacher, name, hours. Secondary sort is by type." << std::endl;
  read_w_message(criterion, "Which criterion to search by? ");

  try {
    auto result_vector = ctrl.getSortedBy(criterion);

    if(result_vector->size() == 0){
      print_error("There are no courses yet.");
    }

    std::cout << "The sorted courses are: " << std::endl;
    for(auto& course : *result_vector){
      std::cout << course.to_str() << std::endl;
    }
  }
  catch(std::invalid_argument& e){
    print_error(e.what());
  }

}
