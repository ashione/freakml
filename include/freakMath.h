#ifndef __FREAK_MATH_H__
#define __FREAK_MATH_H__

#include <freakConfig.h>

#include <algorithm>

namespace freak {

template <class T>
void exp(T &t) {
    t = std::exp(t);
}

template <class T>
void log(T &t) {
    t = std::log(t);
}

}

#endif /* __FREAK_MATH_H__ */
