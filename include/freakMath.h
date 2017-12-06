#ifndef __FREAK_MATH_H__
#define __FREAK_MATH_H__

#include <freakConfig.h>

#include <algorithm>
#include <cmath>
#include <cstdio>

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

template<class T>
T* diag(const size_t nRow,const size_t nCol,T* mat,T* companyMat=NULL)
{
    T* diagMat = new T[nRow*nCol];
    std::memcpy(diagMat,mat,sizeof(T)*nRow*nCol);

    /*
    if (companyMat) {
        assert(sizeof(companyMat) == nRow*nCol*sizeof(T));
    }
    */

    for(size_t i=1;i<nRow; ++i) {
        T scale = diagMat[i*nCol + i-1] / diagMat[(i-1)*nCol+i-1] ;
        assert(abs(scale) > EPS);
        for(size_t j=i-1;j<nCol;++j) {
            diagMat[i*nCol + j] -= diagMat[(i-1)*nCol+j] * scale;
            if (companyMat) {
                companyMat[i*nCol+j] -= companyMat[(i-1)*nCol+j] * scale;
            }
        }
    }

    return diagMat;
}

template <class T>
T* eye(const size_t nRow, const size_t nCol, T* mat=NULL)
{
    T* eyeMat = mat;
    if (eyeMat) {
        eyeMat = new T[nRow*nCol]; 
    }
    std::fill(eyeMat,eyeMat+nRow*nCol,0.0);
    size_t mDim = nRow>nCol?nCol:nRow;
    for(size_t i=0;i<mDim;++i) {
        eyeMat[i*nCol + i] = 1.0;
    }
    return eyeMat;

}

template<class T>
void printF(T* mat,size_t n,size_t m) {
    for(size_t i=0;i<n;++i) {
        for(size_t j=0;j<m;++j) {
            printf("%f ",mat[i*m+j]);
        }
        printf("\n");
    }
}

//matrix-inverse-row-operations-gauss-jordan
template <class T>
T* inv(const size_t nRow, const size_t nCol, T* mat)
{
    T* companyMat = new T[nRow*nCol];

    companyMat = eye(nRow,nCol,companyMat);

    T* diagMat = diag(nRow,nCol,mat,companyMat);

    for(size_t i=0;i<nRow;++i) {
        T scale = diagMat[i*nCol+i];

        for(size_t j=i;j<nCol;++j) {
            diagMat[i*nCol+j] /= scale;
        }
        for(size_t j=0;j<nCol;++j) {
            companyMat[i*nCol+j] /= scale;
        }
    }
    for(size_t i= 0; i<nRow-1; i++) {
        T scale = diagMat[i*nCol+i+1] / diagMat[(i+1)*nCol + i+1];
        assert(abs(scale) > EPS);
        for(size_t j = i+1; j<nCol; j++) {
            diagMat[i*nCol+j] -= scale * diagMat[(i+1)*nCol+j];
        }

        for(size_t j=0;j<nCol;j++) {
            companyMat[i*nCol+j] -= companyMat[(i+1)*nCol+j] * scale;

        }

    }
    delete[] diagMat;
    return companyMat;
}

template <class T>
T* mul(const size_t nRow,const size_t nCol, const size_t tCol, T* A, T* B) {
    T* result = new T[nRow*tCol];
    for(size_t i=0;i<nRow;++i) {
        for(size_t k=0;k<tCol;++k) {
            size_t index = i*tCol+k;
            result[index] = 0;
            for(size_t j=0;j<nCol;j++) {
                result[index] += A[i*nCol+j] * B[j*tCol+k];
            }
        }
    }
    return result;
}

template <class T>
T det(const size_t nRow,const size_t nCol, T* mat,bool isDiag = false)
{
    T* diagMat = mat; 
    if (!isDiag) {
        diagMat =diag(nRow,nCol,mat);
    }
    size_t mDim = std::min(nRow,nCol);
    T result = 0;
    for(size_t i=0;i<mDim;++i) {
       result += diagMat[i*nCol + i];
    }

    return result;
}

