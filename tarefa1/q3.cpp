#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace cv;
using namespace std;

int main(){
	//========= Q3 letra A
	cv::String imageName("mandel.png");
	Mat image;
	image = imread(samples::findFile(imageName), CV_LOAD_IMAGE_COLOR);
	
	if(image.empty()){
		cout << "IMAGEM NAO EXISTE" << endl;
		return -1;
	}

	imshow("original", image);
	waitKey(0);
	
	Mat aux;

	imshow("blue", image & Scalar(255,0,0));
	imwrite("blue.png", image & Scalar(255,0,0));

	imshow("green", image & Scalar(0,255,0));
	imwrite("green.png", image & Scalar(0,255,0));

	imshow("red", image & Scalar(0,0,255));
	imwrite("red.png", image & Scalar(0,0,255));

	flip(image,aux,1);
	imshow("flip horizontally", aux);
	imwrite("flip.png",aux);
	waitKey(0);

	Mat second = imread(samples::findFile("julia.jpeg"), CV_LOAD_IMAGE_COLOR);

	resize(second, second, Size(image.cols, image.rows));
	
	imshow("second",second);
	imwrite("second.png", second);

	imshow("blend",0.5*second + 0.5*image);
	imwrite("blend.png",0.5*second + 0.5*image);

	imshow("blend2",(0.5*aux & Scalar(255,0,0)) + (0.5*image & Scalar(0,255,0)));
	waitKey(0);

	Mat gradient = Mat::zeros(image.rows,image.cols, CV_8UC1);

	for(int row=0;row<gradient.rows;row++){
		for(int col=0;col<gradient.cols;col++){
			gradient.at<uchar>(row,col) = row * 256/gradient.rows;
		}
	}
	imshow("gradient", gradient);
	imwrite("gradient.png", gradient);
	waitKey(0);

	return 0;
}

