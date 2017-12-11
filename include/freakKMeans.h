#ifndef __FREAK_KMEANS_H__
#define __FREAK_KMEAMS_H__

#include <freakConfig.h>
#include <freakDatum.h>
#include <freakMatrix.h>

#include <vector>

namespace freak {

template<class T>
std::vector<FreakVector<T> > 
kmeans(const std::vector<FreakVector<T> >& FreakVectors, size_t k, T epsilon, size_t max_iter);

std::vector<FreakVector<> > 
kmeans(const std::vector<FreakVector<> >& FreakVectors, size_t k, DATUM_TYPE epsilon = 1e-6, size_t max_iter = 1000);

template<class T>
std::vector<FreakVector<T> > 
kmeans(FreakMat<T>& freakMat, size_t k, DATUM_TYPE epsilon = 1e-6, size_t max_iter = 1000)
{
    std::vector<FreakVector<T> > data(freakMat.nRow);
    for(size_t i=0;i<freakMat.nRow;++i) {
         data[i] = freakMat.row(i);
    }
    return kmeans(data,k,epsilon,max_iter);
}
    

}

#endif /* __FREAK_KMEANS_H__ */
