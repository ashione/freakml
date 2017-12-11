#ifndef __FREAK_MATRIX_H__
#define __FREAK_MATRIX_H__

#include <freakConfig.h>
#include <freakDatum.h>
#include <freakMath.h>

#include <vector>
#include <cstring>
#include <iostream>

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
        FreakMat() { nRow=0; nCol=0; };

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

        FreakMat<T> dot(FreakVector<T>& t);

        FreakMat<T> dot(T t);

        FreakMat<T> div(FreakMat<T>& t);

        FreakMat<T> div(FreakVector<T>& t, size_t axis = 0);

        FreakMat<T> sum(size_t axis);

    public :
        static FreakMat<T> diag(FreakVector<T>& t);

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
        for(size_t j=0;j<nCol;++j) {
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
        for(size_t j=0;j<nCol;++j) {
            result.at(i,j) = at(i,j) - t[j];
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
        for(size_t j=0;j<nCol;++j) {
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
        for(size_t j=0;j<nCol;++j) {
            result.at(i,j) = x.at(i,j) - t.at(i,j);
        }
    }

    return result;
}

template <class T>
FreakMat<T> FreakMat<T>::operator* (FreakMat<T>& t)
{
    assert(t.nRow == nCol);

    FreakMat<T> result(nRow,t.nCol);
    mul(nRow,nCol,t.nCol,this->ptr(),t.ptr(),result.ptr());

    return result;
}

template <class T>
FreakVector<T> FreakMat<T>::row(size_t i)
{
    FreakVector<T> matRow(nCol);
    std::memcpy(matRow.ptr(),ptr(i,0),sizeof(T) * nCol);
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
    return result;
}

template <class T>
FreakMat<T> FreakMat<T>::div(FreakMat<T>& t)
{
    FreakMat<T> result(nRow,nCol);
    freak::div(size(),ptr(),t.ptr(),result.ptr());
    return result;
}


template <class T>
FreakMat<T> FreakMat<T>::dot(T t)
{
    FreakMat<T> result(nRow,nCol);
    for(size_t i=0;i<nRow;++i) {
        for(size_t j=0;j<nCol;++j) {
            result.at(i,j) = at(i,j) * t;
        }
    }
    return result;
}

template <class T>
FreakMat<T> FreakMat<T>::dot(FreakVector<T>& t)
{
    FreakMat<T> result(nRow,nCol);
    for(size_t i=0;i<nRow;++i) {
        freak::dot(nCol,ptr(i,0),t.ptr(),result.ptr(i,0));
    }
    return result;
}

template <class T>
FreakMat<T> FreakMat<T>::div(FreakVector<T>& t, size_t axis)
{
    FreakMat<T> result(nRow,nCol);
    if (axis == 0) {
        assert( nCol == t.size() );
        for(size_t i=0;i<nRow;++i) {
            freak::div(nCol,ptr(i,0),t.ptr(),result.ptr(i,0));
        }
    } else if (axis == 1) {
        assert( nRow == t.size() );
        for(size_t i=0;i<nRow;++i) {
            for(size_t j=0;j<nCol;++j) {
                result.at(i,j) = at(i,j) / t[i];
            }
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
        mCol = nCol;
    } else {
        mRow = nRow;
        mCol = 1;
    }

    FreakMat<T> result(mRow,mCol);
    if (axis == 0) {
        for(size_t j=0;j<nCol;++j) {
            T temp = 0;
            for(size_t i=0;i<nRow;++i) {
                temp += at(i,j);
            }
            result.at(0,j) = temp;
        }

    } else {
        for(size_t i=0;i<nRow;++i) {
            T temp = 0;
            for(size_t j=0;j<nCol;++j) {
                temp += at(i,j);
            }
            result.at(i,0) = temp;
        }
    }
    return result;

}

template <class T>
FreakMat<T> FreakMat<T>::diag(FreakVector<T>& t)
{
    assert(t.size() >0 );
    FreakMat<T> result(t.size(),t.size());
    std::fill(result.ptr(),result.ptr(result.nRow-1,result.nCol-1),0);
    for(size_t i = 0;i<result.nRow; ++i) {
        result.at(i,i) = t[i];
    }
    return result;
}

typedef FreakMat<float> FreakMatF;

}

#endif /* __FREAK_MATRIX_H__ */
