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

	Mat result(image.rows,image.cols,CV_8UC3);

	for(int i=0;i<image.rows;i++){
		for(int j=0;j<image.cols;j++){
			result.at<V3>(i,j) = image.at<V3>(i,image.cols - j - 1);
		}
	}

	imshow("fliped", result);
	imshow("original", image);

	waitKey(0);
	return 0;
}

