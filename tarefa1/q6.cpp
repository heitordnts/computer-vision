#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>

using namespace cv;
using namespace std;

Mat media(vector<Mat> &imgs){
	//transforma a imagem para float64 para
	//podermos acumular a variavel sem dar overflow
	Mat res(imgs[0].rows, imgs[0].cols, CV_64FC3);
	res.setTo(Scalar(0,0,0,0));

	Mat aux;
	for(int i=0;i< imgs.size();i++){
		imgs[i].convertTo(aux,CV_64FC3);

		res += aux;
	}
	//converte novamente para uchar8 e divide pela quantidade de imagens
	res.convertTo(res, CV_8U, 1. / imgs.size());
	return res;
}

int main(){
	//========= Q3 letra A
	cv::String imageName("mandel.png");
	string path("./ImagensComRuido/");
	vector<Mat> imgs(9);
	char filename[32];
	//abre as imagens
	for(int i=1;i<=9;i++){
		sprintf(filename, "./imagensComRuido/a%d.jpg",i);
		cout << "opening " << filename <<endl;
		imgs[i-1] = imread(String(filename), CV_LOAD_IMAGE_COLOR);
		if(imgs[i-1].empty()){
			cout << "IMAGEM NAO EXISTE" << endl;
			return 1;
		}
	}
	Mat result = media(imgs);
	
	imshow("result", result);
	imwrite("result.png", result);
	waitKey(0);
	
	return 0;

}

