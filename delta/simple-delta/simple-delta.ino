#include <Servo.h>

#define S1 9
#define S2 10
#define S3 11

Servo s1,s2,s3;

float PI3 = 2.0*PI/3.0;


void setup() { 
  
  s1.attach(S1);
  s2.attach(S2);
  s3.attach(S3);
  
//  s1.write(0);
//  s2.write(0);
//  s3.write(0);
  
} 

int max_a = 170;
int min_a = 90;
void loop() {
  static float a = 0.0;
  a+=0.01;
  
  int a1,a2,a3;
  
  a1 = min_a+(1.0+sin(a+  PI3))/2.0*(max_a-min_a);
  a2 = min_a+(1.0+sin(a*1.1+2*PI3))/2.0*(max_a-min_a);
  a3 = min_a+(1.0+sin(a*1.2      ))/2.0*(max_a-min_a);
  
  s1.write(a1);
  s2.write(a2);
  s3.write(a3);
  delay(5);
  
}
