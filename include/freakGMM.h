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
        T loss;
        T minCov;
        size_t n;
        size_t m;

    public :
        FreakGMM(size_t k_, FreakMat<T>* records_, const T& torelance_ = 1e-7,T minCov_=1e-3);
        void init();
        void exceptationStep();
        void maximizationStep();
        void run();
        void addMinCorr();

        ~FreakGMM();
};

template <class T>
FreakGMM<T>::FreakGMM(size_t k_, FreakMat<T>* records_, const T& torelance_,T minCov_)
{
    records = records_;
    k = k_;
    torelance = torelance_;
    assert(records->size() > 0);
    n = records->nRow;
    minCov = minCov_;
    init();
}

template <class T>
void FreakGMM<T>::init()
{

    m = records->nCol;
    assert(m > 0);

    sigma2 = std::vector<FreakMat<T> >(k,FreakMat<T>(m,m));
    mu = FreakMat<T>(k,m);
    alpha =  FreakVector<T>(k);
    r = FreakMat<T>(n,k);
    
    for(size_t i =0 ; i<k; ++i) {
        for(size_t j=0;j<m;++j) {
            mu.at(i,j) = records->at((i*17)%n,j);
        }
    }

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
        std::cout<<"coef "<<coef<<std::endl;
        for(size_t j=0;j<n;++j) {
            r.at(j,i) = coef * std::exp(-0.5*tmp.at(j,0));
            std::cout<<i<<" "<<j<<" "<<r.at(j,i)<<std::endl;
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
    std::cout<<"nk : "<<std::endl;
    printD(nk);
    alpha = nk/(1.0*n);
    std::cout<<"alpha : "<<std::endl;
    printD(alpha);

    FreakMat<T> rNkDiag = FreakMat<T>::diag(rNk);
    FreakMat<T> gammaT = gamma.transpose();

    FreakMat<T> newMu = rNkDiag * gammaT * (*records);
    loss = 0;
    for(size_t i=0;i<mu.nRow;++i) {
        for(size_t j=0;j<mu.nCol;++j){
            loss += std::abs(mu.at(i,j) - newMu.at(i,j));
        }
    }
    mu = newMu;

    for(size_t i=0;i<k;++i) {
        FreakVector<T> muRow = mu.row(i);
        FreakMat<T> records_shift = *records - muRow;
        FreakVector<T> gammaCol = gamma.col(i);
        FreakMat<T> records_shitfT = records_shift.transpose();
        std::cout<<"gammaDiag "<<i<<": "<<std::endl;
        printD(gammaCol);
        FreakMat<T> gammaRowDiag = FreakMat<T>::diag(gammaCol);
        sigma2[i] = records_shitfT * gammaRowDiag * records_shift;
        std::cout<<"sigma2 updated "<<i<<": "<<std::endl;
        printF(sigma2[i].ptr(),m,m);
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
        std::cout<<"Iteration : "<<i<<" loss sum : "<<loss<<std::endl;
        i++;
        printF(mu.ptr(),k,m);
    } while(loss >= torelance);

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
