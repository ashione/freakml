#ifndef __FREAK_GMM_H__
#define __FREAK_GMM_H__

#include <freakConfig.h>
#include <freakEM.h>
#include <freakDatum.h>

namespace freak {


template <class T1 = DATUM_TYPE, class T2 = LABEL_TYPE>
class FreakGMM : public FreakEM
{
    private :
        std::vector<FreakRecord<T1,T2> >& records;
        FreakVector<T1>* sigma2;
        FreakVector<T1>* mu;
        FreakVector<T1>* alpha;
        FreakVector<T1>* r;
        size_t k;
        T1 torelance;
        T1 stepLoss;

    public :
        FreakGMM(size_t k_, std::vector<FreakRecord<T1,T2> >& records_, const T1& torelance = 1e-6);
        void init();
        void exceptationStep();
        void maximizationStep();
        void run();

        ~FreakGMM();
};

typedef FreakGMM<> FreakGMMF;

}

#endif /* __FREAK_GMM_H__*/
