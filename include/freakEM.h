#ifndef __FREAK_EM_H__
#define __FREAK_EM_H__

#include <freakConfig.h>
#include <freakRecord.h>
#include <vector>

namespace freak {

class FreakEM {
    public :
        virtual void exceptationStep() = 0;
        virtual void maximizationStep() = 0;
        virtual void run() = 0;
};

}

#endif /* __FREAK_EM_H__*/
