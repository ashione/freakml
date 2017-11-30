#ifndef __FREAK_KMEANS_H__
#define __FREAK_KMEAMS_H__

#include <freakConfig.h>
#include <freakDatum.h>

#include <vector>

namespace freak {

template<class T>
std::vector<FreakVector<T> > 
kmeans(const std::vector<FreakVector<T> >& FreakVectors, size_t k, T epsilon, size_t max_iter);

std::vector<FreakVector<> > 
kmeans(const std::vector<FreakVector<> >& FreakVectors, size_t k, DATUM_TYPE epsilon = 1e-6, size_t max_iter = 1000);

}

#endif /* __FREAK_KMEANS_H__ */
