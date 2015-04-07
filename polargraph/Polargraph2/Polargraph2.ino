#define MINIMUM_DELAY_BETWEEN_STEPS 20000
#define STEPS_PER_REVOLUTION 64*16
// #define DISTANCE_BETWEEN_MOTORS 1175
#define DISTANCE_BETWEEN_MOTORS 1175
#define LEFT_STRING_LENGTH 750
#define RIGHT_STRING_LENGTH 750
#define VELOCITY 10.0
#define WIDTH 300.0
#define HEIGHT 300.0
#define SEED 13

#define L0 10
#define L1 11
#define L2 12
#define L3 13

#define R0 2
#define R1 3
#define R2 4
#define R3 5


uint8_t stepper_states[36] = {
   LOW,  LOW,  LOW, HIGH, // 0
   LOW,  LOW, HIGH, HIGH, // 1
   LOW,  LOW, HIGH,  LOW, // 2
   LOW, HIGH, HIGH,  LOW, // 3
   LOW, HIGH,  LOW,  LOW, // 4
  HIGH, HIGH,  LOW,  LOW, // 5
  HIGH,  LOW,  LOW,  LOW, // 6
  HIGH,  LOW,  LOW, HIGH, // 7
   LOW,  LOW,  LOW,  LOW  // OFF STATE 
};

struct vector {
  double x;
  double y;
};

typedef struct vector vector; 

unsigned char pins_left [4] = {L0,L1,L2,L3}; // direct  order
unsigned char pins_right[4] = {R3,R2,R1,R0}; // reverse order

double 
  l, // x coordinate relative left motor mm
  L, Ln, // Left string length mm
  r, // x coordinate realtive right motor mm
  R, Rn, // Right string length mm
  h, // Height relative motors mm 
  W, // Distance between motors mm
  V, // Moving velocity mm/s
  S; // Steps of motor per mm; 
  
//#include "tests.h"
//#include "panda.h"
#include "planes.h"

void setup(){
  
  Serial.begin(115200);
  randomSeed(SEED);
  
  // Pulley D = 12.2 -> Pi*D = 38.32... mm per 360 revolution
  // Step motor angle per step 5.625 -> 64 steps per 360 revolution
  // 64/38.32... = 1.66982235375 steps per 1 mm
  
  S = STEPS_PER_REVOLUTION / (PI*12.2);

  W = DISTANCE_BETWEEN_MOTORS;
  L = LEFT_STRING_LENGTH;
  R = RIGHT_STRING_LENGTH;
  V = VELOCITY; // mm/s

  calcGeometry();
  
  for(int i=0;i<4;i++) {
    pinMode(pins_left [i], OUTPUT); 
    pinMode(pins_right[i], OUTPUT); 
  }
  
  delay(1000);

}

void border() {
  move({-WIDTH/2.0,-HEIGHT/2.0});
  move({WIDTH,0});
  move({0,HEIGHT});
  move({-WIDTH,0});
  move({0,-HEIGHT});
  move({WIDTH,0});
  move({-WIDTH/2.0,HEIGHT/2.0});
}

void spiral_exp() {
  float i = 1;
  float di = 0.5;
  float s = random(20)+10;
  int d = 50;
  while(i<s) {  
    move({ i,   0});delay(d); i+=di;
    move({ 0,   i});delay(d); i+=di;
    move({-i,   0});delay(d); i+=di;
    move({ 0,  -i});delay(d); i+=di;
    di+=0.1;
  }
}


void wigly(float R) {
  float steps= 36.0;
  
  struct vector v = {R,0};
  
  float alpha = 10.0/180.0*PI;
  double cp = cos(alpha);
  double sp = sin(alpha);
  double cm = cos(-alpha);
  double sm = sin(-alpha);
  
  while (1) { 
    
    R=0.5+5.0*random(100)/100.0;
    float c = random(36);
    for (int i = 0; i<c; i++) {
      norm(&v, R);
      v.x= v.x*cp-v.y*sp;
      v.y= v.x*sp+v.y*cp;
      move(v);
    }
    c = 18+random(36);
    R=0.1+random(100)/100.0;
    for (int i = 0; i<c; i++) {
      norm(&v, R);
      v.x = v.x*cm-v.y*sm;
      v.y = v.x*sm+v.y*cm;
      move(v);
    }  
  }
}


