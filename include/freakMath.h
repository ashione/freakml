#ifndef __FREAK_MATH_H__
#define __FREAK_MATH_H__

#include <freakConfig.h>

#include <algorithm>
#include <cmath>

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

template <class T, class B>
T exp(const T& t, B isArray) {
    T result(t);
    for(size_t i=0;i<t.size();++i) {
        result[i] = std::exp(result[i]);
    }
    return result;
}

template <class T>
T sqrt(T t) {
    return std::sqrt(t);
}

template <class T, class B>
T sqrt(const T& t, B isArray) {
    T result(t);
    for(size_t i=0;i<t.size();++i) {
        result[i] = std::sqrt(result[i]);
    }
    return result;
}

template <class T>
void log(T &t) {
    t = std::log(t);
}

template <class T>
T log(T &t) {
    return std::log(t);
}


template <class T>
T abs(T& t)
{
    return std::abs(t);
}

template <class T,class B>
T abs(T& t,B isArray)
{
    T result(t);
    for(size_t i=0;i<t.size();++i) {
        result[i] = abs(result[i]);
    }
    return result;
}

template <class T>
T log(const T t) {
    return std::log(t);
}

template <class T>
T gaussian(const T t,const T mu,const T sigmaSquare)
{
   return 1.0/std::sqrt(2*PI*sigmaSquare) *
       exp(-(t-mu)*(t-mu)/(sigmaSquare*2.0));
}

template <class T1, class T2>
T2 gaussian(const T1& t,const T1& mu,const T2& sigmaSquare)
{
   float param = 2.0*PI;
   T2 tempS = 1.0/sqrt(sigmaSquare*param);
   T2 tempD = t.distance(mu);
   T2 result = exp(std::log(tempS) + (tempD*tempD/(sigmaSquare*-2.0)));
   return result;
}

}

#endif /* __FREAK_MATH_H__ */
