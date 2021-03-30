#ifndef LIBME_STACK_HPP
  #define LIBME_STACK_HPP

namespace me {

  template<size_t _Length>
  class stack {

  protected:

  public:

    template<typename T>
    T* allocate();

    template<typename T>
    T* allocate(T&& _value);

    template<typename T, typename... A>
    T* allocate(A&&... _args);

  };

}

#endif
