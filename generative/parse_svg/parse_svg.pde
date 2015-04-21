import geomerative.*;

RShape shp;
RShape poly;
RPoint[] pnts;


void setup() {
  size(800, 600);
  RG.init(this);
  shp = RG.loadShape("horse.svg");
  shp = RG.centerIn(shp, g);
  
  pnts = shp.getPoints();

}

void draw() {
  
  // We decided the separation between the polygon points dependent of the mouseX
  
  // We create the polygonized version

  
  
  background(255);
  translate(width>>1, height>>1);
  if(false) {
    RG.ignoreStyles();
    noFill();
    
    stroke(0,50);
    RG.shape(shp);
    for(int i=0; i<pnts.length; i++){
        line(pnts[i].x,pnts[i].y-2,pnts[i].x,pnts[i].y+2);
        line(pnts[i].x-2,pnts[i].y,pnts[i].x+2,pnts[i].y);
    }
  }
  
  stroke(0, 100);
  
  float s = map(constrain(mouseX, 100, width-100), 100, width-100, 5.0, 50.0);
   
  RG.setPolygonizer(RG.UNIFORMLENGTH);
  RG.setPolygonizerLength(s);
    
  poly = RG.polygonize(shp);
  
  RPoint[] ppnts = poly.getPoints();
  for(int i=0; i<ppnts.length; i++){
      float x = ppnts[i].x;
      float y = ppnts[i].y;
      line(x  ,y-2,x  ,y+2);
      line(x-2,y  ,x+2,y  );
  }
  
  RG.shape(poly);
    
//  
//  //println(angle);

//  
}
