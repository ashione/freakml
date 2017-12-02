#include <freakGMM.h>
#include <freakMath.h>

namespace freak {
template <class T1, class T2>
FreakGMM<T1,T2>::FreakGMM(size_t k_, std::vector<FreakRecord<T1,T2> >& records_, const T1& torelance)
{
    k = k_;
    torelance = torelance;
    records = records_;
    assert(records.size() > 0);
    init();
}

template <class T1, class T2>
void FreakGMM<T1,T2>::init()
{

    sigma2 = new FreakVector<T1>(k);
    mu= new FreakVector<T1>(k);
    alpha = new FreakVector<T1>(k);
    r = new FreakVector<T1>(k * records.size());
    for(size_t i =0 ; i<k; ++i) {
        *(alpha)[i] = 1.0/k;
    }
}

template <class T1, class T2>
FreakGMM<T1,T2>::~FreakGMM()
{
    delete sigma2;
    delete mu;
    delete alpha;
    delete r;
}

template <class T1, class T2>
void FreakGMM<T1,T2>::exceptationStep()
{
    FreakVector<T1> theta(k);
    for(size_t i=0; i<records.size(); ++i) {
        T1 sumMixed = 0.0;
        for(size_t j=0;j<k; ++j) {
            theta[j] = alpha[j] * 
                gaussian(records[i].getLabel(),mu[k],sigma2[k]);
            sumMixed += theta[j];
        }
        for(size_t j=0;j<k; ++j) {
            r[i*k + j] = theta[j]/sumMixed;
        }
    }

}

template<class T1>
T1 assignAndCountLoss(T1& a, T1 b)
{
    T1 loss = 0;
    loss = abs(a-b);
    a = b;
    return loss;
}

template <class T1, class T2>
void FreakGMM<T1,T2>::maximizationStep()
{
    size_t records_n = records.size();
    stepLoss = 0.0;
    for(size_t j=0;j<k;++j) {

        T1 rk = 0.0;
        T1 rMuTemp = 0.0;
        T1 rSigma2Temp  = 0.0;

        for(size_t i=0;i<records_n; ++i) {
            rk+= r[i*k+j];
            rMuTemp += r[i*k+j] * records[i].getLabel();
            rSigma2Temp += r[i*k+j]*pow(records[i].getLabel() -mu[j],2.0);
            
        }
        stepLoss+=(mu[j], rMuTemp / rk);
        stepLoss+=(sigma2[j], rSigma2Temp / rk);
        stepLoss+=(alpha[j], rk/records_n);
    }
}

template <class T1, class T2>
void FreakGMM<T1,T2>::run()
{
    do{
        exceptationStep();
        maximizationStep();
    } while(stepLoss >= torelance);

}

}
