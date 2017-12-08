#ifndef __FREAK_MATRIX_H__
#define __FREAK_MATRIX_H__

#include <freakConfig.h>
#include <freakDatum.h>
#include <freakMath.h>

#include <vector>
#include <cstring>

namespace freak {

template <class T = DATUM_TYPE >
class FreakMat {
    
    public :
        std::vector<T> x;

    public :
        size_t nRow;
        size_t nCol;

    public :
        FreakMat(size_t nRow_, size_t nCol_, T* data = NULL);
        FreakMat(const FreakMat<T>& t);

        inline T& at(size_t i, size_t j);
        inline T* ptr(size_t i = 0,size_t j = 0);
        inline size_t size();
        FreakMat transpose();

        inline FreakMat<T> operator+ (FreakVector<T>& t);

        inline FreakMat<T> operator- (FreakVector<T>& t);

        inline FreakMat<T> operator+ (FreakMat<T>& t);

        inline FreakMat<T> operator- (FreakMat<T>& t);

        inline FreakMat<T> operator* (FreakMat<T>& t);

        FreakVector<T> row(size_t i);

        FreakVector<T> col(size_t i);

        FreakMat<T> dot(FreakMat<T>& t);

        FreakMat<T> dot(T t);

        FreakMat<T> sum(size_t axis);

};

template <class T>
FreakMat<T>::FreakMat(size_t nRow_,size_t nCol_,T* data)
{
    nRow = nRow_;
    nCol = nCol_;
    x = std::vector<T>(nRow * nCol);

    if (data) {
        std::memcpy(ptr(),data,sizeof(T)*size());
    }
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
size_t FreakMat<T>::size() 
{
    return nRow * nCol;
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

template <class T>
FreakVector<T> FreakMat<T>::row(size_t i)
{
    FreakVector<T> matRow(nCol);
    std::memcpy(matRow.ptr(),this->at(i,0),sizeof(T) * nCol);
    return matRow;
}

template <class T>
FreakVector<T> FreakMat<T>::col(size_t j)
{
    FreakVector<T> matCol(nRow);
    for(size_t i=0;i<nRow;++i) {
        matCol[i] = this->at(i,j);
    }
    return matCol;
}

template <class T>
FreakMat<T> FreakMat<T>::dot(FreakMat<T>& t)
{
    FreakMat<T> result(nRow,nCol);
    freak::dot(size(),ptr(),t.ptr(),result.ptr());
}

template <class T>
FreakMat<T> FreakMat<T>::dot(T t)
{
    FreakMat<T> result(nRow,nCol);
    for(size_t i=0;i<nRow;++i) {
        for(size_t j=0;i<nCol;++j) {
            result.at(i,j) = at(i,j) * t;
        }
    }
    return result;
}

template <class T>
FreakMat<T> FreakMat<T>::sum(size_t axis)
{
    size_t mRow = 0, mCol = 0;
    if (axis == 0) {
        mRow = 1;
    } else {
        mRow = nRow;
        mCol = 1;
    }

    FreakMat<T> result(mRow,mCol);
    if (axis == 0) {
        for(size_t i=0;i<nRow;++i) {
            T temp = 0;
            for(size_t j=0;j<nCol;++j) {
                temp += at(i,j);
            }
            result.at(0,i) = temp;
        }
    } else {
        for(size_t j=0;j<nCol;++j) {
            T temp = 0;
            for(size_t i=0;i<nRow;++i) {
                temp += at(i,j);
            }
            result.at(j,0) = temp;
        }

    }
    return result;

}

}

#endif /* __FREAK_MATRIX_H__ */
