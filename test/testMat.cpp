#include <iostream>

#include <freakMatrix.h>
#include <cstring>

using namespace freak;
using namespace std;

void printMat(FreakMat<float>& mat) {
    for(size_t i=0;i<mat.nRow;++i) {
        for(size_t j=0;j<mat.nCol;++j) {
            cout<<mat.at(i,j)<<" ";
        }
        cout<<endl;
    }

}

int main(){
    FreakMat<float> mat(2,3);
    float array[] = {1.1,2.3,3.5,5.6,0,999};
    for(size_t i=0;i<mat.nRow;++i) {
        for(size_t j=0;j<mat.nCol;++j) {
            mat.at(i,j) = i*mat.nCol + j;
        }
    }
    printMat(mat);
    memcpy(mat.ptr(0,0),array,sizeof(float)*mat.nCol * mat.nRow);
    printMat(mat);

    FreakMat<float> tmat = mat.transpose();
    printMat(tmat);
    FreakMat<float> mmat = mat * tmat;
    printMat(mmat);

    FreakMat<float> matColSum = mat.sum(1);
    cout<<"matsum : "<<endl;
    printMat(matColSum);
    FreakVector<float> matCol = matColSum.col(0);
    cout<<"matP: "<<endl;
    FreakMat<float> matP = mat.div(matCol,1);
    printMat(matP);

    FreakMat<float> matN = matP.sum(0);
    cout<<"matN: "<<endl;
    printMat(matN);

    FreakVector<float> matR = matN.row(0);
    matR = matR/mat.nRow;
    //printD(matR);

    return 0;
}

