
#include <queue>
#define cimg_plugin "plugins/skeleton.h"
#include "CImg.h"
using namespace cimg_library;
#ifndef cimg_imagepath
#define cimg_imagepath "data/"
#endif

// Main procedure
//----------------
int main (int argc, char **argv) {

  cimg_usage("Compute the skeleton of a shape, using Hamilton-Jacobi equations");

  // Read command line arguments
  cimg_help("Input/Output options\n"
            "--------------------");
  const char* file_i = cimg_option("-i",cimg_imagepath "test01.png","Input (black&white) image");
  const int median = cimg_option("-median",0,"Apply median filter");
  const bool invert = cimg_option("-inv",false,"Invert image values");
  const char* file_o = cimg_option("-o",(char*)0,"Output skeleton image");
  const bool display = cimg_option("-visu",true,"Display results");

  cimg_help("Skeleton computation parameters\n"
            "-------------------------------");
  const float thresh = cimg_option("-t",-0.3f,"Threshold");
  const bool curve = cimg_option("-curve",false,"Create medial curve");

  cimg_help("Torsello correction parameters\n"
            "------------------------------");
  const bool correction = cimg_option("-corr",false,"Torsello correction");
  const float dlt1 = 2;
  const float dlt2 = cimg_option("-dlt",1.0f,"Discrete step");

  // Load the image (forcing it to be scalar with 2 values { 0,1 }).
  CImg<unsigned int> image0(file_i), image = image0.get_norm().quantize(2).normalize(0.0f,1.0f);
  if (median) image.blur_median(median);
  if (invert) (image-=1)*=-1;
  if (display) (image0.get_normalize(0,255),image.get_normalize(0,255)).display("Input image - Binary image");

  // Compute distance map.
  CImgList<float> visu;
  CImg<float> distance = image.get_distance(0);
  if (display) visu.insert(distance);

  // Compute the gradient of the distance function, and the flux (divergence) of the gradient field.
  const CImgList<float> grad = distance.get_gradient("xyz");
  CImg<float> flux = image.get_flux(grad,1,1);
  if (display) visu.insert(flux);

  // Use the Torsello correction of the flux if necessary.
  if (correction) {
    CImg<float>
      logdensity = image.get_logdensity(distance,grad,flux,dlt1),
      nflux = image.get_corrected_flux(logdensity,grad,flux,dlt2);
    if (display) visu.insert(logdensity).insert(nflux);
    flux = nflux;
  }

  if (visu) {
    cimglist_apply(visu,normalize)(0,255);
    visu.display(visu.size()==2?"Distance function - Flux":"Distance function - Flux - Log-density - Corrected flux");
  }

  // Compute the skeleton
  const CImg<unsigned int> skel = image.get_skeleton(flux,distance,curve,thresh);
  if (display) {
    (image0.resize(-100,-100,1,3)*=0.7f).get_shared_channel(1)|=skel*255.0;
    image0.draw_image(0,0,0,0,image*255.0,0.5f).display("Image + Skeleton");
  }

  // Save output image if necessary.
  if (file_o) skel.save(file_o);

  return 0;
}
