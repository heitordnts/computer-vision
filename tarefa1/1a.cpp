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

	cout << image.cols << ", " << image.rows << endl;
	Mat separated(image.rows,3*image.cols,CV_8UC3);

	for(int i=0;i<image.rows;i++){
		for(int j=0;j<image.cols;j++){
			separated.at<V3>(i,j)              = {image.at<V3>(i,j).b,0,0};
			separated.at<V3>(i,j+image.cols)   = {0,image.at<V3>(i,j).g,0};
			separated.at<V3>(i,j+2*image.cols) = {0,0,image.at<V3>(i,j).r};
		}
	}

	imshow("separated", separated);
	imshow("original", image);

	waitKey(0);
	return 0;
}

