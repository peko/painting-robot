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
         move(0.0, -10.0);
         break;
      case DOWN:
         move(0.0, 10.0);
         break;
      case LEFT:
         move(-10.0,0.0);
         break;
      case RIGHT:
         move( 10.0,0.0);
         break;
      case 34: // page up
         up();
         break;
      case 33: // page down
         down(); 
         break;
      default:
        print(" "+keyCode);
    }
  }
  //println(key);
}

void move(float dx, float dy) {
  port.write("m "+nf(dx,0,2)+" "+nf(dy,0,2)+" ");
}

void up() {
  port.write("u ");
}

void down() {
  port.write("d ");
}
