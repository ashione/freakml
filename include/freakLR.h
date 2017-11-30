#ifndef __FREAK_LR_H__
#define __FREAK_LR_H__

#include <freakConfig.h>
#include <freakRecord.h>
#include <freakWeight.h>

#include <vector>
namespace freak {

template <class T1 = DATUM_TYPE, class T2 = LABEL_TYPE>
class FreakLR {

    public :
        std::vector<FreakRecord<T1,T2> >& records;
        FreakWeight<T1>* w;

    public :
        FreakLR(std::vector<FreakRecord<T1,T2> >& input) : records(input){
            w = new FreakWeight<T1>(input[0].size());
        };
        void train(const T1 epsilon);

        virtual ~FreakLR();
};

//template <class T1,class T2>
//FreakLR<T1,T2>::FreakLR(std::vector<FreakRecord<T1,T2> >& input)
//{
//    this->records = input;
//    w = new FreakVector<T1>(input[0].size());
//}

template <class T1,class T2>
void 
FreakLR<T1,T2>::train(const T1 epsilon)
{
    train(this,epsilon);
}

template <class T1,class T2>
FreakLR<T1,T2>::~FreakLR()
{
    delete w;
}

void train(FreakLR<>* freakLR,DATUM_TYPE lr = 0.002, size_t max_iter= 1000);

}

#endif /* __FREAK_LR_H__ */
