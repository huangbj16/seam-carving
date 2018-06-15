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
};

class MyMat{
public:
	float CDMax;
	int ori_rows, ori_cols;
	int rows, cols;
	Mat ori_mat;
	float **Gx, **Gy;//Gx�Ǻ�����ݶȣ�Gy��������ݶȡ� 
	float **DPx, **DPy;//DPx����Gx��ʵʩ����carving�� 
	int **Sx, **Sy;
	Unit **array;
	int **mark;
	
	void Initialize();
	void DeleteOld();
	void CopyfromMat(Mat &L);
	int MinColor(float v1, float v2, float v3);
	float ColorDistance(Vec3b c1, Vec3b c2);
	void CalculateGradientX();
	void DynamicProgramingX();
	void CalculateGradientY();
	void DynamicProgramingY();
	void PrintGradientImage();
	void PrintDPImage();
	void PrintDPTxt();
	void CutX();
	void CutY();
	void ShowX();
	void ShowseamX();
	void ScanX();
	void ShowY();
	void ShowseamY();
	void ScanY();
}; 
