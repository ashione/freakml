#ifndef __FREAK_RECORD_H__
#define __FREAK_RECORD_H__

#include <freakConfig.h>
#include <freakDatum.h>
#include <freakLabel.h>

namespace freak {

template< class T1 = DATUM_TYPE, class T2 = LABEL_TYPE>
class FreakRecord : public FreakVector<T1>, public FreakLabel<T2>
{
    public :
        FreakRecord(const FreakVector<T1>& X, const FreakLabel<T2>& Y) :
            FreakVector<T1>(X), FreakLabel<T2>(Y){};
};

}

#endif

