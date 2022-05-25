//
// Created by jdt on 2022-03-31.
//

#include "../include/test.h"
#include "../include/domain.h"
#include "../include/validate.h"
#include "../include/repo.h"
#include "../include/controller.h"
#include <algorithm>

#include <cassert>

void testDomain(){
  Course c{"Painting", "optional", "Bob Ross", 4, 0};

  assert(c.to_str() == "ID: 0; Name: Painting; Type: optional; Teacher: Bob Ross; Hours per week: 4");
  assert(c.to_csv() == "0,Painting,optional,Bob Ross,4"),
  c.set_name("Racketeering");
  assert(c.get_name() == "Racketeering");

  c.set_teacher("Mike");
  assert(c.get_teacher() == "Mike");

  c.set_type("Maybe");
  assert(c.get_type() == "Maybe");

  c.set_hpw(1);
  assert(c.getHPW() == 1);

  assert(c.get_id() == 0);
}

void testValidate(){
  assert(is_valid_string("Cooking"));
  assert(!is_valid_string(""));
  assert(!is_valid_string("\""));
  assert(!is_valid_string("A"));

  for(int i = 1; i < 24 * 7 / 4; ++i) {
    assert(is_valid_hours_per_week(i));
  }
  assert(!is_valid_hours_per_week(0));
  assert(!is_valid_hours_per_week(100));
}

void testRepo(){
    Repo<Course> repo;
    Course c1{"Painting", "optional", "Bob Ross", 4, 0};
    Course c2{"Math", "optional", "Terry Tao", 8, 1};
    Course c3{"Music", "optional", "Lena Raine", 5, 2};

    repo.add(c1);
    repo.add(c2);
    repo.add(c3);

    Course courses[] = {c1, c2,c3};

    assert(repo.get_size() == 3);
    for(size_t i = 0; i < repo.get_size(); ++i){
        const auto& res_c = repo.at(i);
        assert(res_c.to_str() == courses[i].to_str());
    }

    int i = 0;
    for(const auto& res_c : repo){
        assert(res_c.to_str() == courses[i].to_str());
        ++i;
    }

    i = 0;
    for(auto it = repo.begin(); it != repo.end(); ++it){
        assert(it->to_str() == courses[i].to_str());
        ++i;
    }

    i = 0;
    for(auto it = repo.cbegin(); it != repo.cend(); ++it){
        assert(it->to_str() == courses[i].to_str());
        ++i;
    }

    i = 0;
    for(auto it = repo.begin(); it != repo.end(); ++it){
        assert(it->to_str() == courses[i].to_str());
        ++i;
    }

    auto& c = repo.find_by_id(1);
    assert(c.to_str() == c2.to_str());

    repo.remove_by_id(2);
    assert(repo.get_size() == 2);

    auto& vect = repo.getVector();
    assert(vect.size() == 2);

    try{
        repo.find_by_id(100);
        assert(false);
    }
    catch (std::out_of_range& e){}

    try{
        repo.remove_by_id(100);
        assert(false);
    }
    catch (std::out_of_range& e){}
}

