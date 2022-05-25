//
// Created by jdt on 3/27/2022.
//

#include "user_interface.h"
#include "validate.h"
#include "controller.h"

#include <unordered_map>
#include <functional>

int UserInterface::execute() {
    UserInterface::init();

    MainProjectWindow mainWindow;
    mainWindow.show();
    return QApplication::exec();
}


void UserInterface::addCourse(std::string name, std::string teacher, std::string type, std::string hours_string) {
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

    qDebug() << "Course added successfully!\n";
}

void UserInterface::deleteCourse(std::string id_buff) {
//    std::string id_buff;
//    read_w_message(id_buff, "ID of course to delete: ");

    try {
        ctrl.remove_course(id_buff);
    }
    catch (std::out_of_range &e) {
        print_error(e.what());
        return;
    }
    std::cout << "Course removed successfully!\n";
}

void UserInterface::modifyCourse(Course modded) {
    try {
        UserInterface::ctrl.modify_course(std::to_string(modded.get_id()), modded);
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


[[maybe_unused]] void UserInterface::exit_gracefully() {
    std::cout << "Goodbye!";
    exit(EXIT_SUCCESS);
}


void UserInterface::print_error(const char *message) {
    std::cout << message << std::endl;

    auto* error = new QMessageBox;
    error->setText(message);
    error->setIcon(QMessageBox::Icon::Critical);
    error->show();
}

void UserInterface::print_message(const std::string message){
    std::cout << message << std::endl;
    auto c_message = message.c_str();

    auto* messageBox = new QMessageBox;
    messageBox->setText(c_message);
    messageBox->setIcon(QMessageBox::Icon::Information);
    messageBox->show();
}


//void UserInterface::listAll() {
//    const auto &result = ctrl.getAll();
//    if (result.size() == 0) {
//        print_error("There are no courses.");
//        return;
//    }
//
//    for (const auto &e: result) {
//        std::cout << e.to_str() << std::endl;
//    }
//}

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

void UserInterface::typeReportGUI(const Course& crt){
    auto type_data = ctrl.getCourseTypeData();

    auto count = (*type_data)[crt.get_type()];
    std::string message = std::to_string(count) + " other courses have type " + crt.get_type();
    print_error(message.c_str());
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
