#include <cv.h>
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <fstream>

using namespace cv;
using namespace std;

struct Unit{
	int x,y;
	Vec3b c;
	int protect;//0=no, 1=protect, 2=remove;
};

class MyMat{
public:
	float CDMax;
	int ori_rows, ori_cols;
	int rows, cols;
	float **Gx, **Gy;//Gx是横向的梯度，Gy是纵向的梯度。 
	float **DPx, **DPy;//DPx根据Gx来实施纵向carving。 
	int **Sx, **Sy;
	Unit **array;
	
	void Initialize();
	void CopyfromMat(Mat &L, Mat &Cov);
	int MinColor(float v1, float v2, float v3);
	float ColorDistance(Vec3b c1, Vec3b c2);
	void CalculateGradient();
	void DynamicPrograming();
	void PrintGradientImage();
	void PrintDPImage();
	void PrintDPTxt();
	void CutX();
	void CutY();
	void Show();
	void Showseam();
}; 
