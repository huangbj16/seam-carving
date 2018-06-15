#include "mymat.h"

int main()
{
	cout<<"1"<<endl;
	Mat L = imread("Images/mj.png", 1);
	Mat Cov = imread("Images/mj_cover.png", 1);
	cout<<L.rows<<L.cols<<endl;
	MyMat mat;
	cout<<"1"<<endl;
	mat.CopyfromMat(L, Cov);
	cout<<"1"<<endl;	
	for(int i=0; i<L.rows/5; ++i){
		cout<<i<<endl;
		mat.CutY();
	}
	for(int i=0; i<L.cols/5; ++i){
		cout<<i<<endl;
		mat.CutX();
	}
	mat.Showseam();
	mat.Show();
	return 0;
}

