#include <iostream>
#include <freakKMeans.h>
#include <freakDatum.h>

#include <vector>
#include <cmath>

using namespace std;
using namespace freak;

int main() {

    FreakVectorF x1;
    x1.push_back(1.0);
    x1.push_back(2.0);

    FreakVectorF x2;
    x2.push_back(2.0);
    x2.push_back(1.0);
    std::vector<FreakVectorF > x;
    x.push_back(x1);
    x.push_back(x2);

    FreakVectorF y1;
    y1.push_back(-12.0);
    y1.push_back(-1.0);

    FreakVectorF y2;
    y2.push_back(-3.0);
    y2.push_back(-4.0);

    x.push_back(y1);
    x.push_back(y2);

    std::vector<FreakVectorF > result = freak::kmeans(x,2);

    for(size_t i=0;i<result.size();++i) {
        printD(result[i]);
    }

    size_t n;
    cin>>n;

    std::vector<FreakVectorF > src;
    FreakVectorF item(n);
    float datum;
    while(cin>>datum) {
        size_t i =0;
        item[i++] = datum;
        for(;i<n;++i) {
            cin>>datum;
            item[i] = datum;
        }
        src.push_back(item);
        printD(item);
    }

    std::vector<FreakVectorF > dsc = freak::kmeans(src,10);

    for(size_t i=0;i<dsc.size();++i) {
        printD(dsc[i]);
    }

    return 0;
}
