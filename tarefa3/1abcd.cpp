// A ideia da transforma Hough eh a partir de uma informacao da entrada inferir 
// um uma forma que pode ser parametrizada de alguma forma.
// Nesse programa o quadrado foi parametrizado por centro, angulo e lado, 
// e a informacao da entrada sao dois pontos que forma a diagonal do quadrado
// a partir dessa diagonal podemos inferir um quadrado (centro, angulo, lado)

// As limitacoes e a dependencia do detector cantos eh sabida
// mas a ideia eh aplicar o conceito de uma forma diferente das convencionais

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
#define RESO_ANG 1
#define RESO_LADO 1

template<int D, typename T>
struct multidimensional_vector : public vector<multidimensional_vector<D - 1, T>> {
  static_assert(D >= 1, "Vector dimension must be greater than zero!");
  template<typename... Args>
  multidimensional_vector(int n = 0, Args... args) : vector<multidimensional_vector<D - 1, T>>(n, multidimensional_vector<D - 1, T>(args...)) {
  }
};
template<typename T>
struct multidimensional_vector<1, T> : public vector<T> {
  multidimensional_vector(int n = 0, const T& val = T()) : vector<T>(n, val) {
  }
};

struct Quadrado{
	int cx,cy,ang,l;
};

// ========= GLOBAIS ===========
Mat img,qds;
vector<Quadrado> quadrados;

// parametros do detector de cantos
double minDistance = 5; 
int    blockSize = 3;
int    gradientSize = 3;
int    maxCorners = 400;
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

	// tabela de votacao
    multidimensional_vector<4,uchar> vote_table(W,H,90/RESO_ANG,min(W,H)/RESO_LADO);

	// para cada diagonal(par de cantos) temos um possivel quadrado
	for(int i=0;i<corners.size();i++){
		for(int j=0;j<corners.size();j++){
			if(i != j){
				// representar em centro, angulo, lado
				Point2f v = corners[i] - corners[j];
				Point2f centro = corners[j] + 0.5*v;
				double angulo = atan2(v.y,v.x)*(180.0/M_PI) + 180 + 45;
				double lado = norm(v) / sqrt(2);
				int cx = int(centro.x);
				int cy = int(centro.y);
				int ang = (int(round(angulo)) % 90) / RESO_ANG ;
				int l = round(lado)/RESO_LADO;
				
				// alem de votar para o quadrado calculado votamos para os vizinhos dele tambem,
				// para contornar erros de precisao
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

	// analisando a tabela de votacao
	for(int cx=0;cx<W;cx++){
	for(int cy=0;cy<H;cy++){
	for(int ang=0;ang<90/RESO_ANG;ang++){
	for(int l=1;l<min(W,H)/RESO_LADO;l++){
		if(vote_table[cx][cy][ang][l] == 4){
			Point2f c(cx,cy);
			bool flag = true;

			// gera alguns pontos aleatorios para ver se o interior do quadrado eh preto
			for(int p=0,numPoints=30 + l*RESO_LADO;p<numPoints;p++){
				int dx = int(0.9 * ((rand() % (l*RESO_LADO)) - (l*RESO_LADO)/2));
				int dy = int(0.9 * ((rand() % (l*RESO_LADO)) - (l*RESO_LADO)/2));

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
	if( argc == 1 ){
		cout << "Uso: executavel entrada" << endl;
		return -1;
	}
	cv::String filename(argv[1]);
	cout << "Abrindo " << filename << endl;
	img = imread(filename, IMREAD_GRAYSCALE);
	if(img.empty()){
		cout << "Erro ao abrir " << filename << endl;
		return -1;
	}

	unsigned long long int memoriaNecessaria = img.cols*img.rows*(90.0/RESO_ANG)*(min(img.cols,img.rows)/(double)RESO_LADO);
	cout << "Memoria Necessaria: " << memoriaNecessaria/(1<<20) << " MiB" << endl; 
	if(memoriaNecessaria > 3000ULL*(1<<20)){
		cout << "Limite de memoria excedido!" <<  endl; 
		return -1;
	}

	cout << "Se voce nao tiver toda essa memoria diminua a resolucao da sua entrada!" << endl;
	cout << "Digite y para continuar ou outra tecla para sair!" << endl << "> ";
	if(cin.get() != 'y'){
		cout << "Fechando..."<< endl;
		return 1;
	}	

	imshow("Cantos",img);
	detectar_cantos();
	cout << "\nAperte Enter...\n" << endl;
	waitKey(0);
	cout << "Processando..." << endl;

	Hough();
	cout << "OK" << endl;

	map<pair<int,int>,Quadrado> qs;
	vector<int> aux(quadrados.size(),0);

	// filtrando quadrados semelhantes
	for(int i=0;i<quadrados.size();i++){
		Quadrado qi = quadrados[i]; // candidato a representante
		if(aux[i] != 0) continue; // ja tem representante

		aux[i] = i+1;
		Quadrado qq=qi; // quadrado medio do grupo i+1
		
		int n = 1;
		for(int j=i+1;j<quadrados.size();j++){
			Quadrado qj = quadrados[j];
			// se qj for similar a qi, qi vira representante dele
			if(abs(qj.cx - qi.cx) < 5 && abs(qj.cy - qi.cy) < 5){
				aux[j] = i+1;
				qq.cx  += qj.cx;
				qq.cy  += qj.cy;
				qq.ang += qj.ang;
				qq.l   += qj.l;
				n++;
			}
		}
		qq.cx  /= n;
		qq.cy  /= n;
		qq.ang /= n;
		qq.l   /= n;

		qs[{qq.cx,qq.cy}] = qq;
		//cerr << aux[i] << endl;
	}

	cout << "Quantidade de quadrados: " << qs.size() << endl;

	// desenha os quadrados
	for(auto& p : qs){
		Quadrado q = p.second;

		RotatedRect rRect = RotatedRect(Point2f(q.cx,q.cy),Size2f(q.l*RESO_LADO,q.l*RESO_LADO),q.ang*RESO_ANG);
		Point2f vertices[4];
		rRect.points(vertices);
		for (int i = 0; i < 4; i++)
			line(qds, vertices[i], vertices[(i+1)%4], Scalar(0,255,0), 2);
	}

	imshow("Resultado",qds);
	imwrite("saida.jpg",qds);
	waitKey(0);
	
	return 0;
}
