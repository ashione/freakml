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

void testInvMatrix(float* mat,size_t n)
{
    cout<<"det : "<<det(n,n,mat)<<endl;

    cout<<"diag : "<<endl;
    float* diagMat = diag(n,n,mat);
    printMat(n,n,diagMat);

    float* invMat = inv(n,n,mat);
    cout<<"invMat : "<<endl;
    printMat(n,n,invMat);

    cout<<"mulMat : "<<endl;
    float *mulMat = mul(n,n,n,mat,invMat);
    printMat(n,n,mulMat);

    cout<<"original :  "<<endl;
    printMat(n,n,mat);

    delete[] invMat;
    delete[] mulMat;
}

int main(){
    //float mat[]={2,3,1,2};
    float mat[] = {7,8 ,1, 3, 4,
    1,2.3,7.9,5.6,1988,
    12.45,2545.5,5435,33,45,
    0.2,0.7,789,98.98,23,
    65.45,67.3,11,27,19};


    testInvMatrix(mat,5);

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

    float* rad =new float[5];
    float* rad2 =new float[5];
    randu(rad,5);
    randu(rad2,5);
    cout<<"randu : "<<endl;
    for(size_t i=0;i<5;++i) {
        cout<<rad[i]<<" "<<rad2[i]<<" ";
    }
    cout<<endl;
    swap(rad,rad2,5);

    for(size_t i=0;i<5;++i) {
        cout<<rad[i]<<" "<<rad2[i]<<" ";
    }
    cout<<endl;

    delete[] rad;
    delete[] rad2;
    return 0;
}
