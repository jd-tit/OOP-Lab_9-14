//
// Created by jdt on 3/27/2022.
//

#include "user_interface.h"
#include "validate.h"
#include "controller.h"

#include <unordered_map>
#include <functional>

void UserInterface::command_loop() {
    std::string cmd;
    std::unordered_map<std::string, std::function<void()>> valid_commands;

    MainWindow mainWindow;
    mainWindow.show();
    QApplication::exec();

//    while (true) {
//        std::cout << std::endl;
//        print_help_menu();
//        read_w_message(cmd, "-->");
//
//
//        if (cmd == "add") {
//            addCourse();
//        } else if (cmd == "exit") {
//            exit_gracefully();
//        } else if (cmd == "list") {
//            listAll();
//        } else if (cmd == "rm") {
//            deleteCourse();
//        } else if (cmd == "mod") {
//            modifyCourse();
//        } else if (cmd == "find name") {
//            findCourseByName();
//        } else if (cmd == "filter hpw") {
//            filterByHPW();
//        } else if (cmd == "filter teacher") {
//            filterByTeacher();
//        } else if (cmd == "sorted by") {
//            getSortedBy();
//        } else if (cmd == "add to contract" || cmd == "a2c") {
//            addCourseToContract();
//        } else if (cmd == "clear contract" || cmd == "cc") {
//            clearContract();
//        } else if (cmd == "export contract" || cmd == "ex c") {
//            exportContract();
//        } else if (cmd == "generate contract" || cmd == "gen c") {
//            generateContract();
//        } else if (cmd == "type report" || cmd == "tr") {
//            typeReport();
//        } else if (cmd == "undo" || cmd == "u") {
//            undoLast();
//        } else {
//            print_error("Invalid command.");
//        }
//    }
}


void UserInterface::print_help_menu() {
    std::cout << "'exit': Exit the program.\n" <<
              "'add':  Add a course.\n" <<
              "'rm':   Remove a course.\n" <<
              "'list': List all courses.\n" <<
              "'mod':  Modify a course.\n" <<
              "'filter hpw': Filter courses by how many hours per week they take.\n" <<
              "'filter teacher': Filter courses by teacher.\n"
              "'find name': Find a course by its name.\n" <<
              "'sorted by': Get the list of courses, sorted by a given criterion.\n" <<
              "'add to contract' or 'a2c': Add a course to the contract.\n" <<
              "'clear contract' or 'cc': Clear all courses from the contract\n" <<
              "'export contract' or 'ex c': Export the contract ot a file\n" <<
              "'generate contract' or 'gen c': Generate a random contract.\n" <<
              "'type report' or 'tr': Generate a report about the types of courses.\n" <<
              "'undo' or 'u': Undo last add/delete/modify." <<
              std::endl;
}


void UserInterface::addCourse(std::string name, std::string teacher, std::string type, std::string hours_string) {
//    std::string name, teacher, type, hours_string;
//    std::cout << "Add a course" << std::endl;
//    read_w_message(name, "Course name: ");
//    read_w_message(teacher, "Teacher's name: ");
//    read_w_message(type, "Course type: ");
//    read_w_message(hours_string, "How many hours per week will this course take: ");

    try {
        UserInterface::ctrl.add_course(name, teacher, type, hours_string);
    }
    catch (std::out_of_range &e) {
        print_error(e.what());
        return;
    }
    catch (std::invalid_argument &e) {
        print_error(e.what());
        return;
    }

    std::cout << "Course added successfully!\n";
}

void UserInterface::deleteCourse() {
    std::string id_buff;
    read_w_message(id_buff, "ID of course to delete: ");

    try {
        ctrl.remove_course(id_buff);
    }
    catch (std::out_of_range &e) {
        print_error(e.what());
        return;
    }
    std::cout << "Course removed successfully!\n";
}

