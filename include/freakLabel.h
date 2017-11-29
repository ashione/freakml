#ifndef __FREAK_LABEL_H__
#define __FREAK_LABEL_H__

#include <freakConfig.h>

namespace freak {

template <class T = LABEL_TYPE>
class FreakLabel {

    private :
        T y;

    public :

        FreakLabel();

        FreakLabel(const T& t);

        FreakLabel(const FreakLabel<T>& t);

        bool isCorrect(const T& t);

        float loss(const T& t);

        bool isCorrect(const FreakLabel<T>& t);

        float loss(const FreakLabel<T>& t);

        inline T getLabel() const;
};

template <class T>
FreakLabel<T>::FreakLabel(const T& t)
{
    this->y = t;
}

template <class T>
FreakLabel<T>::FreakLabel(const FreakLabel<T>& t)
{
    this->y = t.getLabel();
}

template <class T>
bool FreakLabel<T>::isCorrect(const T& t)
{
    return y == t;
}

template <class T>
float FreakLabel<T>::loss(const T& t)
{
    return std::abs(t - y);
}

template <class T>
bool FreakLabel<T>::isCorrect(const FreakLabel<T>& t)
{
    return y == t.y;
}

template <class T>
float FreakLabel<T>::loss(const FreakLabel<T>& t)
{
    return std::abs(t.y - y);
}

template <class T>
T FreakLabel<T>::getLabel() const
{
    return this->y;
}

typedef FreakLabel<LABEL_TYPE> FreakLabelC;

typedef FreakLabel<float> FreakLabelF;

}

#endif /* __FREAK_LABEL_H__ */
