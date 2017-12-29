#include <exception>
#include <string>

namespace freak {

#define FreakException(name) \
class Freak ##name : public std::exception \
{                                     \
    private :                           \
        std::string message;                 \
    public :                          \
        Freak ##name(const char* msg) : message(msg) {}; \
        const char * what () const throw() \
        {                                   \
              return message.c_str();     \
        }                                   \
};

FreakException(ThreadException)
FreakException(ThreadInitException)
FreakException(ThreadPoolInitException)
FreakException(ThreadPoolExpandException)
FreakException(ThreadPoolException)

}
