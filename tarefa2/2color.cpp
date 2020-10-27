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
	int ksize = kernel.rows;

	Mat dest(img.rows, img.cols,CV_32FC3);

	for(int y=0;y<nrows;y++){
		for(int x=0;x<ncols;x++){
			Vec3f res(0,0,0);
			for(int i=-ksize/2;i<=ksize/2;i++){
				for(int j=-ksize/2;j<=ksize/2;j++){
					int iy = (y + i + nrows) % nrows;
					int jx = (x + j + ncols) % ncols;
					float b = img.at<Vec3f>(iy,jx)[0] * kernel.at<Vec3f>(i+1,j+1)[0];
					float g = img.at<Vec3f>(iy,jx)[1] * kernel.at<Vec3f>(i+1,j+1)[1];
					float r = img.at<Vec3f>(iy,jx)[2] * kernel.at<Vec3f>(i+1,j+1)[2];
					Vec3f c(b,g,r);
					res += c;
				}
			}
			dest.at<Vec3f>(y,x) = res;
		}
	}
	return dest;
}

int main(int argc, char *argv[]){

	cv::String filename(argv[1]);	
	cv::String output(argv[2]);	
	cout << "Abrindo " << filename << endl;

	Mat img = imread(filename, IMREAD_COLOR);	
	img.convertTo(img,CV_32FC3,1./255);

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

	float kernel_gaussian5[]   ={1,4, 7, 4, 1,
							     4,16,26,16,4,
							     7,26,41,26,7,
							     4,16,26,16,4,
							     1,4, 7, 4, 1};

	float kernel_laplacian[]  = {-1,-1,-1,
							     -1, 8,-1,
							     -1,-1,-1};

	//convolute a kernel with the image
	Mat avg = Mat(3,3,CV_32FC3, kernel_average)/9.0f;
	Mat gauss = Mat(3,3,CV_32FC3, kernel_gaussian)/16.0f;
	Mat gauss5 = Mat(5,5,CV_32FC3, kernel_gaussian5)/273.0f;
	Mat edges = Mat(3,3,CV_32FC3, kernel_laplacian);
	Mat kernel = gauss;
	Mat imgsuave;

	imgsuave = conv(img,kernel);
	imshow("original",img);
	imgsuave.convertTo(imgsuave,CV_8UC3,255);
	imshow("resultado",imgsuave);
	
	waitKey(0);

	imwrite(output,imgsuave);

	return 0;
}
