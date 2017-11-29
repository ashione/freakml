#include <freakDatum.h>
#include <iostream>

using namespace std;

int main() {
    Data<float> x1;
    x1.x.push_back(1.0);
    x1.x.push_back(2.0);
    Data<float> y1;
    y1.x.push_back(2.0);
    y1.x.push_back(1.0);

    cout<<x1.prod(y1)<<endl;
    printD(x1);
}
