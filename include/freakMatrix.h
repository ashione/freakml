#ifndef __FREAK_MATRIX_H__
#define __FREAK_MATRIX_H__

#include <freakConfig.h>
#include <freakDatum.h>
#include <freakMath.h>

#include <vector>

namespace freak {

template <class T = DATUM_TYPE >
class FreakMat {
    
    public :
        std::vector<T> x;

    public :
        size_t nRow;
        size_t nCol;

    public :
        FreakMat(size_t nRow_, size_t nCol_);
        FreakMat(const FreakMat<T>& t);

        inline T& at(size_t i, size_t j);
        inline T* ptr(size_t i = 0,size_t j = 0);
        FreakMat transpose();

        inline FreakMat<T> operator+ (FreakVector<T>& t);

        inline FreakMat<T> operator- (FreakVector<T>& t);

        inline FreakMat<T> operator+ (FreakMat<T>& t);

        inline FreakMat<T> operator- (FreakMat<T>& t);

        inline FreakMat<T> operator* (FreakMat<T>& t);

};

template <class T>
FreakMat<T>::FreakMat(size_t nRow_,size_t nCol_)
{
    nRow = nRow_;
    nCol = nCol_;
    x = std::vector<T>(nRow * nCol);
}

template <class T>
FreakMat<T>::FreakMat(const FreakMat<T>& t)
{
    nRow = t.nRow;
    nCol = t.nCol;
    x = t.x;
}

template <class T>
T& FreakMat<T>::at(size_t i, size_t j) 
{
    return x.at(i*nCol+j);
}

template <class T>
T* FreakMat<T>::ptr(size_t i, size_t j) 
{
    return &x[0] + i*nCol + j;
}

template <class T>
FreakMat<T> FreakMat<T>::transpose() 
{
    FreakMat transMat(nCol,nRow);
    for(size_t i=0;i<nRow;++i) {
        for(size_t j=0;j<nCol;++j) {
            transMat.at(j,i) = this->at(i,j);
        }
    }

    return transMat;
}

template <class T>
FreakMat<T> FreakMat<T>::operator+ (FreakVector<T>& t)
{
    assert(t.size() == nCol);
    FreakMat<T> result(nRow,nCol);

    for(size_t i = 0; i<nRow; ++i) {
        for(size_t j=0;j<nCol;++i) {
            result.at(i,j) = x.at(i,j) + t[j];
        }
    }

    return result;
}

template <class T>
FreakMat<T> FreakMat<T>::operator- (FreakVector<T>& t)
{
    assert(t.size() == nCol);
    FreakMat<T> result(nRow,nCol);

    for(size_t i = 0; i<nRow; ++i) {
        for(size_t j=0;j<nCol;++i) {
            result.at(i,j) = x.at(i,j) - t[j];
        }
    }

    return result;

}

template <class T>
FreakMat<T> FreakMat<T>::operator+ (FreakMat<T>& t)
{
    assert(t.nCol == nCol && t.nRow == nRow);

    FreakMat<T> result(nRow,nCol);
    for(size_t i = 0; i<nRow; ++i) {
        for(size_t j=0;j<nCol;++i) {
            result.at(i,j) = x.at(i,j) + t.at(i,j);
        }
    }

    return result;

}

template <class T>
FreakMat<T> FreakMat<T>::operator- (FreakMat<T>& t)
{
    assert(t.nCol == nCol && t.nRow == nRow);

    FreakMat<T> result(nRow,nCol);
    for(size_t i = 0; i<nRow; ++i) {
        for(size_t j=0;j<nCol;++i) {
            result.at(i,j) = x.at(i,j) - t.at(i,j);
        }
    }

    return result;
}

template <class T>
FreakMat<T> FreakMat<T>::operator* (FreakMat<T>& t)
{
    assert(t.nRow == nCol);

    FreakMat<T> result(nRow,nCol);
    mul(nRow,nCol,t.nCol,this->ptr(),t.ptr(),result.ptr());

    return result;
}

}

#endif /* __FREAK_MATRIX_H__ */
