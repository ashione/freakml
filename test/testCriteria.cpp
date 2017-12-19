#include <iostream>
#include <freakCriteria.h>

using namespace std;
using namespace freak;

int main(){
    float a[] = {0.5,0.5};
    cout<<entropy(a,2)<<endl;
    cout<<gini(a,2)<<endl;

    return 0;
}
