#include <freakDatum.h>
#include <iostream>
#include <algorithm>

void freak::printD (const FreakVectorF& t){
    std::cout<<"size : "<<t.size()<<std::endl;
    for(int i=0;i<t.size();++i)  {
        std::cout<<t.getByIndex(i)<<std::endl;
    }
}

