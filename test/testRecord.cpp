
#include <freakRecord.h>

#include <iostream>
#include <cstdio>

using namespace freak;
using namespace std;

int main()
{

    FreakLabelC y1(1);
    FreakVectorF x1;

    x1.push_back(1.0);
    x1.push_back(2.0);

    FreakVectorF x2;
    FreakLabelC y2(0);
    x2.push_back(3.0);
    x2.push_back(3.2);

    FreakRecord<float,int> c1(x1,y1);
    FreakRecord<float,int> c2(x2,y2);

    cout<<c1.loss(c2)<<endl;
    cout<<c1.prod(c2)<<endl;
    c1.add(c2);

    return 0;
}
