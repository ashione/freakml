#include <freakDatum.h>
#include <iostream>
#include <algorithm>

namespace freak {

void printD (const FreakVectorF& t)
{
    for(size_t i=0; i<t.size(); ++i)  {
        std::cout<<t.getByIndex(i)<<" ";
    }
    std::cout<<std::endl;
}


}

