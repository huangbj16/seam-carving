#include "mymat.h"

int main()
{
	cout<<"1"<<endl;
	Mat L = imread("Images/op.jpeg", 1);
	cout<<L.rows<<L.cols<<endl;
	MyMat mat;
	mat.CopyfromMat(L);
	for(int i=0; i<L.rows/5; ++i){
		cout<<i<<endl;
		mat.CutY();
	}
	mat.ScanY();
	mat.ShowseamY();
	mat.ShowY();
	for(int i=0; i<L.cols/5; ++i){
		cout<<i<<endl;
		mat.CutX();
	}
	mat.ScanX();
	mat.ShowseamX();
	mat.ShowX();
	return 0;
}

