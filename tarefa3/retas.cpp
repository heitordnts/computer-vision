// detectar a partir de bordas

#include <iostream>
#include <opencv2/core.hpp>      // Mat
#include <opencv2/highgui.hpp>   // imshow
#include <opencv2/imgcodecs.hpp> // imread, imwrite
#include <opencv2/imgproc.hpp>   // Canny
//#include <cmath> // cos, sin
using namespace std;
using namespace cv;

#define N_RO_VALUES 100
#define N_THETA_VALUES 100

int main(int argc, char *argv[]){
	cv::String filename(argv[1]);
	cout << "Abrindo " << filename << endl;
	Mat img = imread(filename, IMREAD_GRAYSCALE);

	// gerar imagem de bordas
	Canny(img,img,10,50,3);

	// matriz de votacao
	int vote_matrix[N_RO_VALUES][N_THETA_VALUES];
	memset(vote_matrix, 0, sizeof(vote_matrix));

	for(int i=0;i<img.rows;i++){
		for(int j=0;j<img.cols;j++){
			if(/*pixel de borda*/0){
#if 0
				for(double theta=0;theta<PI;theta+=PI/N_THETA_VALUES){
					int ro = cos(theta)*i +sin(theta)*j;
					int theta_idx = theta*N_THETA_VALUES/PI - 1;
					vote_matrix[ro][theta_idx]++;
				}
#endif
			}
		}
	}
		
	
	imshow("Result",img);
	waitKey(0);
	return 0;
}

