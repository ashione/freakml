#ifndef __FREAK_GMM_H__
#define __FREAK_GMM_H__

#include <freakConfig.h>
#include <freakEM.h>
#include <freakDatum.h>
#include <random>
#include <iostream>

namespace freak {


template <class T = DATUM_TYPE>
class FreakGMM : public FreakEM
{
    public  :
        std::vector<FreakVector<T> >& records;
        FreakVector<T> sigma2;
        std::vector<FreakVector<T> > mu;
        FreakVector<T> alpha;
        std::vector<FreakVector<T> > r;
    private :
        size_t k;
        T torelance;
        FreakVector<T> stepLoss;
        size_t n;
        size_t m;

    public :
        FreakGMM(size_t k_, std::vector<FreakVector<T> >& records_, const T& torelance_ = 1e-7);
        void init();
        void exceptationStep();
        void maximizationStep();
        void run();

        ~FreakGMM();
};

template <class T>
FreakGMM<T>::FreakGMM(size_t k_, std::vector<FreakVector<T> >& records_, const T& torelance_) : records(records_)
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

    sigma2 = FreakVector<T>(k,1.0);
    mu = std::vector<FreakVector<T> >(k,FreakVector<T>(m));
    alpha =  FreakVector<T>(k);
    r = std::vector<FreakVector<T> >(n,FreakVector<T>(k));
    for(size_t i =0 ; i<k; ++i) {
        alpha[i] = 1.0/k;
		sigma2[i] = (i+1.0)/k;
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
    FreakVector<T> theta(k);
    for(size_t i=0; i<n; ++i) {
        for(size_t j=0;j<k;++j) {
            theta[j] = gaussian(records[i],mu[j],sigma2[j]) * alpha[j];
        }
        T sumMixed = theta.sum();
        r[i] = theta/sumMixed;
    }

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
