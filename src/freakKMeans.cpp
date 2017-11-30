#include <freakKMeans.h>

#include <limits>

using namespace freak;

std::vector<FreakVector<> > 
freak::kmeans(const std::vector<FreakVector<> >& fvts, 
       size_t k,
       DATUM_TYPE epsilon,
       size_t max_iter)
{
    assert(k <= fvts.size());
    std::vector<FreakVector<> > k_indexes(k);

    std::copy(fvts.begin(),fvts.begin()+k,&k_indexes.front());
    
    //DATUM_TYPE loss = std::numeric_limits<DATUM_TYPE>::max();

    for(size_t iter=0; iter<max_iter; ++iter) {
        std::vector<FreakVector<> > k_temp(k);
        std::fill(k_temp.begin(),k_temp.end(),FreakVectorF(fvts[0].size()));
        std::vector<size_t > k_num(k);
        std::fill(k_num.begin(),k_num.end(),0);

        for(size_t i=0; i<fvts.size(); ++i) {
            DATUM_TYPE short_d = std::numeric_limits<DATUM_TYPE>::max();
            size_t short_index = -1;
            for(size_t j=0; j<k; ++j) {
                DATUM_TYPE k_d = fvts[i].distance(k_indexes[j]);
                if (k_d < short_d) {
                    short_index = j;
                    short_d = k_d;
                }
            }
            k_temp[short_index].add(fvts[i]);
            k_num[short_index]++;
        }

        for(size_t j=0; j<k; ++j) {
            k_temp[j].mul(1.0/k_num[j]);
        }
        k_indexes = k_temp;
    }

    return k_indexes;
}
