#ifndef __FREAK_DATUM_H__
#define __FREAK_DATUM_H__

#include <vector>

template <class T>
class Data { 
    public :
        std::vector<T> x;

    public :
        void add(const Data<T>& t);
        void add(const T& t);
        T prod(const Data<T>&  t);
        void mul(const T& t);
};

template <class T> 
void Data<T>::add(const Data<T>& t) {
    for(int i=0;i<t.x.size();++i){
        this->x[i]+=t.x[i];
    }
}

template <class T>
T Data<T>::prod(const Data<T>& t){
    T result = 0;
    for(int i=0;i<t.x.size();++i) {
        result += this->x[i] * t.x[i];
    }
    return result;
}

template <class T>
void Data<T>::add(const T& t){
    for(int i=0;i<this->x.size();++i) {
        this->x[i] += t;
    }
}

template <class T>
void Data<T>::mul(const T& t){
    for(int i=0;i<this->x.size();++i) {
        this->x[i] *= t;
    }
}

void printD (const Data<float>& t);

#endif /* __FREAK_DATUM_H */
