ControlP5 cp5;

RShape shp;
ArrayList< ArrayList<Vector> > shp_points;
ArrayList< Vector > current_shape;

boolean first_point = false;

float cx, cy;

float x =-100;
float y = 130;
float dx, dy;
float tx, ty;
float mx, my;

boolean ready = false;
boolean positioning = true;

float R1 = 120.0; // 140 mm
float R2 = 130.0; // 140 mm
float O  = -PI/6.0;// Offset angle


Slider a1s,a2s, a3s, xs, ys;

float angle1 = 90;
float angle2 = 90;
float angle3 = 90;

float old_angle1 = 90;
float old_angle2 = 90;
float old_angle3 = 90;

float sp = 1.0;

boolean isUp = true;
int   UP_VALUE =  60;
int DOWN_VALUE = 100;


void initSVG() {

  RG.init(this);
  RG.ignoreStyles(true);
  shp = RG.loadShape("cat.svg");

}

void initGUI() {
  
  cp5 = new ControlP5(this);
  
  int yy = 10;
  
  xs = cp5.addSlider("x")
   .setPosition(10, yy)
   .setRange(-R1-R2,R1+R2);
  
  yy+=20;
  
  ys = cp5.addSlider("y")
   .setPosition(10,yy)
   .setRange(-R1-R2,R1+R2);
        
  yy += 20;
  
  a1s = cp5.addSlider("angle1")
   .setPosition(10, yy)
   .setRange(0,180);
  
  yy += 20; 
  
  a2s = cp5.addSlider("angle2")
   .setPosition(10,yy)
   .setRange(0,180);
  
  yy += 20;
  
  a3s = cp5.addSlider("angle3")
   .setPosition(10,yy)
   .setRange(0,180);

}
