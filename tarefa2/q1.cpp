#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

using namespace cv;
using namespace std;


int sat(int a){
	return a < 256 ? a : 255;
}

void aumentaBrilho(Mat& img,int delta){
	uchar* p = img.data;
	for(int i=0;i<img.cols*img.rows;i++){
		p[i] = sat(p[i]+delta);
	}
}

int main(int argc,char **argv){
	Mat image;
	cv::String filename;
	if(argc > 1)
		filename = argv[1];
	else
		filename = "lena.jpeg";

	image = imread(filename,0);

	if(image.empty()){
		cerr << "IMAGEM NAO EXiSTE" << endl;
		return -1;
	}

	aumentaBrilho(image,150);

	imshow("brilho",image);
	waitKey(0);

	int T = 10;
	float dt = 0.8;

	for(int i=0;i<image.cols;i++){
		if(i % T < T*dt){
			image.col(i) = Scalar(0);
		}
	}

	imshow("tarjas",image);
	waitKey(0);

	return 0;
}

