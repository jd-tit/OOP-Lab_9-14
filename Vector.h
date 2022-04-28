//
// Created by jdt on 2022-04-06.
//

#ifndef LAB5_CONTRACT_DE_STUDII_VECTOR_H
#define LAB5_CONTRACT_DE_STUDII_VECTOR_H

#include <cstdlib>
#include <functional>
#include <stdexcept>

template<typename T>
class Vector{
private:
  T* contents;
  size_t capacity;
  size_t crt_size;
  static const size_t RESIZE_FACTOR = 2;
  static const size_t INIT_MAX_SIZE = 10;

  /**
   * Increase the size of the internal container
   */
  void increase_size();

  /**
   * Decrease the size of the internal container
   */
  void decrease_size();

public:
//    struct iterator: public std::iterator<std::random_access_iterator_tag, T>{
//        using difference_type = typename std::iterator<std::random_access_iterator_tag, T>::difference_type;
//        using iterator_category = std::random_access_iterator_tag;
//        using value_type        = T;
//        using pointer           = T*;
//        using reference         = T&;
//    private:
//        T* ptr;
//    public:
//        iterator(): ptr(nullptr) {};
//        explicit iterator(T* ptr): ptr{ptr} {}
//
//        iterator(const iterator& it) : ptr{it.ptr} {}
//
//        pointer operator->() const
//        {
//          if (ptr == nullptr)
//            throw std::bad_function_call();
//          return (ptr);
//        }
//
//        iterator& operator=(const iterator& it){
//          if(*this == it){
//            return *this;
//          }
//          this->ptr = it.ptr;
//          return *this;
//        }
//
//        iterator& operator++(){
//          ptr++;
//          return *this;
//        }
//
//        iterator& operator--(){
////          if (index <= 0)
////            throw std::out_of_range("iterator cannot be decremented past the end of range.");
//
//          --ptr;
//          return *this;
//        }
//
//        T& operator*(){
//          if (ptr == nullptr)
//            throw std::bad_function_call();
//          return *ptr;
//        }
//
//        difference_type operator-(iterator other){
//          return ptr - other.ptr;
//        }
//
//        iterator& operator-(int val){
//          *this -= val;
//          return *this;
//        }
//
//        iterator& operator+(int val){
//          *this += val;
//          return *this;
//        }
//
//        iterator& operator+=(difference_type const offset){
////          if (index + offset < 0)
////            throw std::out_of_range("iterator cannot be incremented past the end of range.");
//          ptr += offset;
//          return *this;
//        }
//
//        iterator& operator-=(difference_type const offset){
//          *this += -offset;
//          return *this;
//        }
//
//        bool operator<=(iterator other){
//          return ptr <= other.ptr;
//        }
//
//        bool operator<(iterator other){
//          return ptr < other.ptr;
//        }
//
//        bool operator>=(iterator other){
//          return ptr >= other.ptr;
//        }
//
//        bool operator>(iterator other){
//          return ptr > other.ptr;
//        }
//
//        bool operator==( iterator rhs){
//          return ptr == rhs.ptr;
//        }
//
//        bool operator!=(iterator rhs){
//          return ptr != rhs.ptr;
//        }
//
//        const T& operator[](difference_type rhs){
//          return ptr[rhs];
//        }
//    };

    class iterator
    {
    public:
        using value_type = T;
        using pointer    = T*;
        using reference  = T&;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::random_access_iterator_tag;

        iterator(): v(nullptr), i(0) {}
        iterator(Vector<T>* v, int i): v(v), i(i) {}
        // Default Copy/Move Are Fine.
        // Default Destructor fine.

        reference       operator*()             {return (*v)[i];}
        reference operator*()       const {return (*v)[i];}
        pointer         operator->()            {return &((*v)[i]);}
        const pointer   operator->()      const {return &((*v)[i]);}
        reference       operator[](int m)       {return (*v)[i + m];}
        reference operator[](int m) const {return (*v)[i + m];}


        iterator& operator++()       {++i;return *this;}
        iterator& operator--()       {--i;return *this;}
        iterator  operator++(int)    {iterator r(*this);++i;return r;}
        iterator  operator--(int)    {iterator r(*this);--i;return r;}

        iterator& operator+=(int n)  { i += n;return *this;}
        iterator& operator-=(int n)  { i -= n;return *this;}

        iterator operator+(int n)   const {iterator r(*this);return r += n;}
        iterator operator-(int n)   const {iterator r(*this);return r -= n;}

        difference_type operator-(iterator const& r) const {return i - r.i;}

        // Note: comparing iterator from different containers
        //       is undefined behavior so we don't need to check
        //       if they are the same container.
        bool operator<(iterator const& r)  const {return i < r.i;}
        bool operator<=(iterator const& r) const {return i <= r.i;}
        bool operator>(iterator const& r)  const {return i > r.i;}
        bool operator>=(iterator const& r) const {return i >= r.i;}
        bool operator!=(const iterator &r) const {return i != r.i;}
        bool operator==(const iterator &r) const {return i == r.i;}

    private:
        Vector<T>* v;
        int        i;
    };

    struct const_iterator: public std::iterator<std::random_access_iterator_tag, T>{
        using difference_type = typename std::iterator<std::random_access_iterator_tag, T>::difference_type;
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using pointer           = T*;
        using reference         = const T&;
    private:
        const Vector<T>* ptr;
        int i;
    public:
        explicit const_iterator(const Vector<T>* cont, int i): ptr{cont}, i(i){}
        const_iterator() : i{0}, ptr{nullptr} {}

