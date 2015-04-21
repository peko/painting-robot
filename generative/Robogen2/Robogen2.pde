
import java.util.Map;

int W = 512;
int H = 512;
Part hip = new Hip();

void setup() {
  size(W, H);
}

void draw() {
  background(0);
  stroke(255);
  noFill();
  translate(W/2,H/2);
  scale(1,-1);
  
  Arm armL = (Arm) hip.parts.get("weist").parts.get("body").parts.get("armL");
  while((armL = (Arm) armL.parts.get("segment")) != null ) {
    armL.a=PI/12.0*sin(millis()/100.0);
  }
  Arm armR = (Arm) hip.parts.get("weist").parts.get("body").parts.get("armR");
  while((armR = (Arm) armR.parts.get("segment")) != null ) {
    armR.a=-PI/12.0*sin(millis()/100.0);
  }
  
  hip.draw();
  
  
}

class Part {

  HashMap<String, Part> parts = new HashMap<String, Part>();
  float x=0, y=0, a=0;
  float W=10, H=10;

  Part() {
  }

  void draw(){
    pushMatrix();
    translate(x,y);
    rotate(a);
    drawSelf();
    drawChildren();
    popMatrix();
  }

  void drawSelf() {
  }

  void drawChildren() {
    for (Map.Entry me : parts.entrySet()) {
      Part part = (Part) me.getValue();
      part.draw();
    }
  }
  
}

class Hip extends Part {
  
  float W = 40, H=20; 
  
  Hip() {
    Weist weist = new Weist();
    weist.y=H/2.0;
    parts.put("weist",(Part) weist);
    Leg legL = new Leg();
    legL.a = PI;
    legL.y=-H/2.0;
    legL.x=-W/2.0+legL.W/2.0;
    parts.put("legL",(Part) legL);
    Leg legR = new Leg();
    legR.a = PI;
    legR.y=-H/2.0;
    legR.x= W/2.0-legR.W/2.0;
    parts.put("legR",(Part) legR);
  }
  
  void drawSelf() {
    rectMode(CENTER);
    rect(0,0,W,H);
  }
  
}

class Weist extends Part {
  
  float W=20, H=10;
  
  Weist() {
    Body body = new Body();
    body.y = H;
    parts.put("body", body);
  }
  
  void drawSelf() {
    rectMode(CORNERS);
    rect(-W/2.0,0,W/2.0,H);
  }
  
}


class Spine extends Part {
  
  float W=20, H=10;
  
  Spine() {
  }
  
  void drawSelf() {
    rectMode(CORNERS);
    rect(-W/2.0,0,W/2.0,H);
  }
  
}

class Body extends Part {
  
  float W=50, H=60;
  
  Body() {
    Neck neck = new Neck();
    neck.y = H;
    parts.put("neck", (Part)neck);
    Arm armL = new Arm(12,-PI/24.0);
    armL.x = W/2.0;
    armL.y = H-armL.W/2.0;
    armL.a = -PI/2.0;
    parts.put("armL", armL);
    Arm armR = new Arm(12, PI/24.0);
    armR.x =-W/2.0;
    armR.y = H-armR.W/2.0;
    armR.a = PI/2.0;
    parts.put("armR", armR);
  }
  
  void drawSelf() {
    rectMode(CORNERS);
    rect(-W/2.0,0,W/2.0,H);
  }
 
}

class Neck extends Part {
  
  float W=10, H=10;
  
  Neck() {
    Head head = new Head();
    head.y = H;
    parts.put("head", head);
  }
  
  void drawSelf() {
    rectMode(CORNERS);
    rect(-W/2.0,0,W/2.0,H);
  }
 
}

class Head extends Part {
  
  float W=30, H=30;
  
  Head() {
    
  }
  
  void drawSelf() {
    rectMode(CORNERS);
    rect(-W/2.0,0,W/2.0,H);
  }
 
}


class Arm extends Part {
  
  float W=10, H=5;
  
  Arm(int segments,float angle) {
    if(segments>0) {
      Arm segment = new Arm(segments-1,angle);
      segment.y=H;
      segment.a=angle;
      parts.put("segment",segment);
    } else {
      
    }
    
  }
  
  void drawSelf() {
    rectMode(CORNERS);
    rect(-W/2.0,0,W/2.0,H);
  }
 
}

class Leg extends Part {
  
  float W=10, H=40;
  
  Leg() {
    
  }
  
  void drawSelf() {
    rectMode(CORNERS);
    rect(-W/2.0,0,W/2.0,H);
  }
 
}

