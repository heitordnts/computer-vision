#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int main(int argc, char **argv){
	vector<Mat> imgs;
	for(int i=1;i<argc;i++){
		imgs.push_back(imread(String(argv[i]),IMREAD_GRAYSCALE));
	}
	Mat result(imgs[0].rows, imgs[0].cols,CV_64FC1);
	result.setTo(0);

	Mat aux;
	for(int i=0;i<imgs.size();i++){
		imgs[i].convertTo(aux,CV_64FC1);
		result += aux;
	}
	result.convertTo(result, CV_8U, 1. /imgs.size());

	imshow("result",result);
	waitKey(0);
	
	return 0;
}

