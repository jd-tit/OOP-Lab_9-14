//
// Created by jdt on 2022-03-31.
//

#ifndef LAB5_CONTRACT_DE_STUDII_REPO_H
#define LAB5_CONTRACT_DE_STUDII_REPO_H

#include <vector>
#include "Vector.h"

template<typename T>
class Repo{
private:
  Vector<T> contents;
public:
  size_t next_free_id;

  Repo();

  /**
   * Get the element at a given index
   * @param index
   * @return
   */
  const T& at(size_t index) const{
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
  typename Vector<T>::iterator begin(){
    return contents.begin();
  }

    /**
     * Get a const iterator to the beginning of the Repository
     * @return
     */
    typename Vector<T>::const_iterator begin() const{
      return cbegin();
    }

    /**
     * Get a const iterator to the beginning of the Repository
     * @return
     */
    typename Vector<T>::const_iterator cbegin() const{
      return contents.cbegin();
    }

    /**
     * Get an iterator to one past the end of the Repository
     * @return
     */
  typename Vector<T>::iterator end(){
    return contents.end();
  }

    /**
       * Get an iterator to one past the end of the Repository
       * @return
       */
    typename Vector<T>::const_iterator end() const{
      return cend();
    }

    /**
       * Get a const iterator to one past the end of the Repository
       * @return
       */
    typename Vector<T>::const_iterator cend() const{
      return contents.cend();
    }

  //Find an element by its ID
  T& find_by_id(size_t id){
    auto res = std::find_if(begin(), end(),
                            [id] (const T& first)
                            {return first.get_id() == id;});
    if(res == end())
      throw std::out_of_range("Error: ID not found");
    return *res;
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
   * Remove the element at a certain index.
   * @param index
   */
  void remove_at(size_t index){
    auto it = begin() + index;
    contents.erase(it);
  }

  /**
   * Get the internal vector.
   * @return
   */
  Vector<Course>& getVector(){
    return contents;
  }
};
template <typename T> Repo<T>::Repo() : next_free_id(0){}

#endif // LAB5_CONTRACT_DE_STUDII_REPO_H
