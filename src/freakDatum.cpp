#include <freakDatum.h>
#include <algorithm>

namespace freak {

void printD(const FreakVector<float>& t){
    for(size_t i=0; i<t.size(); ++i)  {
        std::cout<<t.getByIndex(i)<<" ";
    }
    std::cout<<std::endl;
}
}

