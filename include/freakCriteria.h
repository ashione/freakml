#ifndef __FREAK_CRITERIA_H__
#define __FREAK_CRITERIA_H__

#include <freakConfig.h>
#include <freakMath.h>

namespace freak {

template <class T>
T entropy(T* p, UInt k)
{
    T sumE = 0.0;
    for(UInt i=0;i<k;++i) {
        T pt = *(p+i);
        sumE += -pt*freak::log(pt);
    }
    return sumE;
}

template <class T>
T gini(T*p, UInt k)
{
    T giniV = 1.0;

    for(UInt i=0;i<k;++i) {
        T pt = *(p+i);
        giniV -= pt*pt;
    }

    return giniV;
}

}


#endif /* __FREAK_CRITERIA_H__ */
