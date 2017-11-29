#include <freakConfig.h>
#include <freakLR.h>

#include <iostream>

DATUM_TYPE freak::train(FreakLR<DATUM_TYPE,LABEL_TYPE>* freakLR,DATUM_TYPE lr, size_t max_iter)
{
    size_t record_n = freakLR->records.size();
    assert(record_n > 0 );
    size_t v_n = freakLR->records[0].size();
    assert(v_n > 0 );
    DATUM_TYPE b = 1.0;
    for( size_t iter = 0 ; iter <  max_iter ; ++iter) {
        for(size_t i =0 ; i< record_n ; ++i) {
            DATUM_TYPE prod_v = freakLR->records[i].prod(*(freakLR->w)) + b;
            LABEL_TYPE label_v = freakLR->records[i].getLabel();
            DATUM_TYPE delta = lr * (label_v - std::exp(prod_v)/(1.0+ std::exp(prod_v)));

            for(size_t j = 0 ; j<v_n ; j++) {
                freakLR->w->set(j,freakLR->w->getByIndex(j)+ delta * freakLR->records[i][j]); 
            }
            b = b + delta*b;
            for(size_t j =0 ;j<v_n ; j++) {
                std::cout<<freakLR->w->getByIndex(j)<<" ";
            }
            std::cout<<b<<std::endl;
            std::cout<<"iter : "<<iter<<" "<<delta<<std::endl;
        }
    }
    return b;
}
