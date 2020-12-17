#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv){
	int altura,largura;

	altura  = atoi(argv[1]);
	largura = atoi(argv[2]);
	Mat gradient(altura, largura, CV_8UC1);

	for(int i=0;i<altura;i++){
		gradient.row(i) =  i * 255/altura;
	}
	
	imshow("gradient", gradient);
	waitKey(0);
}
