#ifndef ZHANG_SUEN
#define ZHANG_SUEN

#include <opencv2/opencv.hpp>

using namespace cv;

void thinningIteration(Mat& img, int iter);
void thinning(const Mat& src, Mat& dst);

#endif