void testControl(){
    ContractController ctrl;
    ctrl.add_course("Painting",  "Bob Ross", "optional", "4");
    Course c1{"Painting", "optional", "Bob Ross", 4, 0};
    Course c2{"Math", "optional", "Terry Tao", 8, 1};
    Course c3{"Music", "optional", "Lena Raine", 5, 2};
    auto repo = ctrl.getAll();

    assert(repo->size() == 1);
    assert(repo->at(0).to_str() == c1.to_str());

    ctrl.add_course("Math", "Terry Tao", "optional", "8");
    ctrl.add_course("Music", "Lena Raine", "optional", "5");
    ctrl.add_course("Music part two", "Lena Raine", "optional", "5");


    // bad add
    try
    {
        ctrl.add_course("Music", "Lena Raine", "optional", "5");
        assert(false);
    }
    catch (std::invalid_argument&){}
    try
    {
        ctrl.add_course("", "Lena Raine", "optional", "5");
        assert(false);
    }
    catch (std::invalid_argument&){}
    try
    {
        ctrl.add_course("Music", "", "optional", "5");
        assert(false);
    } catch (std::invalid_argument&){}
    try
    {
        ctrl.add_course("Music", "Lena Raine", "", "bad");
        assert(false);
    } catch (std::invalid_argument&){}
    try
    {
        ctrl.add_course("Music", "Lena Raine", "optional", "bad");
        assert(false);
    } catch (std::invalid_argument&){}
    try
    {
        ctrl.add_course("Music", "Lena Raine", "optional", "-1");
        assert(false);
    } catch (std::invalid_argument&){}

    auto& cc = ctrl.getCourse("Math");
    assert(cc.to_str() == c2.to_str());

    auto vec = ctrl.getCoursesMatchingHPW("5", "8");
    assert(vec->size() == 3);

    try {
        ctrl.getCoursesMatchingHPW("8", "5");
        assert(false);
    } catch(std::invalid_argument& e){}

    try {
        ctrl.getCoursesMatchingHPW("asdsad", "bad");
        assert(false);
    } catch(std::invalid_argument& e){}

    auto vec2 = ctrl.getCoursesMatchingTeacher("Lena Raine");
    assert(vec2->size() == 2);

    std::string criteria[3] = {"hours", "name", "teacher"};
    auto vec3 = ctrl.getSortedBy(criteria[0]);
    assert(vec3->size() == 4);
    assert(vec3->at(0).to_str() == c1.to_str());

    auto vec4 = ctrl.getSortedBy(criteria[1]);
    assert(vec4->size() == 4);
    assert(vec4->at(0).to_str() == c2.to_str());

    auto vec5 = ctrl.getSortedBy(criteria[2]);
    assert(vec5->size() == 4);
    assert(vec5->at(0).to_str() == c1.to_str());

    try{
        ctrl.getSortedBy("bad_criterion");
        assert(false);
    }
    catch(std::invalid_argument&){}

    ctrl.modify_course("2", Course{"Math", "optional", "Terry Tao",  10, 2});

    ctrl.remove_course("0");
    assert(ctrl.getAll()->size() == 3);

    ctrl.generate_contract("1");
    auto last = ctrl.getAll()->at(3);
    assert(is_valid_string(last.get_name()));
    assert(is_valid_string(last.get_teacher()));
    assert(is_valid_string(last.get_type()));
    assert(is_valid_hours_per_week(last.getHPW()));
    assert(last.get_id() == 4);
    assert(ctrl.get_number_of_courses_in_contract() == 1);
    ctrl.add_course_to_contract("Math");
    try {
        ctrl.add_course_to_contract("BAD");
    } catch (std::out_of_range&) {}
    assert(ctrl.get_number_of_courses_in_contract() == 2);
    ctrl.clear_contract();
    assert(ctrl.get_number_of_courses_in_contract() == 0);


    // bad generate
    try{
        ctrl.generate_contract("bad");
        assert(false);
    } catch (std::invalid_argument&) {}

    ContractController ctrl2;
    ctrl2.generate_contract("1");
    ctrl2.export_contract_to_file("test");
    std::fstream in("test.csv", std::ios::in);
    assert(in.is_open());
    std::string line;
    std::getline(in, line);
    assert(ctrl2.getAll()->at(0).to_csv() == line);

    //Type Data

    ContractController ctrl3;
    ctrl3.add_course("Math", "Terry Tao", "mandatory", "8");
    ctrl3.add_course("Music", "Lena Raine", "optional", "5");
    ctrl3.add_course("Music part two", "Lena Raine", "optional", "5");

    auto data = ctrl3.getCourseTypeData();

    assert((*data)["mandatory"] == 1);
    assert((*data)["optional"] == 2);
}

void testUndo() {
    ContractController ctrl;
    ctrl.add_course("Math", "Terry Tao", "mandatory", "8");
    ctrl.add_course("Music", "Lena Raine", "optional", "5");

    assert(ctrl.getAll()->size() == 2);

    ctrl.undo_last();
    assert(ctrl.getAll()->size() == 1);

    Course c1{"Artistic Painting", "optional", "Bob Ross", 4, 0};
    ctrl.modify_course("0", c1);

    ctrl.undo_last();

    ctrl.getCourse("Math");
    try {
        ctrl.getCourse("Painting");
        assert(false);
    } catch(std::exception& e) {}


    ctrl.remove_course("0");
    assert(ctrl.getAll()->size() == 0);

    ctrl.undo_last();
    assert(ctrl.getAll()->size() == 1);

}

