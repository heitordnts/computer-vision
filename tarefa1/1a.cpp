#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace cv;
using namespace std;
struct V3{
	unsigned char b,g,r;
};

int main(int argc, char *argv[]){
	//========= Q3 letra A ========= 

	cv::String imageName(argv[1]);
	Mat image;

	image = imread(samples::findFile(imageName), IMREAD_COLOR);
	
	if(argc != 2 || image.empty()){
		cout << "IMAGEM NAO EXISTE" << endl;
		return -1;
	}

	imshow("Red", image & Scalar(0,0,255));
	imshow("Blue", image & Scalar(255,0,0));
	imshow("Green", image & Scalar(0,255,0));

	imshow("original", image);

	waitKey(0);
	return 0;
}

