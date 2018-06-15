#include "mymat.h" 
#include <cstdio>

int MyMat::MinColor(float v1, float v2, float v3){
	if(v1 < v2){
		if(v1 < v3) return 1;
		else return 3;
	}
	else{
		if(v2 < v3) return 2;
		else return 3;
	}
}

float MyMat::ColorDistance(Vec3b c1, Vec3b c2){
	float res = 0;
	for(int i=0; i<3; ++i){
		res += (c1[i] - c2[i]) * (c1[i] - c2[i]);
	}
	res = sqrt(res)/2;
	return res;
}

void MyMat::Initialize(){
	Gx = new float*[cols];
	Gy = new float*[cols];
	DPx = new float*[cols];
	DPy = new float*[cols];
	Sx = new int*[cols];
	Sy = new int*[cols];
	array = new Unit*[cols];
	for(int i=0; i<cols; ++i){
		Gx[i] = new float[rows];
		Gy[i] = new float[rows];
		DPx[i] = new float[rows];
		DPy[i] = new float[rows];
		Sx[i] = new int[rows];
		Sy[i] = new int[rows];
		array[i] = new Unit[rows];
	}
}

void MyMat::CopyfromMat(Mat &L, Mat &Cov){
	rows = L.rows;
	cols = L.cols;
	ori_rows = L.rows;
	ori_cols = L.cols;
	Initialize();
	Vec3b red(0, 0, 255);
	Vec3b green(0, 255, 0);
	for(int i=0; i<cols; ++i){
		for(int j=0; j<rows; ++j){
			array[i][j].c = L.at<Vec3b>(j, i);
			array[i][j].x = i;
			array[i][j].y = j;
			if(L.at<Vec3b>(j, i) != Cov.at<Vec3b>(j, i)){ 
				if(Cov.at<Vec3b>(j, i) == red){
					array[i][j].protect = 1;
				}
				else if(Cov.at<Vec3b>(j, i) == green){
					array[i][j].protect = 2;
				}
			}
			else array[i][j].protect = false;
		}
	}
	CDMax = 441.67;
}

void MyMat::CalculateGradient(){
	//X
	for(int y=0; y<rows; ++y){
		Gx[0][y] = CDMax;
	}
	for(int x=1; x<cols-1; ++x){
		for(int y=0; y<rows; ++y){
			if(array[x][y].protect == 0)
				Gx[x][y] = ColorDistance(array[x+1][y].c, array[x-1][y].c);
			else if(array[x][y].protect == 1)
				Gx[x][y] = CDMax;
			else
				Gx[x][y] = 0;
		}
	}
	for(int y=0; y<rows; ++y){
		Gx[cols-1][y] = CDMax;
	}
	//Y
	for(int x=0; x<cols; ++x){
		Gy[x][0] = CDMax;
	}
	for(int y=1; y<rows-1; ++y){
		for(int x=0; x<cols; ++x){
			if(array[x][y].protect == 0)
				Gy[x][y] = ColorDistance(array[x][y+1].c, array[x][y-1].c);
			else if(array[x][y].protect == 1)
				Gy[x][y] = CDMax;
			else
				Gy[x][y] = 0;
		}
	}
	for(int x=0; x<cols; ++x){
		Gy[x][rows-1] = CDMax;
	}
}

void MyMat::DynamicPrograming(){
	//DPx
	int cnt = 0;
	for(int x=0; x<cols; ++x){
		DPx[x][0] = Gx[x][0];
		Sx[x][0] = -1;
	}
	float v1,v2,v3;
	for(int y=1; y<rows; ++y){
		for(int x=0; x<cols; ++x){
			if(x-1>=0) v1 = DPx[x-1][y-1];
			else v1 = CDMax*y;
			v2 = DPx[x][y-1];
			if(x+1<cols) v3 = DPx[x+1][y-1];
			else v3 = CDMax*y;
			int p = MinColor(v1, v2, v3);
			
			int rp;
			switch(p){
				case 1:
					rp = x-1;
					break;
				case 2:
					rp = x;
					break;
				case 3:
					rp = x+1;
					break;
				default:
					rp = x;
					break;
			}
			DPx[x][y] = DPx[rp][y-1] + Gx[x][y];
			Sx[x][y] = rp;
		}
	}
	//cout<<cnt<<endl;
	//DPy 
	
	for(int y=0; y<rows; ++y){
		DPy[0][y] = Gy[0][y];
		Sy[0][y] = -1;
	}
	for(int x=1; x<cols; ++x){
		for(int y=0; y<rows; ++y){
			if(y-1>=0) v1 = DPy[x-1][y-1];
			else v1 = CDMax*x;
			v2 = DPy[x-1][y];
			if(y+1<rows) v3 = DPy[x-1][y+1];
			else v3 = CDMax*x;
			
			int p = MinColor(v1, v2, v3);
			int rp;
			switch(p){
				case 1:
					rp = y-1;
					break;
				case 2:
					rp = y;
					break;
				case 3:
					rp = y+1;
					break;
				default:
					rp = y;
					break;
			}
			DPy[x][y] = DPy[x-1][rp] + Gy[x][y];
			Sy[x][y] = rp;
		}
	}
	
}

