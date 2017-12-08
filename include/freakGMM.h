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
        FreakMat<T>& records;
        std::vector<FreakMat<T> > sigma2;
        FreakMat<T> mu;
        FreakVector<T> alpha;
        FreakMat<T> r;
    private :
        size_t k;
        T torelance;
        FreakVector<T> stepLoss;
        size_t n;
        size_t m;

    public :
        FreakGMM(size_t k_, FreakMat<T>& records_, const T& torelance_ = 1e-7);
        void init();
        void exceptationStep();
        void maximizationStep();
        void run();

        ~FreakGMM();
};

template <class T>
FreakGMM<T>::FreakGMM(size_t k_, FreakMat<T>& records_, const T& torelance_) : records(records_)
{
    k = k_;
    torelance = torelance_;
    assert(records.size() > 0);
    n = records.size();
    init();
}

template <class T>
void FreakGMM<T>::init()
{

    m = records[0].size();
    assert(m > 0);

    sigma2 = std::vector<FreakMat<T> >(k,FreakMat<T>(m,m));
    mu = FreakMat<T>(k,m);
    alpha =  FreakVector<T>(k);
    r = FreakMat<T>(n,m);
    for(size_t i =0 ; i<k; ++i) {
        alpha[i] = 1.0/k;
		randu(sigma2[i].ptr(),sigma2[i].size());
        mu[i] = records[i];
    }


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
        FreakMat<T> records_shift = records - mu.row(i);
        inv(m,m,sigma2[i].ptr(),invSigma2);
        FreakMat<T> invMat(m,m,invSigma2);
        FreakMat<T> tmp = (records * invMat).dot(records_shift).sum(1);
    }

    delete invSigma2;

}

template<class T>
T assignAndCountLoss(T& a, T b)
{
    T temp = a-b;
    T loss = abs(temp);
    a = b;
    return loss;
}

template<class T>
T assignAndCountLoss(T& a, T b,bool isArray)
{
    T temp = a-b;
    T loss = abs(temp,isArray);
    a = b;
    return loss;
}

template <class T>
void FreakGMM<T>::maximizationStep()
{
    stepLoss = FreakVector<T>(m);
    for(size_t j=0;j<k;++j) {

        T rk = 0.0;
        FreakVector<T> rMuTemp(m);
        T rSigma2Temp = 0.0;

        for(size_t i=0;i<n; ++i) {
            rk = rk+r[i][j];
            rMuTemp = rMuTemp + records[i] * r[i][j];
            rSigma2Temp = rSigma2Temp + r[i][j]* records[i].distance(mu[j]);
            
        }
        stepLoss= stepLoss + assignAndCountLoss(mu[j], rMuTemp / rk,true);
        stepLoss= stepLoss + assignAndCountLoss(sigma2[j], rSigma2Temp / rk);
        stepLoss= stepLoss + assignAndCountLoss(alpha[j], rk/n);
    }
}

template <class T>
void FreakGMM<T>::run()
{
    size_t i=0;
    do{
        exceptationStep();
        maximizationStep();
        std::cout<<"Iteration : "<<i<<" loss sum : "<<stepLoss.sum()<<std::endl;
        i++;
        for(size_t i=0;i<mu.size();++i) {
            printD(mu[i]);
            std::cout<<sigma2[i]<<std::endl;
            std::cout<<alpha[i]<<std::endl;
        }
    } while(stepLoss.sum() >= torelance);

}

typedef FreakGMM<> FreakGMMF;

}

#endif /* __FREAK_GMM_H__*/
