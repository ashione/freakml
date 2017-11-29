#include <freakLabel.h>
#include <iostream>
#include <cstdio>

using namespace freak;
using namespace std;

int main()
{
    FreakLabelC x(1);
    cout<<x.isCorrect(1)<<endl;
    printf("%f\n",x.loss(1.12));
    cout<<x.isCorrect(0)<<endl;

    FreakLabelF x1(1.2);
    printf("%f\n",x1.loss(1.12));
    return 0;
}
