//
// Created by jdt on 2022-03-31.
//

#ifndef LAB5_CONTRACT_DE_STUDII_REPO_H
#define LAB5_CONTRACT_DE_STUDII_REPO_H

#include <utility>
#include <vector>
#include <fstream>

#include "domain.h"

template<typename T>
class Repo {
private:
    std::string filename;

protected:
    std::vector<T> contents;

public:
  size_t next_free_id;
  typedef typename std::vector<T>::iterator iterator;

  explicit Repo(std::string filename = "test_data.csv"): filename(std::move(filename)), next_free_id(0) {}

  /**
   * Get the element at a given index
   * @param index
   * @return
   */
  const T& at(size_t index) const{
    return contents.at(index);
  }

  /**
   * Get the element at a given index
   * @param index
   * @return
   */
  T& at(size_t index){
      return contents.at(index);
  }

  /**
   * Get the current size of the internal vector
   * @return
   */
  [[nodiscard]] size_t get_size() const{
    return contents.size();
  }

  /**
   * Add an element to the Repository
   * @param element
   */
  void add(const T& element){
    this->contents.push_back(element);
    next_free_id++;
  }

  /**
   * Get an iterator to the beginning of the Repository
   * @return
   */
  typename std::vector<T>::iterator begin(){
    return contents.begin();
  }

    /**
     * Get a const iterator to the beginning of the Repository
     * @return
     */
    typename std::vector<T>::const_iterator begin() const{
      return cbegin();
    }

    /**
     * Get a const iterator to the beginning of the Repository
     * @return
     */
    typename std::vector<T>::const_iterator cbegin() const{
      return contents.cbegin();
    }

    /**
     * Get an iterator to one past the end of the Repository
     * @return
     */
  typename std::vector<T>::iterator end(){
    return contents.end();
  }

    /**
       * Get an iterator to one past the end of the Repository
       * @return
       */
    typename std::vector<T>::const_iterator end() const{
      return cend();
    }

    /**
       * Get a const iterator to one past the end of the Repository
       * @return
       */
    typename std::vector<T>::const_iterator cend() const{
      return contents.cend();
    }

  // Find an element by its ID
  T& find_by_id(size_t id){
    auto res = std::find_if(begin(), end(),
                            [id] (const T& first)
                            {return first.get_id() == id;});
    if(res == end())
      throw std::out_of_range("Error: ID not found");
    return *res;
  }

  // Find an element's iterator by its ID
  iterator find_iterator_by_id(size_t id){
      auto res = std::find_if(begin(), end(),
                              [id] (const T& first)
                              {return first.get_id() == id;});
      if(res == end())
          throw std::out_of_range("Error: ID not found");
      return res;
  }

  /**
   * Remove the element with a given ID
   * @param id
   */
  void remove_by_id(size_t id){
    auto res = std::find_if(begin(), end(),
                            [id] (const T& first)
                            {return first.get_id() == id;});
    if(res == end())
      throw std::out_of_range("Error: ID not found");
    contents.erase(res);
  }

  /**
   * Insert an element before a given index
   * @param index
   */
  void insert_at(const T& element, size_t index){
      contents.insert(contents.begin() + index, element);
  }

  /**
   * Remove the element at a certain index.
   * @param index
   */
  void remove_at(size_t index){
    auto it = begin() + index;
    contents.erase(it);
  }

  /**
   * Remove the element at a certain iterator.
   * @param index
   */
  void remove_at(iterator it){
      contents.erase(it);
  }

  /**
   * Get the internal vector.
   * @return
   */
  std::vector<Course>& getVector(){
    return contents;
  }

void write_to_file() {
    std::fstream output(this->filename, std::ios::out | std::ios::trunc);

    for(const Course& e : this->contents){
        output << e.to_csv() << std::endl;
    }
  }

  void read_from_file() {
      std::fstream input(this->filename, std::ios::in);

      if(!input.is_open()){
          // create file if it does not exist yet
          std::fstream temp(this->filename, std::ios::out);
          if(!temp.is_open()){
              std::string errmsg("File '" + this->filename + "' could not be created.");
              throw(std::out_of_range(errmsg));
          }
          temp.close();
      }

      std::string csv_course;
      while(std::getline(input, csv_course)){
          if(csv_course.empty())
              break;
          this->add(*Course::from_csv(csv_course));
      }
  }
};
#endif // LAB5_CONTRACT_DE_STUDII_REPO_H
