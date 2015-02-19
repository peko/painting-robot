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
  L, Ln, // Left string length mm
  r, // x coordinate realtive right motor mm
  R, Rn, // Right string length mm
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

  W = 1175;
  L = 720;
  R = 720;
  V = 1.0; // mm/s

  calcGeometry();
  
  for(int i=0;i<8;i++)
    pinMode(p[i], OUTPUT); 
  
  delay(10000);

}

void calcGeometry() {
  double p = (W + L + R)/2.0;            // half perimeter
  double ir = sqrt((p-L)*(p-R)*(p-W)/p); // incircle
  h = 2.0 * p * ir / W;                 // height
  l = sqrt(L*L-h*h);                     // left projection
  r = W - l;                             // right projection
  Serial.println("Calc geomtry:");
  Serial.print("p: ");
  Serial.println(p);
  Serial.print("ir: ");
  Serial.println(ir);
  Serial.print("h: ");
  Serial.println(h);
  Serial.print("l: ");
  Serial.println(l);
  Serial.print("r: ");
  Serial.println(r);
  Serial.println("-------------");
  
  
}

// covert cartesian dx, dy to polar du,dv
struct vector toPolar( struct vector c) {
  
  h += c.x;
  l += c.y;
  Ln = sqrt(h*h+l*l);
  
  r = W-l;
  Rn = sqrt(h*h+r*r);
  
  struct vector p = { Ln-L, Rn-R };
  
  return p;
}

void move(struct vector c) {
  
  Serial.print("M:");
  Serial.print(c.x);
  Serial.print(",");
  Serial.println(c.y);
  
  unsigned long mc=0, mcl=0, mcr=0;
  
  double d = sqrt(c.x*c.x+c.y*c.y); // distance to move 
  double t = d/V;                   // time to move distance / velocity
  struct vector p = toPolar(c);     // polar contractions
  
  // convert mm to Steps
  p.x *= S; 
  p.y *= S; 

  Serial.print("Steps:");
  Serial.print(p.x);
  Serial.print(",");
  Serial.println(p.y);
    
  double stepsL = abs(p.x);
  double stepsR = abs(p.y);

  unsigned int tl= 1000.0 * t / stepsL; // time per step ms/step
  unsigned int tr= 1000.0 * t / stepsR; // time per step ms/step
  
  while(stepsL>0 || stepsR>0){
    
    mc = micros();
    
    if(stepsR>0 && mc-mcr>=tr){
      mcr = micros();
      
      if(p.y<0) doStepR(-1);
      else      doStepR(1);

      Serial.print("r: ");
      Serial.println(stepsR);
      stepsR--;
    }
    
    if(stepsL>0 && mc-mcl>=tl) {
      mcl = micros();
      
      if(p.x>0) doStepL(-1);
      else      doStepL(1);

      Serial.print("l: ");
      Serial.println(stepsL);
      
      stepsL--;
    }
    

    
  }
  delay(700);
  L = Ln; // new L length
  R = Rn; // new R length
   
}

void squre() {
  
  vector v;
  int side = 100;
  
  v = { side, 0 };
  move(v); 
  delay(1000);
  
  v = { 0, side };
  move(v); 
  delay(1000);
    
  v = { -side,0 };
  move(v);
 
  v = { 0, -side };
  move(v);
}

void circle(float R) {
  float steps= 8.0;
  move({R, 0});
  for (int i = 0; i<steps; i++) {
    move({R * sin(i*PI*2/steps), R * cos(i*PI*2/ steps)});
    delay(300);
  }
  move({-R,0});
}

void crossV() {
  int st = 1000;
  int sh = st>>1; 
  move({ sh,   0});
  move({-sh,   0});
  move({   0, sh});
  move({   0,-sh}); 
  move({ -sh,  0});
  move({  sh,  0});
  move({   0,-sh});
  move({   0, sh}); 
 
}


void crossD() {
  int st = 100; 
  for( int i=0;i<10; i++) move({ st, st});
  for( int i=0;i< 5; i++) move({-st,-st});
  for( int i=0;i< 5; i++) move({ st,-st});
  for( int i=0;i<10; i++) move({-st, st}); 
  for( int i=0;i< 5; i++) move({ st,-st});
  for( int i=0;i< 5; i++) move({-st,-st});
}

void loop() {
// circle(50+random(250));  
    crossV();
//  crossD();
}

void doStepL(char d) {
    
  static char S = 0;
  char s = 0;
  delay(1);
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
  
  S+=d;
  if(S>7) S=0;
  if(S<0) S=7;
}

void doStepR(char d) {
    
  static char S = 0;
  char s = 4;
  delay(5);
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
  
  S+=d;
  if(S>7) S=0;
  if(S<0) S=7;
}


