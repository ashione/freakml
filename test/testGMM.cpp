#include <freakMath.h>
#include <freakDatum.h>
#include <freakGMM.h>
#include <vector>
#include <freakKMeans.h>


#include <iostream>
#include <fstream>

using namespace std;
using namespace freak;

int main(int argv,char* args[])
{
    size_t n;
    ifstream in(args[1]);
    //cin>>n;
    in>>n;

    std::vector<FreakVectorF > src;
    FreakVectorF item(n);
    FreakVectorF maxItem(n);
    float datum;
    while(in>>datum) {
        size_t i =0;
        item[i] = log(datum);
        if (maxItem[i] < item[i])
            maxItem[i] =  item[i];
        ++i;
        for(;i<n;++i) {
            in>>datum;
            item[i] = log(datum);
            if (maxItem[i] < item[i])
                maxItem[i] =  item[i];

        }
        src.push_back(item);
    }
    for(size_t i=0;i<src.size();++i) {
        src[i] = src[i] / maxItem;
        printD(src[i]);
    }
    
    FreakGMM<float> gmm(3,src);
    gmm.run();

    for(size_t i=0;i<gmm.mu.size();++i) {
        printD(gmm.mu[i]);
        cout<<gmm.sigma2[i]<<endl;
        cout<<gmm.alpha[i]<<endl;
    }

    std::vector<FreakVectorF > dsc = freak::kmeans(src,3);

    for(size_t i=0;i<dsc.size();++i) {
        printD(dsc[i]);
    }

    return 0;
}
