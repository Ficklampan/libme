#ifndef LIBME_EXCEPTION_HPP
  #define LIBME_EXCEPTION_HPP

#include <stdarg.h>
#include <stdio.h>

namespace me {

  class Exception {

  public:

    [[nodiscard]] virtual const char* get_message() const = 0;
    [[nodiscard]] virtual const char* what() const throw() = 0;

  };

  class RuntimeError : public Exception {

  private:

    char message[1024];

  public:

    template<typename... Args>
    constexpr RuntimeError(const char* format, ...)
    {
      va_list args;
      va_start(args, format);
      vsprintf(message, format, args);
      va_end(args);
    }

    [[nodiscard]] const char* get_message() const override
    {
      return message;
    }
    
    [[nodiscard]] const char* what() const throw() override
    {
      return message;
    }

  };

}

#endif
