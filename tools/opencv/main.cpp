/**
 * Code for thinning a binary image using Zhang-Suen algorithm.
 *
 * Author:  Nash (nash [at] opencv-code [dot] com) 
 * Website: http://opencv-code.com
 */

#include "main.h"
#include "zhang-suen.h"

void gui_callback(int, void*) {

    cvtColor(src, dns, COLOR_BGR2GRAY);
    fastNlMeansDenoising(dns, dns,dns_str, dns_tmp, dns_src);
    imshow(dns_win, dns);

    if(thr_rds<=1) thr_rds=3;
    adaptiveThreshold( dns, thr, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, thr_rds*2+1, thr_ofs-127 );
    imshow(thr_win, thr);

    bitwise_not(thr,skl);
    // threshold(bw, bw, 64, 255, THRESH_BINARY);
    
    thinning(skl, skl);
    // imwrite("../data/test03_sk.png",skl);
    // int morph_size = 1; // *2+1 = 3 
    // unsigned char e[3][3] = {
    //     {1,1,1},
    //     {1,0,1},
    //     {1,1,1}};
    // Mat el = Mat(3,3,CV_8U,e);
    // morphologyEx( skl, skl, MORPH_BLACKHAT, element );
    // erode(skl,skl,el);
    // erode(skl,skl,el);
    
    imshow(skl_win,skl);

    // int dilation_size = 1;
    // Mat dil = getStructuringElement( MORPH_RECT,
    //                                    Size( 2*dilation_size + 1, 2*dilation_size+1 ),
    //                                    Point( dilation_size, dilation_size ) );
    // erode(bw,bw, dil);

    // vector<vector<Point> > contours;
    // vector<Vec4i> hierarchy;    
    // findContours( skl, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

    // // Draw contours
    // Mat cnt = Mat::zeros( src.size(), CV_8UC3 );
    // for( int i = 0; i< contours.size(); i++ ) {
    //    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    //    drawContours(cnt, contours, i, color, 1, 8, hierarchy, 0, Point() );
    // }
    
    // drawContours(cnt, contours, 0, Scalar(255,0,0), 1, 8, hierarchy, 0, Point() );

    // imshow(cnt_win, cnt);

    // imshow("Threshold", thr);
    // imshow("Contours" , cnt);
    
    
}

void createGui() {
    // Create Windows
    namedWindow( src_win, WINDOW_AUTOSIZE );
    namedWindow( dns_win, WINDOW_AUTOSIZE );
    namedWindow( thr_win, WINDOW_AUTOSIZE );
    // namedWindow( cnt_win, WINDOW_AUTOSIZE );
    
    imshow( src_win, src );

    createTrackbar( "strength", dns_win, &dns_str, dns_str_max, gui_callback );

    createTrackbar( "radius", thr_win, &thr_rds, thr_rds_max, gui_callback );
    createTrackbar( "offset", thr_win, &thr_ofs, thr_ofs_max, gui_callback );
    
    gui_callback( 0, 0 );

}

int main() {

	src = imread("../data/test03.jpg");
    if (!src.data) return -1;

    createGui();

	waitKey();
	return 0;
}