// denoise
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace cv;


const char* src_win = "Source";
const char* dns_win = "Denoise";
const char* thr_win = "Threshold";
const char* skl_win = "Skelet";
const char* cnt_win = "Contours";



// denoise params
int dns_tmp     =  7;
int dns_src     = 21;
int dns_str     = 20;
int dns_str_max = 100;

// threshold params
int thr_rds     = 10;
int thr_rds_max = 25;
int thr_ofs     = 140;
int thr_ofs_max = 255;

 
Mat src;
Mat dns;
Mat thr;
Mat skl;

RNG rng(12345);
