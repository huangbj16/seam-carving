#include "mymat.h"

int main()
{
	cout<<"1"<<endl;
	Mat L = imread("Images/9.jpg", 1);
	cout<<L.rows<<L.cols<<endl;
	MyMat mat;
	cout<<"1"<<endl;
	mat.CopyfromMat(L);
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

