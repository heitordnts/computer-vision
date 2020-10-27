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
	if(argc != 6){
		cout << "Numero de argumentos errado" << endl;
		exit(1);
	}

	int brilho, entrefaixa ,faixa;
	brilho = atoi(argv[2]);
	entrefaixa = atoi(argv[3]);
	faixa = atoi(argv[4]);

	Mat image;
	cv::String filename(argv[1]);
	cv::String output(argv[5]);

	image = imread(filename,IMREAD_GRAYSCALE);

	if(image.empty()){
		cerr << "IMAGEM NAO EXiSTE" << endl;
		return -1;
	}

	aumentaBrilho(image,brilho);

	imshow("brilho",image);
	waitKey(0);

	int T = faixa + entrefaixa;
	float dt = faixa/(double)T;
	cout << T <<  " " << dt << endl;

	for(int i=0;i<image.cols;i++){
		if(i % T < T*dt){
			image.col(i) = Scalar(0);
		}
	}

	imshow("tarjas",image);
	imwrite(output,image);
	waitKey(0);

	return 0;
}

