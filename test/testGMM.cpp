#include <freakMath.h>
#include <freakDatum.h>
#include <freakMatrix.h>
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
    size_t size;
    ifstream in(args[1]);
    in>>size>>n;

    FreakMat<float> src(size,n);
    FreakVector<float> item(n);
    FreakVector<float> maxItem(n);
    float datum;
    for(size_t i=0;i<size;++i) {
        for(size_t j=0;j<n;++j) { 
            in>>datum;
            item[j] = log(datum);
            if (maxItem[j] < item[j])
                maxItem[j] =  item[j];
            src.at(i,j) = item[j];
        }
    }

    for(size_t i=0;i<src.nRow;++i) {
        for(size_t j=0;j<src.nCol;++j) {
            src.at(i,j) /=  maxItem[j];
        }

    }
    printF(src.ptr(),size,n);
    
    size_t k = 3;
    FreakGMM<float> gmm(k,&src);
    gmm.run();

    for(size_t i=0;i<k;++i) {
        //printD(gmm.mu.row(i));
        //cout<<gmm.sigma2[i]<<endl;
        cout<<gmm.alpha[i]<<endl;
    }

    //std::vector<FreakVectorF > dsc = freak::kmeans(src,3);

    //for(size_t i=0;i<dsc.size();++i) {
    //    printD(dsc[i]);
    //}

    return 0;
}