void UserInterface::modifyCourse() {
    std::string id_buff;
    read_w_message(id_buff, "ID of course to modify: ");

    std::string name, teacher, type, hours_string;
    std::cout << "Add a course" << std::endl;
    read_w_message(name, "Course name: ");
    read_w_message(teacher, "Teacher's name: ");
    read_w_message(type, "Course type: ");
    read_w_message(hours_string, "How many hours per week will this course take: ");

    try {
        Course modded(name, type, teacher, std::stoi(hours_string), std::stoi(id_buff));
        UserInterface::ctrl.modify_course(id_buff, modded);
    }
    catch (std::out_of_range &e) {
        print_error(e.what());
        return;
    }
    catch (std::invalid_argument &e) {
        print_error(e.what());
        return;
    }
}

template<typename T>
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

    auto* error = new QMessageBox;
    error->setText(message);
    error->show();
}


void UserInterface::listAll() {
    const auto &result = ctrl.getAll();
    if (result.get_size() == 0) {
        print_error("There are no courses.");
        return;
    }

    for (const auto &e: result) {
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
    catch (std::out_of_range &e) {
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

        if (result_vector->empty()) {
            std::cout << "No courses have an amount of hours per week in the given range.\n";
            return;
        }

        for (auto &course: *result_vector) {
            std::cout << course.to_str() << std::endl;
        }
    }
    catch (std::invalid_argument &e) {
        print_error(e.what());
    }
}

void UserInterface::filterByTeacher() {
    std::string teacher_name;
    read_w_message(teacher_name, "Teacher name: ");


    auto result_vector = ctrl.getCoursesMatchingTeacher(teacher_name);

    if (result_vector->empty()) {
        std::cout << "No courses have this teacher.\n";
        return;
    }

    for (auto &course: *result_vector) {
        std::cout << course.to_str() << std::endl;
    }

}

void UserInterface::getSortedBy() {
    std::string criterion;
    std::cout << "The valid criteria are: teacher, name, hours. Secondary sort is by type." << std::endl;
    read_w_message(criterion, "Which criterion to search by? ");

    try {
        auto result_vector = ctrl.getSortedBy(criterion);

        if (result_vector->empty()) {
            print_error("There are no courses yet.");
        }

        std::cout << "The sorted courses are: " << std::endl;
        for (auto &course: *result_vector) {
            std::cout << course.to_str() << std::endl;
        }
    }
    catch (std::invalid_argument &e) {
        print_error(e.what());
    }

}

void UserInterface::addCourseToContract() {
    std::string course_name;
    read_w_message(course_name, "Name of course to be added: ");


    try {
        ctrl.add_course_to_contract(course_name);
    }
    catch(std::out_of_range& e){
        print_error(e.what());
        printContractCourseCount();
        return;
    }
    std::cout << "Course added to contract successfully!" << std::endl;
    printContractCourseCount();
}

void UserInterface::printContractCourseCount() {
    std::cout   << "The contract currently contains " << ctrl.get_number_of_courses_in_contract()
                << " courses." << std::endl;
}

void UserInterface::clearContract() {
    ctrl.clear_contract();
    std::cout << "Contract cleared successfully!" << std::endl;
}

void UserInterface::exportContract() {
    std::string filename;
    read_w_message(filename, "Contract filename (without extension): ");

    for(auto c : filename){
        if(!std::isalnum(c)){
            print_error("Error: filename contains invalid characters.");
            return;
        }
    }

    ctrl.export_contract_to_file(filename);

    std::cout << "Contract exported successfully!";
    printContractCourseCount();
}

void UserInterface::generateContract() {
    std::string count_buff;
    read_w_message(count_buff, "Number of courses to generate: ");

    try {
        ctrl.generate_contract(count_buff);
    }
    catch(std::invalid_argument& e)
    {
        print_error(e.what());
    }

    std::cout << "Generated " << count_buff << "courses successfully and added them to the contract!";
}

void UserInterface::typeReport() {
    std::cout << "Type report:" << std::endl;

    auto type_data = ctrl.getCourseTypeData();
    for(const auto& it : *type_data){
        std::cout << it.first << ": " << it.second << std::endl;
    }
}

void UserInterface::undoLast() {
    try {
        ctrl.undo_last();
    } catch (std::out_of_range& e) {
        print_error(e.what());
        return;
    }

    std::cout << "Undo successful!" << std::endl;
}
