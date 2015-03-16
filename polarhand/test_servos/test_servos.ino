/* Sweep
 by BARRAGAN <http://barraganstudio.com> 
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://arduino.cc/en/Tutorial/Sweep
*/ 

#include <Servo.h> 
 
Servo s1,s2, s3;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
 
int pos = 0;    // variable to store the servo position 
 
void setup() { 
  s1.attach(9);  // attaches the servo on pin 9 to the servo object 
  s2.attach(10);  // attaches the servo on pin 9 to the servo object 
  s3.attach(11);  // attaches the servo on pin 9 to the servo object 

} 
 
void loop() { 
  
  return;
  
  int bg = 30;
  int nd = 60;
  
  
//  return;
//  
  for(pos = bg; pos <= nd; pos += 1) {
    s3.write(pos);              
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  
  for(pos = nd; pos >= bg; pos -= 1) {
    s3.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  
  
  return;
  for(pos = bg; pos <= nd; pos += 1) {
    s1.write(pos);              
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  
  for(pos = bg; pos <= nd; pos += 1) {
    s2.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  
  for(pos = nd; pos>=bg; pos-=1) {                                
    s1.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
 
  for(pos = nd; pos>=bg; pos-=1) {                                
    s2.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  
} 

