/* Sweep
 by BARRAGAN <http://barraganstudio.com> 
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://arduino.cc/en/Tutorial/Sweep
*/ 

#include <Servo.h> 
 
Servo SA;  // create servo object to control a servo 
Servo SB;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards

#define SIZE 10
int points[SIZE] = {79,208,109,134,143,211,72,164,147,167};
struct Angles{
  float a;
  float b;
};

int a = 0;    // variable to store the servo position 
int b = 0;    // variable to store the servo position 
 
void setup()  {
  
  Serial.begin(9600);
  
  SA.attach(9); 
  SB.attach(10); 

} 
 
struct Angles calcAngles(float x, float y) {
 
  struct Angles ang = {0,0};
  float R = 140.0; // 140 mm
  float phi = atan2(y,x);
  float AB = sqrt(x*x+y*y);
  float sigma =acos(AB/2/R);
  
  ang.a = (PI - phi - sigma)*180.0/PI;
  ang.b = (PI-2*sigma)*180/PI;
  
  return ang;
}

void moveTo(float x0,float y0,float x1,float y1) {
  struct Angles ang;
  
  float l = sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));
  float st = l;
  float dx = (x1-x0)/st;
  float dy = (y1-y0)/st;
  
  for(int s=0; s<st; s++) {
    ang = calcAngles(x0,y0);
    SA.write(ang.a);
    SB.write(ang.b);
    delay(10);
    x0+=dx;
    y0+=dy;
  } 
}
void grid(float s=20.0) {
  float sx = 60;
  float sy = 60;
  float H = 140;
  float W = 140;
  
  for(float x=0; x<W; x+=s) {
      moveTo(sx+x,sy,sx+x,sy+H);
      moveTo(sx+x+s/2,sy+H,sx+x+s/2,sy);
  }
  
  for(float y=0; y<H; y+=s) {
      moveTo(sx,sy+y,sx+W,sy+y);
      moveTo(sx+W,sy+y+s/2,sx,sy+y+s/2);   
  }

  return;

}

void spiral(float dr=0.25) {
 float cx = 130;
  float cy = 130;
  float R=5.0; 
  float dda=0.0;
  
  while(R<70.0) {
    float s = 5.0;
    float da = 2*PI/s*2;
    for(int i=0; i<s; i++) {
      float x0 = cx+sin(i*da+dda)*R;
      float y0 = cy+cos(i*da+dda)*R;
      float x1 = cx+sin((i+1)*da+dda)*R;
      float y1 = cy+cos((i+1)*da+dda)*R;
      moveTo(x0,y0,x1,y1);
      R+=dr;
      dda+=0.01;
    }
  }
}

void loop() {
//  return;
  
// grid();
spiral();

//  
//  float x0, y0, x1, y1;
//  for(int i = 0; i<SIZE>>1; i++){
//    if (i==0) {
//      x0 = points[SIZE-2];
//      y0 = points[SIZE-1];
//    }
//    x1 = points[i*2];
//    y1 = points[i*2+1];
//    moveTo(x0,y0, x1,y1);
//    x0=x1;
//    y0=y1;
//  } 
//  
//  float x = 70;
//  float y = 130;
//  float dx = 80;
//  float dy = 80;
//  
//  moveTo(x,   y   ,x   ,y+dy);
//  moveTo(x   ,y+dy,x+dx,y+dy);
//  moveTo(x+dx,y+dy,x+dx,y   );
//  moveTo(x+dx,y   ,x   ,y   );
// 
  

//----------------------------------------
//  
//  static float angle=0;
//  angle+=0.01;
//  a = 100+sin(angle*1.5)*15;
//  b = 100+cos(angle*2)*15;
//  SA.write(a);
//  SB.write(b);
//  delay(2);
  

// ----------------------------------------
  
//  for(a = 50; a <= 100; a += 5) {
//    SA.write(a);
//    delay(500);
//    for(b = 45; b <= 135; b += 5) {                                  
//      SB.write(b);              
//      delay(15);
//    }
//    for(b = 135; b >= 45; b -= 5) {                                  
//      SB.write(b);              
//      delay(15);
//    }
//  }
} 


