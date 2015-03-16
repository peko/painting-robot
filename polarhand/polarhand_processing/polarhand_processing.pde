
import processing.serial.*;
import controlP5.*;

import geomerative.*;

void setup(){
  
  size(800, 600);

  cx = width/2.0;
  cy = height/2.0;
  
  tx = x;
  ty = y;
   
  shp_points = new ArrayList< ArrayList<Vector> >();
   
  initSVG();  
  initSerial();
  initGUI();
  
}


void draw() {
  
  readSerial();
  
  background(0);
  
  pushMatrix();
  
  translate(cx,cy);
  scale(1.0,-1.0);
  
  mx = mouseX-cx;
  my = cy-mouseY;
  
  // if(abs(tx-x)>1.0/sp) x+=dx; else x = tx;
  // if(abs(ty-y)>1.0/sp) y+=dy; else y = ty;
   
  // if(x==tx && y ==ty) nextPoint();
  
  x = tx;
  y = ty;
  
  rect(x-5, y-5, 10, 10);
  
  stroke(200, 20, 20, 200);
  rect(tx-8, ty-8, 16, 16);
  
  stroke(255,100);  
  
  if(positioning) {
    
    pushMatrix();
      translate(x,y);
      shp.draw();
    popMatrix();
    
  }
  
  drawShape();
  drawCurrentShape();
  
  
  stroke(255, 20);  
  
  line(       0.0, -height/2.0,       0.0, height/2.0);
  line(-width/2.0,         0.0, width/2.0,        0.0);
  
  rotate(O);
  
  line(-width, 0, width, 0);
  ellipse(0,0, (R1+R2)*2.0, (R1+R2)*2.0);
  
  drawZone();
  
  stroke(255,150); 
  
  Vector v = calcAngles(new Vector(x, y));
  
  if(v!=null) {
    
    a1s.setValue((PI/2.0-v.x)/PI*180);
    a2s.setValue(180-v.y/PI*180);
    
    xs.setValue(x);
    ys.setValue(y);
      
    noFill();
    pushMatrix();
    
      arc(0, 0, R1*2.0, R1*2.0, 0, PI);
  
      rotate(PI/2.0-v.x);
      line(0.0, 0.0, R1, 0.0);
      
      translate(R1,0.0);
      arc(0, 0, R2*2.0, R2*2.0, 0, PI);
      
      rotate(v.y);
      line(0.0, 0.0, R2, 0.0);
    
    popMatrix();
  }
  
  popMatrix();
  //if(true) return;
  sendAngles();
}

void nextPoint() {
  
  if(current_shape == null || current_shape.isEmpty()) {
    // no more path no more points
    if(shp_points.isEmpty()) {
      return;
    }
    up();
    first_point = true;
    current_shape = shp_points.remove(0);
    return;
  }
  
  if(isUp && !first_point) down();
  
  Vector p = current_shape.remove(0);
  moveTo(p.x, p.y);
  
  if(first_point) first_point = false;
}

void up() {
  println("Up");
  angle3=UP_VALUE;
  a3s.setValue(UP_VALUE);
  isUp = true;
}

void down() {
  isUp = false;
  angle3 = DOWN_VALUE;
  a3s.setValue(DOWN_VALUE);
}
