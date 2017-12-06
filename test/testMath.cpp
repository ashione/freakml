#include <iostream>
#include <freakMath.h>

using namespace std;
using namespace freak;

void printMat(float* mat) {
    for(size_t i=0;i<2;++i) {
        for(size_t j=0;j<2;++j) {
            cout<<mat[i*2+j]<<" ";
        }
        cout<<endl;
    }

}
int main(){
    float mat[]={2,3,1,2};
    float* diagMat = diag(2,2,mat);
    cout<<"det : "<<det(2,2,mat)<<endl;
    cout<<"diag det : "<<det(2,2,diagMat,true)<<endl;
    printMat(diagMat);
    float* invMat = inv(2,2,mat);
    printMat(invMat);
    cout<<"inv det : "<<det(2,2,invMat)<<endl;

    float *mulMat = mul(2,2,2,mat,invMat);
    printMat(mulMat);

    delete[] mulMat;
    delete[] diagMat;
    delete[] invMat;
    return 0;
}
