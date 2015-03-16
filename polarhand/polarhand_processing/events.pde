void keyReleased() {
  switch(key) {
    case 'd':
      positioning = false;
      RPoint[][] points = shp.getPointsInPaths();
      for(int i=0; i<points.length; i++){
        ArrayList<Vector> shape = new ArrayList<Vector>();
        shp_points.add(shape);
        for(int j=0; j<points[i].length; j++){
          RPoint p = points[i][j];
          shape.add(new Vector(p.x+x,p.y+y));
        }
      }
      break;
    case 'n':
      nextPoint();
  }
}

void mouseWheel(MouseEvent event) {
  a3s.setValue( a3s.getValue()+event.getCount());
}


void mouseClicked() {
  
  ArrayList< Vector > shape = new ArrayList< Vector >();
  
  dx = (mx-x);
  dy = (my-y);
  
  float dl = sqrt(dx*dx + dy*dy);

  if(dl>1) {
    for(int i=0; i<dl; i++) {
      Vector p = new Vector(x+dx/dl*i, y+dy/dl*i);
      shape.add(p);
    }
  }
  shape.add(new Vector(mx,my));
  shp_points.add(shape);
  
}
