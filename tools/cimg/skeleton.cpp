
void otsu(CImg<unsigned int>  *image) {
  
  float histogram[256];//Array holding histogram values for each value of t
  float probability[256];//Array holding probability values for each value of t

    //Otsu's algorithm variables
  float p1; //first value of the probability in Otsu's algorithm with t = 1
  float q1; //first value of q qith t = 1
  float q1prev;//previos value of q1
  float q1next;//next value of q1 , aka q1(t+1)
  float mu1; //average of first group, before threshold
  float mu2;//average of second group, after threshold
  float mu1next;//next value of the mean for group 1(mu1(t+1))
  float mu2next;//next value of the mean for group 2(mu2(t+1))
  float mu;//the mean gray level value of the entire image before thresholding
  float betweenvariance=0;//between group variance
  float maxbetweenvariance=0;//max between group variance
  float sum=0;//sum of all histogram values to calculate the mean grey level value of the imagem values before threshholding
  int pixelvalue=0;//value of a pixel
  float optimizedthresh=0;//optimized threshhold, at the end of otsu's algorithm this will be the thresshold with the max between group vairance
  
  for(int k =0;k<=255;k++){
    histogram[k]=0;
  }
   //populate probability array with 0 for all values initially
  for(int k =0;k<=255;k++){
   probability[k]=0;
  }
  
  //cycle through entire image and get pixel values and populate the histogram with them
  for (int i =0;i<image->height();i++) {
    for (int j =0;j<image->width();j++){
      pixelvalue=image->atXY(j,i,0);//store pixel value of image at (width,height)
      sum+=pixelvalue;//increment the sum of all pixel values , used to calculate the average grey level value of the entire image later one
      histogram[pixelvalue]++;
    }
  }

  //calculate the probability of each histogram value and store them in the probability array
  for(int k =0;k<=255;k++){
      probability[k]=histogram[k]/(image->width()*image->height());
  }

  //Initial probability p1 which is equal to just the probability of the 0 grey value
  p1 = probability[0];
  //initial q which is the sum of probabilities before 1, which is equal to p1
  q1=p1;
  //initial mean before the 1 has to be 0. mu(1)=0 
  mu1=0;
  //initial mean after the 0. Initially set to 0. This gets reset in the algorithm
  mu2=0;
  //mean grey level (mu) calculation
  mu=sum/(image->width()*image->height());
  
  q1prev=q1;//set previous q1, q1(t), to equal the current q1
  for (int t = 1;t<255;t++){
    q1next=q1prev+probability[t+1]; //q1next-q1(t+1)
    mu1next=(q1prev*mu1+(t+1)*(probability[t+1]))/q1next;//set mu1(t+1)
    mu2next= (mu-q1next*mu1next)/(1-q1next);//set mu2(t+1)
    betweenvariance = q1prev*(1-q1prev)*((mu1-mu2)*(mu1-mu2));//calculate between group variance
    //max between group variance is initially set to 0. Change the max between group variance, and change the optimized threshold to t if the current variance is > max.
    if (betweenvariance>maxbetweenvariance){
      maxbetweenvariance=betweenvariance;
      optimizedthresh=t;//set new optimized threshhold
    }
    
    q1prev=q1next;//set q1(t) to be used in the next iteration to be equal to the current q1(t+1) which is q1next
    mu1=mu1next;//do the same for mu1. set mu1(t) of next iteration to equal the current mu1(t+1)
    mu2=mu2next;//set mu2(t) of next iteration to equal the current mu2(t+1)
    
    if(q1next==0){
      mu1=0;//this eliminates divide by 0 errors because the calculate of the next mu1 would be undefend if the next value of q1 is 0, according to the otsu recursive algorithm
    }

  }
  //cout << optimizedthresh;
  //set image values based on the optimized threshhold calculated above.
  for (int i =0;i<image->height();i++)
  {
    for (int j =0;j<image->width();j++){
      pixelvalue=image->atXY(j,i,0);
      if (pixelvalue<optimizedthresh){//if pixelvalue is< than the threshhold, set it to 200
        pixelvalue=image->atXY(j,i,0)=200;
        pixelvalue=image->atXY(j,i,1)=200;
        pixelvalue=image->atXY(j,i,2)=200;}
      else //if pixelvalue is> than the threshhold, set it to 0
      {
        pixelvalue=image->atXY(j,i,0)=0;
        pixelvalue=image->atXY(j,i,1)=0;
        pixelvalue=image->atXY(j,i,2)=0;}
      {
      
      }
    }
  }
}

// Main procedure
//----------------
int main (int argc, char **argv) {

  cimg_usage("Compute the skeleton of a shape, using Hamilton-Jacobi equations");

  // Read command line arguments
  cimg_help("Input/Output options\n"
            "--------------------");
  const char* file_i  = cimg_option("-i",cimg_imagepath "test01.png","Input (black&white) image");
  const int   median  = cimg_option("-median",0,"Apply median filter");
  const bool  invert  = cimg_option("-inv",true,"Invert image values");
  const char* file_o  = cimg_option("-o",(char*)0,"Output skeleton image");
  const bool  display = cimg_option("-visu",true,"Display results");

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
  CImg<unsigned int> image0(file_i); 
  otsu(&image0);
  CImg<unsigned int> image = image0.get_norm().quantize(2).normalize(0.0f,1.0f);
  

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
