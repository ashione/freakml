#include <freakDatum.h>
#include <iostream>
#include <cmath>

using namespace std;
using namespace freak;


int main() {
    FreakVectorF x1;
    x1.push_back(1.0);
    x1.push_back(2.0);
    FreakVectorF y1;
    y1.push_back(2.0);
    y1.push_back(1.0);

    cout<<x1.prod(y1)<<endl;

    FreakVectorF x2(x1);
    x2.push_back(3.0);
    x2.push_back(3.2);
    x1[0] = 7;
    printD(x2);
    printD(x1);

    //cout<<x2.prod(y1)<<endl;
    x2.apply(freak::exp);
    printD(x2);

    x1.apply(freak::log);
    printD(x1);
}