        pointer operator->() const
        {
          if (ptr == nullptr)
            throw std::bad_function_call();
          return &(*ptr)[i];
        }

        const_iterator& operator++(){
          i++;
          return *this;
        }

        const_iterator& operator--(){
          if (i <= 0)
            throw std::out_of_range("iterator cannot be decremented past the end of range.");
          --ptr;
          return *this;
        }

        reference operator*(){
          if (ptr == nullptr)
            throw std::bad_function_call();
          return (*this)[i];
        }

        difference_type operator-(const_iterator other) const{
          return i - other.i;
        }

        const_iterator& operator-(int val){
          *this -= val;
          return *this;
        }

        const_iterator& operator+(int val){
          *this += val;
          return *this;
        }

        const_iterator& operator+=(difference_type const offset){
          if (i + offset < 0)
            throw std::out_of_range("iterator cannot be incremented past the end of range.");
          i += offset;
          return *this;
        }

        const_iterator& operator-=(difference_type const offset){
          *this += -offset;
          return *this;
        }

        bool operator<=(const_iterator other){
          return i <= other.i;
        }

        bool operator<(const_iterator other){
          return i < other.i;
        }

        bool operator>=(const_iterator other){
          return i >= other.i;
        }

        bool operator>(const_iterator other){
          return i > other.i;
        }

        bool operator==( const_iterator rhs){
          return i == rhs.i;
        }

        bool operator!=(const_iterator rhs){
          return i != rhs.i;
        }

        reference operator[](difference_type rhs) const{
          return (*ptr)[rhs];
        }
    };

  Vector();

  Vector(Vector&);

  ~Vector();

  /**
   * Add an element to the vector
   * @param e - the element ot be added
   */
  void push_back(const T& e);

  /**
   * Erase an element a a given position
   * @param it
   */
  void erase(const_iterator it);

  /**
   * Erase an element a a given position
   * @param it
   */
  void erase(iterator it);

  /**
   * Get a constant iterator to the beginning of the vector
   * @return The iterator
   */
  Vector<T>::const_iterator cbegin() const;


  /**
   * Get a constant iterator to the end of the vector
   * @return The iterator
   */
  Vector<T>::const_iterator cend() const;

  /**
   * Get an iterator to the beginning of the vector
   * @return  The iterator
   */
  Vector<T>::iterator begin();

/**
 * Get an iterator to the end of the vector
 * @return  The iterator
 */
  Vector<T>::iterator end();

  /**
   * Get the element at a given index.
   * @param index
   * @return
   */
  T& operator[](size_t index){
      if(index > crt_size)
        throw(std::out_of_range("Index ot of range"));
      return contents[index];
  };

  const T& operator[](size_t index) const{
    if(index > crt_size)
      throw(std::out_of_range("Index ot of range"));
    return contents[index];
  };

    /**
     * Get the element at a given index.
     * @param index
     * @return
     */
  T& at(size_t index);
  const T& at(size_t index) const;
  [[nodiscard]] size_t size() const;

};

template <typename T> Vector<T>::Vector() :
  capacity(INIT_MAX_SIZE), crt_size(0)
{
  capacity = INIT_MAX_SIZE;
  contents = new T[capacity];
}

template <typename T> void Vector<T>::increase_size() {
  capacity *= RESIZE_FACTOR;
  auto new_contents = new T[capacity];
  for(size_t i = 0; i < crt_size; ++i){
    new_contents[i] = contents[i];
  }
  delete[] contents;
  contents = new_contents;
}

template <typename T> void Vector<T>::decrease_size() {
  capacity /= RESIZE_FACTOR;
  auto new_contents = new T[capacity];
  for(size_t i = 0; i < crt_size; ++i){
    new_contents[i] = contents[i];
  }
  delete[] contents;
  contents = new_contents;
}

template <typename T> Vector<T>::~Vector() {
  delete[] contents;
}

template <typename T> void Vector<T>::push_back(const T &e) {
  if(crt_size +1 == capacity){
    increase_size();
  }
  contents[crt_size] = e;
  ++crt_size;
}

template <typename T> typename Vector<T>::const_iterator Vector<T>::cbegin() const{
  return Vector<T>::const_iterator(this, 0);
}

template <typename T> typename Vector<T>::const_iterator Vector<T>::cend() const{
  return Vector<T>::const_iterator(this, crt_size);
}

template <typename T> typename Vector<T>::iterator Vector<T>::begin() {
  return Vector<T>::iterator(this, 0);
}

template <typename T> typename Vector<T>::iterator Vector<T>::end() {
  return Vector<T>::iterator(this, crt_size);
}

template <typename T> void Vector<T>::erase(const_iterator it) {
  auto last = --end();
  std::swap(*it, *last);
  --crt_size;
}

template <typename T> void Vector<T>::erase(iterator it) {
  auto last = --end();
  std::swap(*it, *last);
  --crt_size;
}

template <typename T> T& Vector<T>::at(size_t index) {
  if(index > crt_size)
    throw(std::out_of_range("Index ot of range"));
  return contents[index];
}
template <typename T> size_t Vector<T>::size() const {
  return crt_size;
}
template <typename T> const T &Vector<T>::at(size_t index) const {
  return contents[index];
}
template <typename T> Vector<T>::Vector(Vector<T>& old) :
capacity{old.capacity}, crt_size{0} {
  contents = new T[capacity];
  for(size_t i = 0; i < old.crt_size; ++i){
    push_back(old[i]);
  }
}

#endif // LAB5_CONTRACT_DE_STUDII_VECTOR_H
