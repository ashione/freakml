#ifndef __FREAK_DATUM_H__
#define __FREAK_DATUM_H__

#include <freakConfig.h>

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>


namespace freak {

template <class T = DATUM_TYPE>
class FreakVector { 

    private :
        std::vector<T> x;
    public :
        std::vector<std::size_t> dims;

        typedef void (*FreakFunc)(T &t);

        typedef typename std::vector<T>::iterator Iterator;

    public :

        FreakVector();

        FreakVector(std::size_t size);

        FreakVector(std::size_t size, T v);

        FreakVector(const FreakVector<T>& t);

        inline T& operator[](std::size_t index) {
            assert(index < this->size() && index >= 0);
            return x.at(index);
        };

        inline FreakVector<T> operator+ (const FreakVector<T>& t) const ;

        inline FreakVector<T> operator- (const FreakVector<T>& t) const;

        inline FreakVector<T> operator* (const FreakVector<T>& t) const;

        inline FreakVector<T> operator/ (const FreakVector<T>& t) const;

        inline FreakVector<T> operator+ (const T t) const;

        inline FreakVector<T> operator- (const T t) const;

        inline FreakVector<T> operator* (const T t) const;

        inline FreakVector<T> operator/ (const T t) const;

        inline T operator[](std::size_t index) const {
            assert(index < this->size() && index >= 0);
            return x[index];
        };

        inline T getByIndex(std::size_t index) const {
            assert(index < this->size() && index >= 0);
            return x[index];
        }

        inline void set(std::size_t index, const T& t) {
            x[index] = t;
        };

        inline std::size_t size() const  {
            return x.size();
        }

        inline std::vector<T> getData() const {
            return x;
        }

        inline void push_back(T t) {
            this->x.push_back(t);
        }

        inline Iterator begin() {
            return this->x.begin();
        };

        inline Iterator end() {
            return this->x.end();
        };

        void add(const FreakVector<T>& t);

        void add(const T& t);

        virtual T prod(FreakVector<T>&  t) const;

        T distance(const FreakVector<T>& t) const;

        void mul(const T& t);

        void apply(FreakFunc func);

        T sum();

        T* ptr(size_t i = 0);

        virtual ~FreakVector(){};
};


typedef FreakVector<DATUM_TYPE> FreakVectorF;

void printD (const FreakVectorF& t);

template <class T>
FreakVector<T>::FreakVector() {
    dims.push_back(1);
}

template <class T>
FreakVector<T>::FreakVector(std::size_t size) {
    x = std::vector<T>(size);
    std::fill(x.begin(),x.end(),0.0);
}

template <class T>
FreakVector<T>::FreakVector(std::size_t size,T v) {
    x = std::vector<T>(size);
    std::fill(x.begin(),x.end(),v);
}

template <class T>
FreakVector<T>::FreakVector(const FreakVector<T>& t)
{
    this->dims = t.dims;
    this->x = t.getData();
}

template <class T> 
void FreakVector<T>::add(const FreakVector<T>& t)
{
    for(size_t i=0; i<t.size(); ++i){
        x[i] += t[i];
    }
}

template <class T>
T FreakVector<T>::prod(FreakVector<T>& t) const
{
    assert(t.size() == this->size());
    return std::inner_product(x.begin(),x.end(),t.begin(),0.0);
}

template <class T>
T FreakVector<T>::distance(const FreakVector<T>& t) const
{
    assert(t.size() == this->size());
    T result = 0;
    for(size_t i=0;i<size();i++) {
        result += std::pow(x[i] - t[i],2.0);
    }
    return static_cast<T>(std::pow(result,0.5));
}

template <class T>
void FreakVector<T>::add(const T& t)
{
    assert(t.size() == this->size());
    for(Iterator it=x.begin();x!=x.end(); it++) {
        it += t;
    }
}

template <class T>
void FreakVector<T>::mul(const T& t){
    for(size_t i=0;i<size();++i) {
        x[i] *= t;
    }
}

template <class T>
T FreakVector<T>::sum(){
    return std::accumulate(this->begin(), this->end(), 0.0);
}

template <class T>
void FreakVector<T>::apply(FreakVector<T>::FreakFunc func) {
    std::for_each(x.begin(),x.end(),func);
}

template <class T>
FreakVector<T> FreakVector<T>::operator+(const FreakVector<T>& t) const
{
    assert(this->size() == t.size());
    FreakVector<T> result(t.size());
    for(size_t i = 0; i<t.size(); i++) {
        result[i] = x[i] + t[i];
    }
    return result;
}

template <class T>
FreakVector<T> FreakVector<T>::operator-(const FreakVector<T>& t) const
{
    assert(this->size() == t.size());
    FreakVector<T> result(t.size());
    for(size_t i = 0; i<t.size(); i++) {
        result[i] = x[i] - t[i];
    }
    return result;
}

template <class T>
FreakVector<T> FreakVector<T>::operator*(const FreakVector<T>& t) const
{
    assert(this->size() == t.size());
    FreakVector<T> result(t.size());
    for(size_t i = 0; i<t.size(); i++) {
        result[i] = x[i] * t[i];
    }
    return result;
}

template <class T>
FreakVector<T> FreakVector<T>::operator/(const FreakVector<T>& t) const
{
    assert(this->size() == t.size());
    FreakVector<T> result(t.size());
    for(size_t i = 0; i<t.size(); i++) {
        result[i] = x[i] / t[i];
    }
    return result;
}

template <class T>
FreakVector<T> FreakVector<T>::operator+(const T t) const
{
    FreakVector<T> result(x.size());
    for(size_t i = 0; i<x.size(); i++) {
        result[i] = x[i] + t;
    }
    return result;
}

template <class T>
FreakVector<T> FreakVector<T>::operator-(const T t) const
{
    FreakVector<T> result(x.size());
    for(size_t i = 0; i<x.size(); i++) {
        result[i] = x[i] - t;
    }
    return result;
}

template <class T>
FreakVector<T> FreakVector<T>::operator*(const T t) const
{
    FreakVector<T> result(x.size());
    for(size_t i = 0; i< x.size(); i++) {
        result[i] = x[i] * t;
    }
    return result;
}

template <class T>
FreakVector<T> FreakVector<T>::operator/(const T t) const
{
    FreakVector<T> result(x.size());
    for(size_t i = 0; i<x.size(); i++) {
        result[i] = x[i] / t;
    }
    return result;
}

template <class T>
T* FreakVector<T>::ptr(size_t i) 
{
    return &x[0] + i;
}

template <class T>
void printD(const FreakVector<T>& t){
    for(size_t i=0; i<t.size(); ++i)  {
        ILOG <<t.getByIndex(i);
    }
}

}

#endif /* __FREAK_DATUM_H */
