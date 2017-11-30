#include <freakLR.h>
#include <freakDatum.h>
#include <freakLabel.h>
#include <freakRecord.h>
#include <iostream>

#include <vector>
#include <cmath>

using namespace std;
using namespace freak;

int main() {
    float x1,x2,y;
    std::vector<FreakRecord<float,int> > records;

    while(cin>>x1>>x2>>y) {
        FreakVector<float> x_v;
        x_v.push_back(x1);
        x_v.push_back(x2);
        FreakLabel<int> y_v(y);
        records.push_back(FreakRecord<float,int>(x_v,y_v));

    }

    FreakLR<float,int> frealr(records);
    train(&frealr);
    float ac = 0;
    for(size_t i =0 ; i<records.size() ; ++i) {
        float p0 = 1/(1.0+exp(records[i].prod(*(frealr.w)) + frealr.w->getB()));
        int y_p = p0>0.5?0:1;
        if (y_p == records[i].getLabel()) {
            ac++;
        }

        cout<<y_p<<" "<<records[i].getLabel()<<endl;
    }
    cout<<"ac : "<<(ac/records.size())<<endl;
}
