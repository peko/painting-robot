#include <Servo.h>

///  Servo SG90 min pulse length - 500 us max pulse length - 2500 us (best performed 800 to 2300)

#define MIN_PULSE 800
#define MAX_PULSE 2300

#define S1 9
#define S2 10
#define S3 11

Servo s1,s2,s3;

float PI3 = 2.0*PI/3.0;


void setup() { 
  
  s1.attach(S1, MIN_PULSE, MAX_PULSE);
  s2.attach(S2, MIN_PULSE, MAX_PULSE);
  s3.attach(S3, MIN_PULSE, MAX_PULSE);
  
  s1.write(120);
  s2.write(120);
  s3.write(120);
  delay(1000);
  
  Serial.begin(115200);
} 

int max_a = 160;
int min_a = 60;
void loop() {
  
  static float a = 0.0;
  a+=0.001;
  
  float a1,a2,a3;
  
  a1 = min_a+(1.0+sin(a+  PI3))/2.0*(max_a-min_a);
  a2 = min_a+(1.0+sin(a+2*PI3))/2.0*(max_a-min_a);
  a3 = min_a+(1.0+sin(a      ))/2.0*(max_a-min_a);
  
  s1.write(a1);
  s2.write(a2);
  s3.write(a3);
  delay(1);
  
}
