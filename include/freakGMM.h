#ifndef __FREAK_GMM_H__
#define __FREAK_GMM_H__

#include <freakConfig.h>
#include <freakEM.h>
#include <freakDatum.h>
#include <freakMatrix.h>
#include <freakMath.h>
#include <random>
#include <iostream>

namespace freak {


template <class T = DATUM_TYPE>
class FreakGMM : public FreakEM
{
    public  :
        FreakMat<T>* records;
        std::vector<FreakMat<T> > sigma2;
        FreakMat<T> mu;
        FreakVector<T> alpha;
        FreakMat<T> r;
    private :
        size_t k;
        T torelance;
        T L;
        T minCov;
        size_t n;
        size_t m;
        size_t max_iter;

    public :
        FreakGMM(size_t k_, FreakMat<T>* records_, const T& torelance_ = 1e-7,T minCov_=1e-3, size_t max_iter=1000);
        FreakGMM(FreakMat<T>& kMu, FreakMat<T>* records_, const T& torelance_ = 1e-7,T minCov_=1e-3,size_t max_iter=1000);
        void init();
        void exceptationStep();
        void maximizationStep();
        void run();
        void addMinCorr();

        ~FreakGMM();
};

template <class T>
FreakGMM<T>::FreakGMM(size_t k_, FreakMat<T>* records_, const T& torelance_,T minCov_, size_t max_iter)
{
    records = records_;
    k = k_;
    torelance = torelance_;
    n = records->nRow;
    m = records->nCol;
    minCov = minCov_;
    this->max_iter = max_iter;
    mu = FreakMat<T>(k,m);

    for(size_t i =0; i<k; ++i) {
        for(size_t j=0;j<m;++j) {
            mu.at(i,j) = records->at((i*13)%n,j);
        }
    }

    init();
}

template <class T>
FreakGMM<T>::FreakGMM(FreakMat<T>& kMu, FreakMat<T>* records_, const T& torelance_,T minCov_, size_t max_iter)
{
    records = records_;
    torelance = torelance_;
    this->max_iter = max_iter;
    n = records->nRow;
    m = records->nCol;
    minCov = minCov_;
    mu = kMu;
    k = kMu.nRow;

    init();
}

template <class T>
void FreakGMM<T>::init()
{

    assert(m > 0 && n > 0);

    L = -std::numeric_limits<T>::infinity();

    sigma2 = std::vector<FreakMat<T> >(k,FreakMat<T>(m,m));
    alpha =  FreakVector<T>(k);
    r = FreakMat<T>(n,k);
    

    std::vector<size_t> numK(n,0);
    for(size_t i=0;i<n;++i) {
        T mDistance = 0.0;
        size_t index = -1;
        FreakVector<T> row = records->row(i);
        for(size_t j=0;j<k;++j) {
            FreakVector<T> muRow = mu.row(j);
            T distance = row.distance(muRow);
            if (j == 0) {
                index = 0;
                mDistance = distance;
            } else {
                if (distance < mDistance) {
                    index = j;
                    mDistance = distance;
                }
            }
        }
        numK[i] = index;
    }
    std::vector<size_t> nTotal(k,0);
    for(size_t i=0;i<n;++i) {
        nTotal[numK[i]]++;
    }
    T* corrData = NULL;
    for(size_t i=0;i<k;++i) {
        FreakMat<T> kRecords(nTotal[i],m);
        alpha[i] = nTotal[i]*1.0/n;
        size_t t = 0;
        for(size_t j=0;j<n;++j) {
            if (numK[j] == i) {
                for(size_t q=0;q<m;++q) {
                    kRecords.at(t,q) = records->at(j,q);
                }
                t++;
            }
        }
        assert( t == nTotal[i] );
        corrData = corr(t,m,kRecords.ptr());
        sigma2[i] = FreakMat<T>(m,m,corrData);
        delete[] corrData;
    }
    addMinCorr();

}

template <class T>
FreakGMM<T>::~FreakGMM()
{
    /*
    delete sigma2;
    delete mu;
    delete alpha;
    delete r;
    */
}

template <class T>
void FreakGMM<T>::exceptationStep()
{
    T* invSigma2 = new T[m*m];
    for(size_t i = 0; i<k ; ++i) {
        FreakVector<T> muRow = mu.row(i);
        FreakMat<T> records_shift = *records - muRow;
        inv(m,m,sigma2[i].ptr(),invSigma2);
        FreakMat<T> invMat(m,m,invSigma2);
        FreakMat<T> tmp = (records_shift * invMat).dot(records_shift).sum(1);
        T coef = pow(2*PI,m*-1.0/2) * sqrt(det(m,m,invMat.ptr()));
        //std::cout<<"coef "<<coef<<std::endl;
        for(size_t j=0;j<n;++j) {
            r.at(j,i) = coef * std::exp(-0.5*tmp.at(j,0));
            //std::cout<<i<<" "<<j<<" "<<r.at(j,i)<<std::endl;
        }
    }
    delete invSigma2;

}

template <class T>
void FreakGMM<T>::maximizationStep()
{
    FreakMat<T> gamma = r.dot(alpha);
    FreakMat<T> gammaColSum = gamma.sum(1);
    FreakVector<T> gammaCol = gammaColSum.col(0);
    gamma = gamma.div(gammaCol,1);
    
    FreakMat<T> gammaRowSum = gamma.sum(0);
    FreakVector<T> nk = gammaRowSum.row(0);
    FreakVector<T> rNk(nk);
    for(size_t i=0;i<k;++i)
        rNk[i] = 1.0/rNk[i];

    //std::cout<<"nk : "<<std::endl;
    //printD(nk);
    alpha = nk/(1.0*n);
    //std::cout<<"alpha : "<<std::endl;
    //printD(alpha);

    FreakMat<T> rNkDiag = FreakMat<T>::diag(rNk);
    FreakMat<T> gammaT = gamma.transpose();

    FreakMat<T> newMu = rNkDiag * gammaT * (*records);
    mu = newMu;

    for(size_t i=0;i<k;++i) {
        FreakVector<T> muRow = mu.row(i);
        FreakMat<T> records_shift = *records - muRow;
        FreakVector<T> gammaCol = gamma.col(i);
        FreakMat<T> records_shitfT = records_shift.transpose();
        //std::cout<<"gammaDiag "<<i<<": "<<std::endl;
        //printD(gammaCol);
        FreakMat<T> gammaRowDiag = FreakMat<T>::diag(gammaCol);
        sigma2[i] = records_shitfT * gammaRowDiag * records_shift;
        //std::cout<<"sigma2 updated "<<i<<": "<<std::endl;
        //printF(sigma2[i].ptr(),m,m);
        sigma2[i] = sigma2[i].dot(rNk[i]);
    }

    addMinCorr();
}

template <class T>
void FreakGMM<T>::run()
{
    size_t i=0;
    do{
        exceptationStep();
        maximizationStep();
        FreakMat<T> logLMat = r.dot(alpha);
        T Lp = 0;
        for(size_t r=0;r<logLMat.nRow;++r) {
            T pLog = 0.0f;
            for(size_t c=0;c<logLMat.nCol;++c) {
                pLog += logLMat.at(r,c);
            }
            Lp += std::log(pLog);
        }
        std::cout<<"Iteration : "<<i<<" likelihood: "<<L<<" "<<Lp<<std::endl;
        if (Lp - L< torelance || i >= max_iter)
            break;
        L = Lp;
        i++;


    } while(true);

}
template <class T>
void FreakGMM<T>::addMinCorr()
{
    for(size_t i=0;i<k;++i) {
        for(size_t j=0;j<m;++j) {
            T& v = sigma2[i].at(j,j);
            v+= minCov;
            assert(v >0 );
        }
    }
}

typedef FreakGMM<> FreakGMMF;

}

#endif /* __FREAK_GMM_H__*/
