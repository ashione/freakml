#ifndef __FREAK_WEIGHT_H__
#define __FREAK_WEIGHT_H__

#include <freakConfig.h>
#include <freakDatum.h>

namespace freak{

template <class T = DATUM_TYPE >
class FreakWeight : public FreakVector<T> {
    private :
        T b;

    public :

        FreakWeight(const FreakVector<T>& freakVector,const T& b_) : FreakVector<T>(freakVector),b(b_){} ;
        FreakWeight(const FreakVector<T>& freakVector) :
            FreakVector<T>(freakVector) {
            b = 1.0;

        };

        FreakWeight(const FreakWeight<T>& freakWeight) : 
            FreakVector<T>(freakWeight){
            b = freakWeight.getB();
        };

        FreakWeight(const std::size_t size) : 
            FreakVector<T>(size){
            b = 1.0;
        };

        inline T& getB();
        inline void setB(const T);

        T prod(FreakVector<T>& t);



};

template<class T>
T& FreakWeight<T>::getB()
{
    return b;
}

template<class T>
void FreakWeight<T>::setB(const T b_)
{
   b = b_; 
}

template<class T>
T FreakWeight<T>::prod(FreakVector<T>& t)
{
    return t.prod(*this) + b;
}

}

#endif /* __FREAK_WEIGHT_H__ */
