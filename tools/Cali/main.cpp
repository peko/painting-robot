#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "zhang-suen.h"

#include <iostream>

using namespace std;
using namespace cv;

Mat img;
int W,H;
int win_r = 15;

static void onMouse( int event, int x, int y, int, void* ) {

    switch(event) {
        case 0: {

            if(x<  win_r  ) x = win_r;
            if(x>W-win_r-1) x = W-win_r-1;
            if(y<  win_r  ) y = win_r;
            if(y>H-win_r-1) y = H-win_r-1;

            Mat dx, dy, roi;

            roi = Mat(img, Rect(x-win_r, y-win_r,win_r*2+1, win_r*2+1));
            int w = roi.cols;

            int cc = ceil(w/2);

            Sobel(roi, dx, CV_32F, 0, 1, 3);
            Sobel(roi, dy, CV_32F, 1, 0, 3);
            Scalar mean_x3 = mean(Mat(dx, Rect(cc-1,cc-1,3,3)));
            Scalar mean_y3 = mean(Mat(dy, Rect(cc-1,cc-1,3,3)));
            Scalar mean_x5 = mean(Mat(dx, Rect(cc-2,cc-2,5,5)));
            Scalar mean_y5 = mean(Mat(dy, Rect(cc-2,cc-2,5,5)));

            Mat drv = Mat(win_r*2+1, win_r*2+1, CV_32FC3);
            //cout << dx.isContinuous()<< atn.isContinuous()<<" " <<dx.step<<endl;


            for(int i=0; i<w; i++) {
                for(int j=0;j<w; j++){
                    float xx = dx.at<float>(i,j);
                    float yy = dy.at<float>(i,j);
                    //short mag = sqrt(xx*xx+yy*yy);
                    drv.at<float>(i, j*3+0) = xx/255.0;
                    drv.at<float>(i, j*3+1) = yy/255.0;
                    //drv.at<float>(i, j*3+2) = sqrt(xx*xx+yy*yy);
                    //drv.at<float>(i, j*4+2) = atan2(xx,yy);
                    //ptr[j] = dy.at<short>(i,j);
                    //cout << dy. at<short>(i,j) << "\t";
                }
                //cout << endl;
            }

            //cout << dx << endl << dy <<endl << drv <<endl;

            // int cc = floor(w/2)+1;
            // float a = drv.at<float>(cc, cc*4+2)/3.1415926*180.0;
            // Mat rot = getRotationMatrix2D(Point(cc,cc), a, 1.0);
            // warpAffine(drv,drv,rot,Size(w, w*scale));

            //GaussianBlur(drv,drv,Size(3,3),0.0,0.0);
            // int i = 9;
            // Mat smt;
            // bilateralFilter(drv,smt,i,i*2, i/2);
            // drv = smt;
            //
            float scale = 9.0;
            Mat bg;

            resize(drv, bg, Size(0,0), scale, scale, INTER_NEAREST);
            bg =(bg+1.0)/2.0;

            // draw lines
            for(int i=0; i<w; i++) {
                for(int j=0;j<w; j++){

                    int px = j*scale+floor(scale/2.0);
                    int py = i*scale+floor(scale/2.0);

                    float dx = drv.at<float>(i, j*3+0);
                    float dy = drv.at<float>(i, j*3+1);

                    float tx = px+dx*scale;
                    float ty = py-dy*scale;

                    line(bg,Point(px,py), Point(tx,ty),Scalar(0,0,0),1,LINE_AA);

                }
            }

            rectangle(bg,Rect( cc   *scale,  cc   *scale,  scale,  scale),Scalar(0,255,0),1);
            rectangle(bg,Rect((cc-1)*scale, (cc-1)*scale,3*scale,3*scale),Scalar(0,200,0),1);
            rectangle(bg,Rect((cc-2)*scale, (cc-2)*scale,5*scale,5*scale),Scalar(0,150,0),1);

            line(bg,Point(cc*scale,cc*scale), Point(cc*scale+mean_x3[0],cc*scale+mean_y3[0]),Scalar(1,0,0),1,LINE_AA);
            line(bg,Point(cc*scale,cc*scale), Point(cc*scale+mean_x5[0],cc*scale+mean_y5[0]),Scalar(0,1,0),1,LINE_AA);

            //linearPolar(bg,bg,Point(w/2.0*scale,w/2.0*scale),w/2.0*scale,WARP_FILL_OUTLIERS);
            imshow("roi",bg);
            break;
            }
        case 1: // DOWN
            circle(img, Point(x,y),3,Scalar(255,255,255),1);
            imshow("image", img);
            break;
    }

}

void setGUI(void){
    namedWindow("image");
    namedWindow("roi",WINDOW_AUTOSIZE);
    setMouseCallback( "image", onMouse, 0 );
}

int main(int argc, char *argv[]) {

    for(int i=0; i<argc; i++) {
        cout << i <<  ": " << argv[i] << endl;
    }

    img = imread(argv[1], IMREAD_COLOR);
    if(img.empty()) return -1;
    cvtColor(img,img,COLOR_BGR2GRAY);

    W = img.cols;
    H = img.rows;

    setGUI();
    imshow("image", img);

    for(;;) {
        int c = waitKey(0);
        if( (c & 255) == 27 ) {
            cout << "Exiting ...\n";
            break;
        }

        switch( (char)c ) {
            case 'b': {
                int i = 16;
                cout << img.type() << " <- " << CV_8UC1 << endl;
                Mat smt;
                bilateralFilter(img,smt,i,i*2, i/2);
                img = smt;
                imshow("image",img);
                break;
            }
            case 'c':
                Canny(img,img,127,255,3);
                img = 255-img;
                imshow("image",img);
                break;
            case 'a':
                adaptiveThreshold(img,img,255,ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY,5,-10);
                imshow("image",img);
                break;

            case 't':
                threshold(img,img,127,255,THRESH_OTSU);
                imshow("image",img);
                break;

            case 'z':
                Mat thin;
                Mat inv = 255 - img;
                thinning(inv,thin);
                img = 255 -thin;
                imshow("image",img);
                break;

        }
    }
    return 0;
}



