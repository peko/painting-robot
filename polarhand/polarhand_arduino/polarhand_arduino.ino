#define S1 10
#define S2 9
#define S3 11


#include <Servo.h>

Servo s1,s2,s3;
float a1,a2,a3;
float o1,o2,o3;

void setup() {

  Serial.begin(9600);
  
  s1.attach(S1);
  s2.attach(S2);
  s3.attach(S3);
  
  Serial.println("RDY");
    
}

void loop() {
    while (Serial.available()) {
      char cmd = Serial.read();
      if(cmd==32 || cmd==-1) return;
      switch(cmd){
        case 's':
          a1 = Serial.parseFloat();
          s1.write(a1);
          a2 = Serial.parseFloat();
          s2.write(a2);
          a3 = Serial.parseFloat();
          s3.write(a3);
          
          int a = max(max(abs(a1-o1),abs(a2-o2)),abs(a3-o3));
          o1 = a1;
          o2 = a2;
          o3 = a3;
          
          delay(a);
          
          Serial.print("s ");
          Serial.print(a1);
          Serial.print(" ");
          Serial.print(a2);
          Serial.print(" ");
          Serial.println(a3);
          Serial.println("OK");
          break;
      }  
  }

}
