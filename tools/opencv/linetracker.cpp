#include <opencv2/opencv.hpp>

#include <math.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {

    Mat img, gray;
    
    // if( argc != 2 && !(img=imread(argv[1])).data)
    //     return -1;
    img = imread(argv[1]);
    if (!img.data) return -1;

    cout << "IMG: " << argv[1] <<" : " << img.size().width << ", " << img.size().height << endl;
    
    Size size = img.size();
    
    cvtColor(img, gray, COLOR_BGR2GRAY);
    

    Mat gradient(size.width, size.height,CV_16SC3);

    Mat grad_x, grad_y, dir;

    setCOI(gradient, 0); 
    
    Sobel( gray, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT );
    Sobel( gray, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT );

    namedWindow( "circles");
    imshow( "circles", gradient);
    waitKey();
    return 0;
}