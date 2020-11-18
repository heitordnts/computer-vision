//Identificar o quadrado por dois pontos A e B (diagonal)
//detectar a partir de cantos

#include <iostream>
#include <map>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

//opencv headers
#include <opencv2/core.hpp>      // Mat
#include <opencv2/highgui.hpp>   // imshow
#include <opencv2/imgcodecs.hpp> // imread, imwrite
#include <opencv2/imgproc.hpp>   // Canny

using namespace std;
using namespace cv;

//#define DEBUG
#define RESO_ANG 2
#define RESO_LADO 2

struct Quadrado{
	int cx,cy,ang,l;
};

// ========= GLOBAIS ===========
Mat img,qds;
vector<Quadrado> quadrados;

// parametros do detector de cantos
double minDistance = 2;
int    blockSize = 3;
int    gradientSize = 3;
int    maxCorners = 200;
double qualityLevel = 0.01;
bool   useHarrisDetector = false;
double k = 0.04;
vector<Point2f> corners;
// =============================

void detectar_cantos(){
	maxCorners = MAX(maxCorners, 1);

    Mat copy = img.clone();

    goodFeaturesToTrack( img,
                         corners,
                         maxCorners,
                         qualityLevel,
                         minDistance,
                         Mat(),
                         blockSize,
                         gradientSize,
                         useHarrisDetector,
                         k );

    cout << "** Number of corners detected: " << corners.size() << endl;
    int radius = 4;
    for( size_t i = 0; i < corners.size(); i++ )
    {
        circle( copy, corners[i], radius, Scalar(0), 2,8,0 );
    }
	imshow("Cantos",copy);
}

void Hough(){
	int W = img.cols;
	int H = img.rows;
	using T = uchar;
	vector<vector<vector<vector<T>>>> vote_table(W,vector<vector<vector<T>>>(H,vector<vector<T>>(90/RESO_ANG,vector<T>(min(W,H)/RESO_LADO,0))));

	/* limites
		centro:	[w-10,h-10]
		angulo: [0, 90]
		lado  : [10, min(w,h)]
	*/

	for(int i=0;i<corners.size();i++){
		for(int j=0;j<corners.size();j++){
			if(i != j){
				// representar em centro, angulo, diagonal
				Point2f v = corners[i] - corners[j];
				Point2f centro = corners[j] + 0.5*v;
				double angulo = atan2(v.y,v.x)*(180.0/M_PI) + 180 + 45; //-pi to pi
				double lado = norm(v) / sqrt(2);
				int cx = int(centro.x);
				int cy = int(centro.y);
				int ang = (int(round(angulo)) % 90) / RESO_ANG ;
				int l = round(lado)/RESO_LADO;

				for(int ii=-1;ii<=1;ii++){
				for(int jj=-1;jj<=1;jj++){
				for(int kk=-1;kk<=1;kk++){
				for(int ww=-1;ww<=1;ww++){
					int a,b,c,d;
					a = min(W-1,max(0,cx+ii));
					b = min(H-1,max(0,cy+jj));
					c = min(90/RESO_ANG - 1,max(0,ang+kk));
					d = min(min(W,H)/RESO_LADO - 1,max(0,l+ww));
					vote_table[a][b][c][d] += 1;	
				}}}}

#ifdef DEBUG	
				cout << corners[i] << " " << corners[j] << endl;
				cout <<"("<<cx<< ","<<cy<<") "<< ang << " " << l  << endl;
#endif
			}
		}
	}

	qds = img.clone();
	cvtColor(qds,qds,COLOR_GRAY2RGB);

	srand(time(0));

	for(int cx=0;cx<W;cx++){
	for(int cy=0;cy<H;cy++){
	for(int ang=0;ang<90/RESO_ANG;ang++){
	for(int l=0;l<min(W,H)/RESO_LADO;l++){
		if(vote_table[cx][cy][ang][l] == 4){
			Point2f c(cx,cy);
			bool flag = true;

			for(int p=0;p<l*RESO_LADO;p++){
				int dx = 0.8 * ((rand() % (l*RESO_LADO)) - (l*RESO_LADO)/2);
				int dy = 0.8 * ((rand() % (l*RESO_LADO)) - (l*RESO_LADO)/2);

				uchar value = img.at<uchar>(c.y+dy,c.x+dx);
				if(value > 240){
					flag = false;
					break;
				}
			}
			if(flag){
				quadrados.push_back({cx,cy,ang,l});
			}
		}
	}}}}
}

int main(int argc, char *argv[]){

	cv::String filename(argv[1]);
	cout << "Abrindo " << filename << endl;
	img = imread(filename, IMREAD_GRAYSCALE);

	unsigned long long int memoriaNecessaria = img.cols*img.rows*(90.0/RESO_ANG)*(min(img.cols,img.rows)/(double)RESO_LADO);
	cout << "Memoria Necessaria: " << memoriaNecessaria/(1<<20) << " MiB" << endl; 
	cout << "Se voce nao tiver toda essa memoria diminua a resolucao da sua entrada!" << endl;
	cout << "Digite y para continuar ou outra tecla para sair!" << endl;
	if(cin.get() != 'y'){
		cout << "Fechando..."<< endl;
		return 1;
	}	

	imshow("Cantos",img);
	detectar_cantos();
	cout << "Aperte Enter..." << endl;
	waitKey(0);

	Hough();

	map<pair<int,int>,Quadrado> qs;
	for(auto& q : quadrados){
		qs[{q.cx,q.cy}] = q;
	}

cout << quadrados.size()/12 << endl;
	for(auto& p : qs){
		Quadrado q = p.second;

		RotatedRect rRect = RotatedRect(Point2f(q.cx,q.cy),Size2f(q.l*RESO_LADO,q.l*RESO_LADO),q.ang*RESO_ANG);
		Point2f vertices[4];
		rRect.points(vertices);
		for (int i = 0; i < 4; i++)
			line(qds, vertices[i], vertices[(i+1)%4], Scalar(0,255,0), 1);
		//cout << q.cx << "," << q.cy << endl;
	}

	imshow("qqqq",qds);
	imwrite("saida.jpg",qds);
	waitKey(0);
	
	return 0;
}
