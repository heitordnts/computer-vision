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
	Mat image1,image2;

	image1 = imread(samples::findFile(String(argv[1])), IMREAD_COLOR);
	image2 = imread(samples::findFile(String(argv[2])), IMREAD_COLOR);
	
	if(argc != 3 || image1.empty() || image2.empty()){
		cout << "IMAGEM NAO EXISTE" << endl;
		return -1;
	}
	//resize	
	resize(image2, image2, Size(image1.cols,image1.rows));

	Mat result(0.5*image1 + 0.5*image2);

	imshow("img1", image1);
	imshow("img2", image2);
	imshow("result", result);

	waitKey(0);
	return 0;
}

