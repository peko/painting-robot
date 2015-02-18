#define LEFT 500

unsigned char p[8] = {2,3,4,5, 13,12,11,10};

struct vector {
  double x;
  double y;
};

int Steps = 0;

boolean Direction = true;// gre
unsigned long last_time;
unsigned long currentMillis ;
int steps_left=LEFT;
long time;

double 
  l, // x coordinate relative left motor mm
  L, // Left string length mm
  r, // x coordinate realtive right motor mm
  R, // Right string length mm
  h, // Height relative motors mm 
  W, // Distance between motors mm
  V, // Moving velocity mm/s
  S; // Steps of motor per mm; 
  


void setup(){
  
  Serial.begin(115200);
  
  
  // Pulley D = 12.2 -> Pi*D = 38.32... mm per 360 revolution
  // Step motor angle per step 5.625 -> 64 steps per 360 revolution
  // 64/38.32... = 1.66982235375 steps per 1 mm
  
  S = 1.66982235375;
  P = 100;
  
  W = 1000;
  L = 1000;
  R = 1000;
  V = 5.0; // mm/s
  
  for(int i=0;i<8;i++)
    pinMode(p[i], OUTPUT); 
  
  delay(1000);

}

// covert cartesian dx, dy to polar du,dv
struct vector toPolar( struct vector c) {
  
  double Ln, Rn;
  
  h += c.x;
  l += c.y;
  Ln = sqrt(h*h+l*l);
  
  r = W-l;
  Rn = sqrt(h*h+r*r);
  
  struct vector p = { Ln-L, Rn-R }; 
  
}

void move(struct vector c) {
  
  double d = sqrt(c.x*c.x+c.y*c.y); // distance to move 
  double t = d/V;                   // time to move distance / velocity
  struct vector p = toPolar(c);     // polar contractions
  
  // convert mm to Steps
  p.x *= S; 
  p.y *= S; 
  
  double tl= 1000 * t / p.x; // time per step ms/step
  double tr= 1000 * t / p.y; // time per step ms/step
  
  while(p.x>0 && p.y>0){
    
    mc = micros();
    
    if(mc-mcl>=tl){
      stepper(1,0);
      mcl = micros();
      p.x--;
    }
    
    if(mc-mcr>=1000){
      stepper(1,4);
      mcr = micros();
      p.y--;
    }

  }
   
}

void loop() {

}

void doStepR(char d) {
    
    static char S = 0;
    char s = 0;
    
    switch(S){
       case 0:
         digitalWrite(p[0+s], LOW); 
         digitalWrite(p[1+s], LOW);
         digitalWrite(p[2+s], LOW);
         digitalWrite(p[3+s], HIGH);
       break; 
       case 1:
         digitalWrite(p[0+s], LOW); 
         digitalWrite(p[1+s], LOW);
         digitalWrite(p[2+s], HIGH);
         digitalWrite(p[3+s], HIGH);
       break; 
       case 2:
         digitalWrite(p[0+s], LOW); 
         digitalWrite(p[1+s], LOW);
         digitalWrite(p[2+s], HIGH);
         digitalWrite(p[3+s], LOW);
       break; 
       case 3:
         digitalWrite(p[0+s], LOW); 
         digitalWrite(p[1+s], HIGH);
         digitalWrite(p[2+s], HIGH);
         digitalWrite(p[3+s], LOW);
       break; 
       case 4:
         digitalWrite(p[0+s], LOW); 
         digitalWrite(p[1+s], HIGH);
         digitalWrite(p[2+s], LOW);
         digitalWrite(p[3+s], LOW);
       break; 
       case 5:
         digitalWrite(p[0+s], HIGH); 
         digitalWrite(p[1+s], HIGH);
         digitalWrite(p[2+s], LOW);
         digitalWrite(p[3+s], LOW);
       break; 
         case 6:
         digitalWrite(p[0+s], HIGH); 
         digitalWrite(p[1+s], LOW);
         digitalWrite(p[2+s], LOW);
         digitalWrite(p[3+s], LOW);
       break; 
       case 7:
         digitalWrite(p[0+s], HIGH); 
         digitalWrite(p[1+s], LOW);
         digitalWrite(p[2+s], LOW);
         digitalWrite(p[3+s], HIGH);
       break; 
       default:
         digitalWrite(p[0+s], LOW); 
         digitalWrite(p[1+s], LOW);
         digitalWrite(p[2+s], LOW);
         digitalWrite(p[3+s], LOW);
       break; 
    }
    SetDirection();
  }
}

void SetDirection(){
  if(Direction==1){ Steps++;}
  if(Direction==0){ Steps--; }
  if(Steps>7){Steps=0;}
  if(Steps<0){Steps=7; }
}

