
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace cv;
using namespace std;

Mat conv(Mat img,Mat kernel){
	int ncols,nrows;
	ncols = img.cols;
	nrows = img.rows;

	Mat dest(img.rows, img.cols,CV_32F);

	for(int y=0;y<nrows;y++){
		for(int x=0;x<ncols;x++){
			float res = 0;
			for(int i=-1;i<=1;i++){
				for(int j=-1;j<=1;j++){
					int iy = (y + i + nrows) % nrows;
					int jx = (x + j + ncols) % ncols;
					res += img.at<float>(iy,jx) * kernel.at<float>(i+1,j+1);
				}
			}
			dest.at<float>(y,x) = res;
		}
	}
	return dest;
}

int main(int argc, char *argv[]){

	cv::String filename(argv[1]);	
	cv::String output(argv[2]);	
	cout << "Abrindo " << filename << endl;

	Mat img = imread(filename, IMREAD_GRAYSCALE);	
	img.convertTo(img,CV_32F,1./255);

	if(img.empty()){
		cerr << "IMAGEM NAO EXiSTE" << endl;
		return -1;
	}

	float kernel_average[]    = {1,1,1,
						   	     1,1,1,
							     1,1,1};

	float kernel_gaussian[]   = {1,2,1,
							     2,4,2,
							     1,2,1};

	float kernel_laplacian[]  = {-1,-1,-1,
							     -1, 8,-1,
							     -1,-1,-1};

	//convolute a kernel with the image
	Mat avg = Mat(3,3,CV_32F, kernel_average)/9.0f;
	Mat gauss = Mat(3,3,CV_32F, kernel_gaussian)/16.0f;
	Mat edges = Mat(3,3,CV_32F, kernel_laplacian);
	Mat kernel = gauss;
	Mat imgsuave;
		
	imgsuave = conv(img,kernel);

	imshow("original",img);
	imgsuave.convertTo(imgsuave,CV_8U,255);
	imshow("resultado",imgsuave);
	
	waitKey(0);

	imwrite(output,imgsuave);

	return 0;
}
