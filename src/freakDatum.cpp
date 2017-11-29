#include <freakDatum.h>
#include <iostream>

void
printD (const Data<float>& t){
    for(int i=0;i<t.x.size();++i)  {
        std::cout<<t.x[i]<<std::endl;
    }
}
