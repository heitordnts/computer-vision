// detectar a partir de cantos

#include <iostream>
#include <map>

//opencv headers
#include <opencv2/core.hpp>      // Mat
#include <opencv2/highgui.hpp>   // imshow
#include <opencv2/imgcodecs.hpp> // imread, imwrite
#include <opencv2/imgproc.hpp>   // Canny

using namespace std;
using namespace cv;

//Identificar o quadrado por dois pontos A e B (diagonal)
typedef pair<int,int> Ponto;
struct Quadrado{
	Ponto a,b; 
};
Mat img,dst;
int thresh = 250;
void callback(int, void*){
    Mat dst_norm, dst_norm_scaled;
	//convertendo para 8bit grayscale(CV_8U)
    normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    convertScaleAbs( dst_norm, dst_norm_scaled );

	int cantos=0;
    for( int i = 0; i < dst_norm.rows ; i++ )
    {
        for( int j = 0; j < dst_norm.cols; j++ )
        {
            if((int) dst_norm.at<float>(i,j) > thresh )
            {
				cantos++;
                circle( dst_norm_scaled, Point(j,i), 5,  Scalar(0), 2, 8, 0 );
            }
        }
    }
	cout << "N de cantos: " << cantos << endl;
	// obter cantos

	// para cada par de cantos (A,B) com A!=B
	// vote_table[{A,B}] += 1;
	// otimizacao: escolher os (A,B) de acordo com gradiente em A
	//imshow("Result",dst_norm_scaled);
	imshow("Input img",dst_norm_scaled);
//	waitKey(0);
	
}

int main(int argc, char *argv[]){
	cv::String filename(argv[1]);
	cout << "Abrindo " << filename << endl;
	img = imread(filename, IMREAD_GRAYSCALE);

 	//map<Quadrado,int> vote_table;
	
    dst = Mat::zeros(img.size(), CV_32FC1 );
	cornerHarris(img, dst, 2, 3, 0.04);

	imshow("Input img",img);
	createTrackbar("thresh: ","Input img",&thresh, 250, callback); 

	waitKey(0);

	return 0;
}

