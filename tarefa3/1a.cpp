// detectar a partir de cantos

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
//Identificar o quadrado por dois pontos A e B (diagonal)
Mat img,dst;
int thresh = 250;
int maxCorners = 200;
int maxTrackBar = 400;
vector<Point2f> corners;
struct Quadrado{
	int cx,cy,ang,l;
};

void callback(int, void*){
	maxCorners = MAX(maxCorners, 1);
    double qualityLevel = 0.01;
    double minDistance = 10;
    int blockSize = 3, gradientSize = 3;
    bool useHarrisDetector = false;
    double k = 0.04;

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
	imshow("Input img",copy);
}

int main(int argc, char *argv[]){
	cv::String filename(argv[1]);
	cout << "Abrindo " << filename << endl;
	img = imread(filename, IMREAD_GRAYSCALE);

	//int vote_table[700][700][361][990]; // vote_table[centro.x][centro.y][ang][diag]
	int W = img.cols;
	int H = img.rows;
	#define RESO_ANG 2
	#define RESO_LADO 2
	vector<vector<vector<vector<int>>>> vote_table(W,vector<vector<vector<int>>>(H,vector<vector<int>>(90/RESO_ANG,vector<int>(min(W,H)/RESO_LADO,0))));
	/* limites
		centro:	[w-10,h-10]
		angulo: [0, 90]
		lado  : [10, min(w,h)]
	*/
	imshow("Input img",img);
	createTrackbar("thresh: ","Input img",&maxCorners ,maxTrackBar , callback); 
	callback(0,0);
	waitKey(0);

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

				cout << corners[i] << " " << corners[j] << endl;
				cout <<"("<<cx<< ","<<cy<<") "<< ang << " " << l  << endl;
			}
		}
	}
		
	
	Mat qds = img.clone();
	cvtColor(qds,qds,COLOR_GRAY2RGB);
	
	vector<Quadrado> quadrados;
	srand(time(0));
	for(int cx=0;cx<W;cx++){
	for(int cy=0;cy<H;cy++){
	for(int ang=0;ang<90/RESO_ANG;ang++){
	for(int l=0;l<min(W,H)/RESO_LADO;l++){
#ifdef DEBUG	
		if(vote_table[cx][cy][ang][l] > 0){
			cout <<"("<<cx<< ","<<cy<<") "<< ang << " " << l  << endl;
			cout << vote_table[cx][cy][ang][l] << endl;
		}
#endif
		if(vote_table[cx][cy][ang][l] == 4){
			Point2f c(cx,cy);
			Point2f v(cos(ang*RESO_ANG),sin(ang*RESO_ANG));
			bool flag = true;

			for(int p=0;p<l*RESO_LADO;p++){
				int dx = (rand() % (l*RESO_LADO)) -(l*RESO_LADO)/2;
				int dy = (rand() % (l*RESO_LADO))-(l*RESO_LADO)/2;
				dx *= 0.8;
				dy *= 0.8;
				uchar value = img.at<uchar>(c.y+dy,c.x+dx);
				if(value > 240){
					flag = false;
					break;
				}
				else{
				//	line(qds, c,Point2f(c.x+dx,c.y+dy), Scalar(255,0,0), 1);
				}

			}
			/*
			for(double factor=-l/2;factor<l/2;factor+=1){
				uchar value = img.at<uchar>(c.y+(factor*RESO_LADO)*v.y,c.x+(factor*RESO_LADO)*v.x);
				cout << int(value) << endl;
				if(value > 240){
					flag = false;
					break;
				}
				else{
					line(qds, c,Point2f(c.x+(factor*RESO_LADO)*v.x,c.y+(factor*RESO_LADO)*v.y), Scalar(255,0,0), 1);
				}
			}
			*/

			
			if(flag){
				quadrados.push_back({cx,cy,ang,l});
			}
		}
	}}}}
	vote_table.clear();

	for(auto& q : quadrados){
		RotatedRect rRect = RotatedRect(Point2f(q.cx,q.cy),Size2f(q.l*RESO_LADO,q.l*RESO_LADO),q.ang*RESO_ANG);
		Point2f vertices[4];
		rRect.points(vertices);
		for (int i = 0; i < 4; i++)
			line(qds, vertices[i], vertices[(i+1)%4], Scalar(0,255,0), 1);
	}

	
	//rectangle(qds,,,Scalar(0));
	imshow("qqqq",qds);
	imwrite("saida.jpg",qds);
	waitKey(0);
	
	return 0;
}