//void testVector(){
//    Course c1{"Painting", "optional", "Bob Ross", 4, 0};
//    Vector<Course> vector;
//
//    for(int i = 0; i < 100; ++i){
//        vector.push_back(c1);
//    }
//
//    for(int i = 0; i < 100; ++i){
//        assert(vector.at(i).to_str() == c1.to_str());
//        assert(vector[i].to_str() == c1.to_str());
//    }
//
//    auto v2 = vector;
//
//    for(int i = 0; i < 100; ++i){
//        assert(v2.at(i).to_str() == c1.to_str());
//    }
//
//    for(auto it = v2.cbegin(); it != v2.cend(); ++it) {
//        assert(it->to_str() == c1.to_str());
//        assert((*it).to_str() == c1.to_str());
//    }
//
//    for(auto it = v2.begin(); it != v2.end(); ++it) {
//        assert(it->to_str() == c1.to_str());
//        assert((*it).to_str() == c1.to_str());
//    }
//
//    try{
//        vector.at(1234);
//        assert(false);
//    } catch (std::out_of_range&) {}
//
//    try{
//        const auto a = vector[1234];
//        assert(false);
//    } catch (std::out_of_range&) {}
//
//    try{
//        auto a = vector[1234];
//        assert(false);
//    } catch (std::out_of_range&) {}
//
//
//    assert(vector.begin()->to_str() == c1.to_str());
//    assert(vector.cbegin()->to_str() == c1.to_str());
//
//}

//void testBadSort() {
//  Course c1{"Music", "optional", "Lena Raine", 5, 0};
//  Course c2{"Music two", "optional", "Lena Raine", 1, 1};
//  ContractController ctrl;
//  ctrl.add_course("Music", "Lena Raine", "optional", "5");
//  ctrl.add_course("Music two", "Lena Raine", "optional", "1");
//
//
//
//  Vector<int> v1;
//
//  v1.push_back(3);
//  v1.push_back(2);
//  v1.push_back(1);
//
//  std::vector<int> v2{1, 3, 2, 1};
//
//  auto begin = v1.begin();
//  auto end = v1.end();
//
//  std::sort(begin, end);
//  std::sort(std::begin(v2), std::end(v2));
//
//
//    std::string h = "hours";
//  auto result = ctrl.getSortedBy(h);
//  assert(result->at(0).to_str() == c2.to_str());
//  assert(result->at(1).to_str() == c1.to_str());
//}

void testFileOps(){
    ContractController ctrl;

    // TEST ADDING
    ctrl.add_course("Math", "Terry Tao", "mandatory", "8");
    Repo<Course> tmp_repo;
    tmp_repo.read_from_file();
    assert(tmp_repo.get_size() == 1);


    // TEST MODIFYING
    Course c1{"Painting", "optional", "Bob Ross", 4, 0};
    ctrl.modify_course("0", c1);

    tmp_repo.getVector().clear();
    tmp_repo.read_from_file();
    assert(tmp_repo.get_size() == 1);
    assert(tmp_repo.at(0).to_str() == c1.to_str());


    // TEST DELETING
    ctrl.remove_course("0");

    tmp_repo.getVector().clear();
    tmp_repo.read_from_file();
    assert(tmp_repo.get_size() == 0);

    //TEST UNDO

    ctrl.undo_last();

    tmp_repo.getVector().clear();
    tmp_repo.read_from_file();
    assert(tmp_repo.get_size() == 1);
    assert(tmp_repo.at(0).to_str() == c1.to_str());
}

void run_all_tests() {
  testDomain();
  testValidate();
  testRepo();
  testControl();
  testUndo();
  testFileOps();
//  testVector();
//  testBadSort();

  std::cout << "All tests ran successfully!" << std::endl;
}
