#ifndef __FREAK_MATH_H__
#define __FREAK_MATH_H__

#include <freakConfig.h>

#include <algorithm>

#define PI 3.14159265

namespace freak{

template <class T>
void exp(T &t) {
    t = std::exp(t);
}

template <class T>
T exp(T t) {
    return std::exp(t);
}

template <class T>
void log(T &t) {
    t = std::log(t);
}

template <class T>
T log(const T t) {
    return std::log(t);
}

template <class T>
T gaussian(const T t,const T mu,const T sigmaSquare)
{
   return 1.0/std::sqrt(2*PI*sigmaSquare) *
       exp(- (t-mu)*(t-mu)/(2.0*sigmaSquare));
}

}

#endif /* __FREAK_MATH_H__ */
