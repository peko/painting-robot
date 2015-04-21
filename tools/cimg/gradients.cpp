// #include "CImg.h"
using namespace cimg_library;
#ifndef cimg_imagepath
#define cimg_imagepath "../data/"
#endif

// Main procedure
//----------------
int main (int argc, char **argv) {

  const char* file_i  = cimg_option("-i", cimg_imagepath "test03.jpg","Input (black&white) image");

  CImg<unsigned int> src(file_i); 
  // Compute the gradient of the distance function, and the flux (divergence) of the gradient field.
  src.blur_median(5);
  const CImgList<float> grd = src.get_gradient("xy",4);
  // CImg<float> flux = img.get_flux(grd,1,1);
  grd.display();


  return 0;
}