void loop() {
//move_random(20.0);
//spiral_exp(); 
wigly(100);
//  draw_ailove(random(5));
//  move_random(5+random(20)); 
// trajectory();
//
//
//
////draw_panda(0.3);
//move_random(20.0);
//circle(2);
//move({30,0});
//circle(2);
//move({-20,0});
//circle(1);
//move({15,0});
//circle(1);
//delay(1000);

}


void move_random(float d) {
  double angle=random(360)/180.0*PI;
  vector v = {d*sin(angle), d*cos(angle)};
  move(v);
}

void calcGeometry() {

  double p = (W + L + R)/2.0;            // half perimeter
  double ir = sqrt((p-L)*(p-R)*(p-W)/p); // incircle radius
  h = 2.0 * p * ir / W;                  // height
  l = sqrt(L*L-h*h);                     // left projection
  r = W - l;                             // right projection
  
  Serial.println("Calc geomtry:" );
  print_f("Half perim" , p , "mm");
  print_f("In circle r", ir, "mm");
  print_f("Height"     , h , "mm");
  print_f("Left"       , l , "mm");
  print_f("Rigth"      , r , "mm");
  Serial.println("-------------" );
}

// covert cartesian dx, dy to polar du,dv
struct vector toPolar( struct vector c) {
  
  l += c.x;
  h += c.y;
  Ln = sqrt(h*h+l*l);
  
  r = W-l;
  Rn = sqrt(h*h+r*r);
  
  struct vector p = { Ln-L, Rn-R };
  
  return p;
  
}

void print_f(char* title, float data,  char* units){
  Serial.print(title);
  Serial.print(": ");
  Serial.print(data);
  Serial.print(" ");
  Serial.println(units);  
};

void print_v(char* title, struct vector v,  char* units){
  Serial.print(title);
  Serial.print(": (");
  Serial.print(v.x);
  Serial.print(", ");
  Serial.print(v.y);
  Serial.print(") ");
  Serial.println(units);  
};

void move(struct vector c) {
  
  print_v("Move", c, "mm");
  
  unsigned long mc=0, mcl=0, mcr=0;
  
  double d = sqrt(c.x*c.x+c.y*c.y); // distance to move 
  print_f("Dist", d, "mm");
  
  double t = d/V;                   // time to move distance / velocity
  print_f("Time", t, "s");
  
  struct vector p = toPolar(c);     // polar contractions
  print_v("dL,dR",p,"mm");
  
  // convert mm to Steps
  p.x *= S; 
  p.y *= S; 
  print_v("dL,dR",p,"steps");
    
  double stepsL = abs(p.x);
  double stepsR = abs(p.y);

  unsigned int tl = 1000000.0 * t / stepsL; // time per step us/step
  unsigned int tr = 1000000.0 * t / stepsR; // time per step us/step
  
  struct vector dt = {tl,tr};
  print_v("tl,tr",dt,"us");
  
  unsigned long start_time = micros();
  while(stepsL>0 || stepsR>0){
    
    mc = micros();
    if(stepsR>0 && mc-mcr>=tr){

      if(p.y<0) doStepR(-1);
      else      doStepR(1);
      mcr = micros();
      stepsR--;
    }
    
    mc = micros();
    if(stepsL>0 && mc-mcl>=tl) {
      
      if(p.x<0) doStepL(-1);
      else      doStepL(1);
      mcl = micros();  
      
      stepsL--;
    }
    
  }
  
  print_f("Duration", (micros()-start_time)/1000000.0, "s");
  
  L = Ln; // new L length
  R = Rn; // new R length
   
}

void norm(struct vector * v, double r){
  double l = sqrt(v->x * v->x + v->y * v->y);
  v->x = v->x/l*r;
  v->y = v->y/l*r;
  
}


void doStepL(char d) {
 
  static char step = 8; // OFF STEP
  for(int i=0; i<4; i++) {
    digitalWrite(pins_left[i],stepper_states[i+(step<<2)]);
  }
  step+=d;
  if(step>7) step=0;
  if(step<0) step=7;
}

void doStepR(char d) {
  static char step = 8; // OFF STEP
  for(int i=0; i<4; i++) {
    digitalWrite(pins_right[i],stepper_states[i+(step<<2)]);
  }
  step+=d;
  if(step>7) step=0;
  if(step<0) step=7;
}

