void moveTo(float tx, float ty) {
  
  this.tx = tx;
  this.ty = ty;
  
  // dx = (tx-x);
  // dy = (ty-y);
  
  // float dl = sqrt(dx*dx + dy*dy);
  
  // dx/=dl*sp;
  // dy/=dl*sp;
  
}

void drawShape() {
    for(int i=0; i<shp_points.size(); i++) {
      ArrayList<Vector> shape = shp_points.get(i);
      for(int j=0; j<shape.size(); j++){
        Vector p = shape.get(j);
        crs(p.x,p.y,3);        
      }
    }
}

void drawCurrentShape() {
  if(current_shape == null || current_shape.isEmpty()) return;
  stroke(200,20,20,200);
  for(int j=0; j<current_shape.size(); j++){
    Vector p = current_shape.get(j);
    crs(p.x,p.y,5);        
  }
}

void drawZone(){
  float stp = 32;
  for(int i=0; i<=stp; i++) {
    pushMatrix();
      rotate(i*PI/stp);
      translate(R1, 0.0);
      arc(0, 0, R2*2.0, R2*2.0, 0, PI);
    popMatrix();
  }
}


void crs(float x,float y, float s) {
  line(x-s, y, x+s, y);
  line(x, y-s, x, y+s);
}

Vector calcAngles(Vector v) {
 
  float AB = sqrt(v.x*v.x+v.y*v.y);
  
  if(AB>R1+R2) return null;
  
  float phi = atan2(v.x,v.y);
  
  float alpha = acos((AB*AB+R1*R1-R2*R2)/(2.0*AB*R1));
  float beta = PI-acos((R1*R1+R2*R2-AB*AB)/(2.0*R1*R2));
  float a = phi + alpha + O;
  float b = beta;
  
  return new Vector(a, b);
}


class Vector {
  float x;
  float y;
  Vector(float x, float y){
    this.x = x;
    this.y = y;
  };
}