template <class T>
T* mean(const size_t nRow, const size_t nCol, T* mat, bool isCol = false)
{
    size_t mCol = nCol;
    if (isCol) {
        mCol = nRow;
    }
    T* mu = new T[mCol];
    if (!isCol) {
        for(size_t i = 0; i<nRow; ++i) {
            mu[i] = 0;
            for(size_t j=0; j<nCol; ++j) {
                mu[i] += mat[i*nCol + j];
            }
            mu[i]/=nCol;
        }
    } else {
        for(size_t i = 0; i<nCol; ++i) {
            mu[i] = 0;
            for(size_t j=0; j<nRow; ++j) {
                mu[i] += mat[j * nCol + i];
            }
            mu[i]/=nRow;
        }

    }    
    return mu;
}

template <class T>
T* standard(const size_t nRow, const size_t nCol, T* mat,bool isCol = false)
{
    size_t mCol = nCol;
    if (isCol) {
        mCol = nRow;
    }
    T* mu = new T[mCol];
    T* stad = new T[mCol];
    if (!isCol) {
        for(size_t i = 0; i<nRow; ++i) {
            mu[i] = 0;
            for(size_t j=0; j<nCol; ++j) {
                mu[i] += mat[i*nCol + j];
            }
            mu[i]/=nCol;
            stad[i] = 0;
            for(size_t j=0; j<nCol;++j) {
                stad[i] += pow(mat[i*nCol +j] - mu[i],2.0);
            }
            stad[i] = sqrt(stad[i]/nCol);
        }
    } else {
        for(size_t i = 0; i<nCol; ++i) {
            mu[i] = 0;
            for(size_t j=0; j<nRow; ++j) {
                mu[i] += mat[j * nCol + i];
            }
            mu[i]/=nRow;

            stad[i] = 0;
            for(size_t j=0; j<nRow;++j) {
                stad[i] += pow(mat[j*nCol +i] - mu[i],2.0);
            }
            stad[i] = sqrt(stad[i]/nRow);
        }

    }    
    delete[] mu;
    return stad;
}

template <class T>
T* corr(const size_t nRow, const size_t nCol, T* mat, T* mu = NULL)
{
    assert(nRow >= 2 && nCol >=2);
    T* matMu = mu;

    if (!matMu) {
        matMu = mean(nRow,nCol,mat,true);
    }

    //printF(matMu,1,nCol);

    T* matCorr = new T[nCol*nCol];
    for(size_t i = 0;i<nCol; ++i) {
        for(size_t j=0; j<nCol;++j) {
            size_t index = i*nCol +j;
            matCorr[index] = 0;
            for(size_t k=0; k<nRow; ++k) {
                matCorr[index] += (mat[k*nCol + i ] - matMu[i]) * (mat[k*nCol + j] - matMu[j]);
            }
            matCorr[index]/=(nRow-1);

        }
    }
    if (!mu) {
        delete[] matMu;
    }
    return matCorr;

}

template <class T>
T* corrCoef(const size_t nRow, const size_t nCol, T* mat, T* mu = NULL)
{
    assert(nRow >= 2 && nCol >=2);
    T* matMu = mu;

    if (!matMu) {
        matMu = mean(nRow,nCol,mat,true);
    }
    T* matStd = standard(nRow,nCol,mat,true);

    //printF(matMu,1,nCol);
    //printF(matStd,1,nCol);

    T* matCorr = new T[nCol*nCol];
    for(size_t i = 0;i<nCol; ++i) {
        for(size_t j=0; j<nCol;++j) {
            size_t index = i*nCol +j;
            matCorr[index] = 0;
            for(size_t k=0; k<nRow; ++k) {
                matCorr[index] += (mat[k*nCol + i ] - matMu[i]) * 
                    (mat[k*nCol + j] - matMu[j])/matStd[i]/matStd[j];
            }
            matCorr[index]/=(nRow-1);

        }
    }
    if (!mu) {
        delete[] matMu;
    }

    delete[] matStd;

    return matCorr;

}

}

#endif /* __FREAK_MATH_H__ */
