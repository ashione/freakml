#include <iostream>
#include <freakMath.h>

using namespace std;
using namespace freak;

void printMat(size_t nRow,size_t nCol,float* mat) {
    for(size_t i=0;i<nRow;++i) {
        for(size_t j=0;j<nCol;++j) {
            cout<<mat[i*nCol+j]<<" ";
        }
        cout<<endl;
    }
}

int main(){
    float mat[]={2,3,1,2};
    float* diagMat = diag(2,2,mat);
    cout<<"det : "<<det(2,2,mat)<<endl;
    cout<<"diag det : "<<det(2,2,diagMat,true)<<endl;
    printMat(2,2,diagMat);
    float* invMat = inv(2,2,mat);
    printMat(2,2,invMat);
    cout<<"inv det : "<<det(2,2,invMat)<<endl;

    float *mulMat = mul(2,2,2,mat,invMat);
    printMat(2,2,mulMat);

    delete[] mulMat;
    delete[] diagMat;
    delete[] invMat;

    size_t n,m;
    cin>>n>>m;
    float *cMat = new float[n*m];

    for(size_t i=0;i<n;++i) {
        for(size_t j=0;j<m;++j) {
            cin>>cMat[i*m + j];
        }
    }

    float *matCorr = corr(n,m,cMat);
    printMat(m,m,matCorr);
    float *matCof = corrCoef(n,m,cMat);
    printMat(m,m,matCof);
    
    delete[] matCorr;
    delete[] matCof;
    delete[] cMat;


    return 0;
}
