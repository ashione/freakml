#ifndef __FREAK_MATH_H__
#define __FREAK_MATH_H__

#include <freakConfig.h>

#include <Eigen/Dense>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <random>

using namespace Eigen;

#define PI 3.14159265

namespace freak{

template<class T>
void printF(T* mat,size_t n,size_t m) {
    for(size_t i=0;i<n;++i) {
        for(size_t j=0;j<m;++j) {
            printf("%f ",mat[i*m+j]);
        }
        printf("\n");
    }
}


template <class T>
void swap(T* A, T* B,const size_t size = 1)
{
    if (A == B)
        return;

    for(size_t i=0;i<size;++i) {
        T temp = A[i];
        A[i] = B[i];
        B[i] = temp;
    }
}

template <class T>
void randu(T* data,size_t size = 1, T s = 0.0, T e = 1.0)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(s, e);
    size_t i = 0;
    while ( i < size ) {
         data[i++] = dis(gen);
    }
}

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
T log(T t) {
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
T* diag(const size_t nRow,const size_t nCol,T* mat)
{
    MatrixXd mxd(nRow,nCol);

    for(size_t i=0;i<nRow;++i) {
        for(size_t j=0;j<nCol;++j) {
            mxd(i,j) = mat[i*nCol + j];
        }
    }
    SelfAdjointEigenSolver<MatrixXd> eigensolver(mxd);
    if (eigensolver.info() != Success){
        abort();
    }

    MatrixXd diagx = eigensolver.eigenvalues().asDiagonal();
    T* diag = new T[nCol * nRow];
    for(size_t i=0;i<nRow;++i) {
        for(size_t j=0;j<nCol;++j) {
           diag[i*nCol + j]  = diagx(i,j);
        }
    }
    return diag;

    /* 
    T* diagMat = new T[nRow*nCol];
    std::memcpy(diagMat,mat,sizeof(T)*nRow*nCol);

    for(size_t i=0;i<nRow; ++i) {

        T maxE = abs(diagMat[i*nCol+i]);
        size_t index = i;

        for(size_t ti = i+1;ti<nRow;++ti) {
            T scaleT = diagMat[ti*nCol+i];
            if (maxE<abs(scaleT))  {
                maxE = scaleT;
                index = ti;
            }
        }

        swap(diagMat+i*nCol,diagMat+index*nCol,nCol);
        for(size_t k=0;k<nRow;++k){
            T scale = diagMat[k*nCol + i] / diagMat[i*nCol+i] ;
            if ( k == i || abs(scale) < EPS ) continue;
            for(size_t j=0;j<nCol;++j) {
                diagMat[k*nCol + j] -= diagMat[i*nCol+j] * scale;
            }
        }
    }

    //printf("diagMat : \n");
    //printF(diagMat,nRow,nCol);
        

    return diagMat;
   */ 
}

template <class T>
T* eye(const size_t nRow,const size_t nCol)
{
    T* eyeMat = new T[nRow*nCol]; 
    std::fill(eyeMat,eyeMat+nRow*nCol,0.0);
    size_t mDim = nRow>nCol?nCol:nRow;
    for(size_t i=0;i<mDim;++i) {
        eyeMat[i*nCol + i] = 1.0;
    }
    return eyeMat;

}

template <class T>
void eye(const size_t nRow, const size_t nCol,T* eyeMat)
{
    std::fill(eyeMat,eyeMat+nRow*nCol,0.0);
    size_t mDim = nRow>nCol?nCol:nRow;
    for(size_t i=0;i<mDim;++i) {
        eyeMat[i*nCol + i] = 1.0;
    }
}

template <class T>
void inv(const size_t nRow, const size_t nCol, T* mat,T *data)
{
    MatrixXd mxd(nRow,nCol);

    for(size_t i=0;i<nRow;++i) {
        for(size_t j=0;j<nCol;++j) {
            mxd(i,j) = mat[i*nCol + j];
        }
    }
    MatrixXd mInv = mxd.inverse();
    for(size_t i=0;i<nRow;++i) {
        for(size_t j=0;j<nCol;++j) {
            data[i*nCol + j] = mInv(i,j);
        }
    }

}
template <class T>
void inverse(const size_t nRow, const size_t nCol, T* mat,T *data)
{
    assert(data);

    T* companyMat = data;

    eye(nRow,nCol,companyMat);
    //T* diagMat = diag(nRow,nCol,mat,companyMat);
    T* diagMat = new T[nRow*nCol];
    std::memcpy(diagMat,mat,sizeof(T) * nRow*nCol);
    //size_t* iPiv = new size_t[nRow];

    for(size_t i=0;i<nRow;++i) {
        T scale = diagMat[i*nCol+i];

        size_t index = i;

        for(size_t ti = i+1;ti<nRow;++ti) {
            T scaleT = diagMat[ti*nCol+i];
            if (abs(scale)<abs(scaleT))  {
                scale = scaleT;
                index = ti;
            }
        }

        swap(diagMat+i*nCol,diagMat+index*nCol,nCol);
        swap(companyMat+i*nCol,companyMat+index*nCol,nCol);
        //if (abs(scale ) < EPS) 
        //    continue;
        //

        for(size_t j=0;j<nCol;++j) {
            diagMat[i*nCol+j] /= scale;
            companyMat[i*nCol+j] /= scale;
        }

        for(size_t k=0;k<nRow;++k) {
            T coef= diagMat[k*nCol+i] / diagMat[i*nCol+i];
            if ( k == i || abs(coef) <EPS ) continue;
            //assert(abs(scale) > EPS);
            /* 
            if (abs(scale) <= EPS ) {
                scale = EPS;
            }
            */
            for(size_t j=0; j<nCol; j++) {
                diagMat[k*nCol+j] -= coef * diagMat[i*nCol+j];
                companyMat[k*nCol+j] -= coef * companyMat[i*nCol+j];
            }
        }
        /*
        printf("mat: %d\n",i);
        printF(diagMat,nRow,nCol);
        printf("company: %d\n",i);
        printF(companyMat,nRow,nCol);
        */
    }

    delete[] diagMat;
}


//matrix-inverse-row-operations-gauss-jordan
template <class T>
T* inv(const size_t nRow, const size_t nCol, T* mat)
{
    T* companyMat = new T[nRow*nCol];
    inv(nRow,nCol,mat,companyMat);
    return companyMat;
}

template <class T>
void mul(const size_t nRow,const size_t nCol, const size_t tCol, T* A, T* B,T* C) {

    T* result = C;

    for(size_t i=0;i<nRow;++i) {
        for(size_t k=0;k<tCol;++k) {
            size_t index = i*tCol+k;
            result[index] = 0;
            for(size_t j=0;j<nCol;j++) {
                result[index] += A[i*nCol+j] * B[j*tCol+k];
            }
        }
    }
}


template <class T>
T* mul(const size_t nRow,const size_t nCol, const size_t tCol, T* A, T* B) {

    T* result = new T[nRow*tCol];
    mul(nRow,nCol,tCol,A,B,result);
    return result;
}

template <class T>
void dot(const size_t size, T* A, T* B,T* C) {

    for(size_t i=0;i<size;++i) {
        C[i] = A[i] * B[i];
    }

}

template <class T>
void div(const size_t size, T* A, T* B,T* C) {

    for(size_t i=0;i<size;++i) {
        C[i] = A[i] / B[i];
    }

}

template <class T>
T det(const size_t nRow,const size_t nCol, T* mat,bool isDiag = false)
{
    T* diagMat = mat; 
    if (!isDiag) {
        diagMat =diag(nRow,nCol,mat);
    }
    size_t mDim = std::min(nRow,nCol);
    T result = 1;
    for(size_t i=0;i<mDim;++i) {
       result *= diagMat[i*nCol + i];
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
    /*
    printf("matCorr\n");
    printF(matCorr,nCol,nCol);
    */

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

template <class T>
size_t* argSort(T* A, const size_t nRow)
{
    std::vector<std::pair<T,size_t> > idx(nRow);

    for(size_t i=0;i<nRow;++i) {
        idx[i] = std::make_pair(A[i],i);
    }

    auto argCompare = [](std::pair<T,size_t>& a,std::pair<T,size_t>& b)
    {
        return a.first < b.first;
    };

    std::sort(idx.begin(),idx.end(),argCompare);

    size_t* argIndex = new size_t[nRow];
    for(size_t i=0;i<nRow;++i){
        argIndex[idx[i].second] = i;
    }

    return argIndex;
}

template <class T>
T* spearmanCorrCoef(const size_t nRow, const size_t nCol, T* mat)
{
    T* matCorr = new T[nCol*nCol];
    size_t** argIndexCols = new size_t* [nCol];
    T* data = new T[nRow];
    for(size_t i=0;i<nCol;++i) {
        for(size_t j=0;j<nRow;++j) 
            data[j] = mat[j*nCol + i];
        argIndexCols[i] = argSort(data,nRow);
    }

    for(size_t i=0;i<nCol;++i) {
        matCorr[i*nCol +i] = 1.0;
        for(size_t j=i+1;j<nCol;++j) {
            T s=0.0;
            for(size_t k=0;k<nRow;++k) {
                T t = argIndexCols[i][k]*1.0 - argIndexCols[j][k];
                s+=t*t;
            }
            matCorr[i*nCol+j] = 1.0 - s*6.0/(nRow*(nRow*nRow-1));
            matCorr[j*nCol+i] = matCorr[i*nCol + j];

        }
    }

    return matCorr;
}

}

#endif /* __FREAK_MATH_H__ */
