import processing.serial.*;

Serial port;

int D = 1105;
int L =  750;

void setup() {
  
  size(D, D);
  
  print(Serial.list());
  port = new Serial(this, "/dev/ttyUSB0",115200);
}

void draw() {

}

void keyPressed() {
  if(key==CODED) {
    switch(keyCode) {
      case UP:
         port.write("m 0.0 -10.0 ");
         break;
      case DOWN:
         port.write("m 0.0 10.0 ");
         break;
      case LEFT:
         port.write("m -10.0 0.0 ");
         break;
      case RIGHT:
         port.write("m 10.0 0.0 ");
         break;

    }
  }
  println(key);
}
