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
    return 0;
}