void MyMat::PrintGradientImage(){
	Mat M(rows, cols, CV_8UC3);
	const float T = 1;//255 / CDMax;
	ofstream fout("Gx.txt");
	for(int row=0; row<rows; ++row){
		for(int col = 0; col < cols; ++col){
			M.at<Vec3b>(row, col) = Vec3b(Gx[col][row]*T, Gx[col][row]*T, Gx[col][row]*T);
			fout<<Gx[col][row]<<" ";
		}
		fout<<endl;
	}
	imwrite("Gx.png", M);
	imshow("Gx", M);
	waitKey(0);
	fout.close();
	Mat N(rows, cols, CV_8UC3);
	ofstream fout2("Gy.txt");
	for(int col = 0; col < cols; ++col){
		for(int row=0; row<rows; ++row){
			N.at<Vec3b>(row, col) = Vec3b(Gy[col][row]*T, Gy[col][row]*T, Gy[col][row]*T);
			fout2<<Gy[col][row]<<" ";
		}
		fout2<<endl;
	}
	imwrite("Gy.png", N);
	imshow("Gy", N);
	fout2.close();
	waitKey(0);
} 

void MyMat::PrintDPImage(){
	Mat M(rows, cols, CV_8UC3, Scalar::all(0));
	int minx = -1;
	float minc = CDMax * rows;
	for(int x=0; x<cols; ++x){
		if(minc > DPx[x][rows-1]){
			minc = DPx[x][rows-1];
			minx = x;
		}
	}
	for(int y=rows-1; y>=0; --y){
		M.at<Vec3b>(y, minx) = Vec3b(255, 255, 255);
		minx = Sx[minx][y];
	}
	imwrite("DPx.png", M);
	imshow("DPx", M);
	waitKey(0);
	Mat N(rows, cols, CV_8UC3, Scalar::all(0));
	int miny = -1;
	minc = CDMax * cols;
	for(int y=0; y<rows; ++y){
		if(minc > DPy[cols-1][y]){
			minc = DPy[cols-1][y];
			miny = y;
		}
	}
	for(int x=cols-1; x>=0; --x){
		N.at<Vec3b>(miny, x) = Vec3b(255, 255, 255);
		miny = Sy[x][miny];
	}
	imwrite("DPy.png", N);
	imshow("DPy", N);
	waitKey(0);
} 

void MyMat::PrintDPTxt(){
	ofstream fout("DPx.txt");
	for(int y=0; y<rows; ++y){
		for(int x=0; x<cols; ++x){
			fout<<DPx[x][y]<<" ";
		}
		fout<<endl;
	}
	fout.close();
}

void MyMat::Show(){
	Mat M(rows, cols, CV_8UC3);
	for(int row=0; row<rows; ++row){
		for(int col = 0; col < cols; ++col){
			M.at<Vec3b>(row, col) = array[col][row].c;
		}
	}
	imwrite("ori.png", M);
	imshow("ori", M);
	waitKey(0);
}

void MyMat::CutX(){
	CalculateGradient();
	DynamicPrograming();
	int minx = -1;
	float minc = CDMax * rows;
	for(int x=0; x<cols; ++x){
		if(minc > DPx[x][rows-1]){
			minc = DPx[x][rows-1];
			minx = x;
		}
	}
	for(int y=rows-1; y>=0; --y){
		for(int i=minx; i<cols-1; ++i)
			array[i][y] = array[i+1][y];
		minx = Sx[minx][y];
	}
	cols--;
}

void MyMat::CutY(){
	CalculateGradient();
	DynamicPrograming();
	int miny = -1;
	int minc = CDMax * cols;
	for(int y=0; y<rows; ++y){
		if(minc > DPy[cols-1][y]){
			minc = DPy[cols-1][y];
			miny = y;
		}
	}
	for(int x=cols-1; x>=0; --x){
		for(int i=miny; i<rows-1; ++i)
			array[x][i] = array[x][i+1];
		miny = Sy[x][miny];
	}
	rows--;
}

void MyMat::Showseam(){
	Mat M(ori_rows, ori_cols, CV_8UC3, Scalar::all(255));
	int ori_row, ori_col;
	for(int row=0; row<rows; ++row){
		for(int col = 0; col < cols; ++col){
			ori_row = array[col][row].y;
			ori_col = array[col][row].x;
			M.at<Vec3b>(ori_row, ori_col) = array[col][row].c;
		}
	}
	imwrite("seam.png", M);
	imshow("seam", M);
	waitKey(0);
}













