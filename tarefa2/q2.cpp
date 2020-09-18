#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char *argv[]){

	cv::String filename(argv[1]);	
	cout << "Abrindo " << filename << endl;

	Mat img = imread(filename, 0);	

	if(img.empty()){
		cerr << "IMAGEM NAO EXiSTE" << endl;
		return -1;
	}

	float kernel_average[] = {1,1,1,
							  1,1,1,
							  1,1,1};

	float kernel_gaussian[] = {1,2,1,
							  2,4,2,
							  1,2,1};

	float kernel_laplacian[] = {-1,-1,-1,
							    -1,8,-1,
							    -1,-1,-1};
	float kernel_laplacian2[] = {-1,0,1,
							    -1,0,1,
							    -1,0,1};
	//convolute a kernel with the image
	Mat avg = Mat(3,3,CV_32F, kernel_average)/9.0f;
	Mat gauss = Mat(3,3,CV_32F, kernel_gaussian)/16.0f;
	Mat edges = Mat(3,3,CV_32F, kernel_laplacian);
	Mat dst = img.clone();
	Mat dst2 = img.clone();
	imshow("resultado",dst);
		waitKey(0);
	
	//while( waitKey(0) != 27){

		filter2D(dst,dst,dst.depth(), gauss);
		filter2D(dst,dst,dst.depth(), gauss);
		filter2D(dst,dst2,dst.depth(), edges);
		filter2D(dst,dst,dst.depth(), gauss);
		filter2D(dst,dst,dst.depth(), gauss);
		filter2D(dst,dst,dst.depth(), gauss);
		filter2D(dst,dst,dst.depth(), gauss);
		imshow("resultado",0.5*dst + 0.5*dst2);
		waitKey(0);
	//}

	return 0;
}

