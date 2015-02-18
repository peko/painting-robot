#define LEFT 500
unsigned char p[8] = {2,3,4,5, 13,12,11,10};

int Steps = 0;

boolean Direction = true;// gre
unsigned long last_time;
unsigned long currentMillis ;
int steps_left=LEFT;
long time;

void setup(){
  Serial.begin(115200);
  for(int i=0;i<8;i++)
    pinMode(p[i], OUTPUT); 
  
  delay(1000);

}

//rotate/flip a quadrant appropriately
void rot(int n, int *x, int *y, int rx, int ry) {
    if (ry == 0) {
        if (rx == 1) {
            *x = n-1 - *x;
            *y = n-1 - *y;
        }
 
        //Swap x and y
        int t  = *x;
        *x = *y;
        *y = t;
    }
}

//convert (x,y) to d
int xy2d (int n, int x, int y) {
    int rx, ry, s, d=0;
    for (s=n/2; s>0; s/=2) {
        rx = (x & s) > 0;
        ry = (y & s) > 0;
        d += s * s * ((3 * rx) ^ ry);
        rot(s, &x, &y, rx, ry);
    }
    return d;
}
 
//convert d to (x,y)
void d2xy(int n, int d, int *x, int *y) {
    int rx, ry, s, t=d;
    *x = *y = 0;
    for (s=1; s<n; s*=2) {
        rx = 1 & (t/2);
        ry = 1 & (t ^ rx);
        rot(s, x, y, rx, ry);
        *x += s * rx;
        *y += s * ry;
        t /= 4;
    }
}

void hilbert() {
  
  int cell_size = 15;
  int n = 0;
  int s = 4096;
  int ox = 0;
  int oy = 0;
  int x = 0;
  int y = 0;
  
  while(true) {
   
    d2xy(s, n, &x, &y);
    int dx = x - ox;
    int dy = y - oy; 
    int steps;
   
    if(!(n%64)) cell_size=10+random(5)*5;
    
    Serial.println(dx,DEC);
    Serial.println(dy,DEC);
   
    Direction = dx>0;
    steps = abs(dx)*cell_size;
    while(steps>0){
      currentMillis = micros();
      if(currentMillis-last_time>=1000){
        stepper(1,0);
        time=time+micros()-last_time;
        last_time=micros();
        steps--;
      }
    }
    
    Direction = dy>0;
    steps = abs(dy)*cell_size;
    while(steps>0){
      currentMillis = micros();
      if(currentMillis-last_time>=1000){
        stepper(1,4);
        time=time+micros()-last_time;
        last_time=micros();
        steps--;
      }
    }
    
    ox = x;
    oy = y;
    n++;
  }
}

void vibrating_hilbert() {
  
  int cell_size = 25;
  int n = 0;
  int s = 2048;
  int ox = 0;
  int oy = 0;
  int x = 0;
  int y = 0;
  
  while(n<s) {
   
    d2xy(s, n, &x, &y);
    int dx = x - ox;
    int dy = y - oy; 
    int steps;
   
//    cell_size = 25+25*(sin(x*PI/16.0)+cos(y*PI/16.0));

    cell_size = 25+x*y/20;

    Direction = dx>0;
    steps = abs(dx)*cell_size;
    while(steps>0){
      currentMillis = micros();
      if(currentMillis-last_time>=1000){
        stepper(1,0);
        time=time+micros()-last_time;
        last_time=micros();
        steps--;
      }
    }
    
    Direction = dy>0;
    steps = abs(dy)*cell_size;
    while(steps>0){
      currentMillis = micros();
      if(currentMillis-last_time>=1000){
        stepper(1,4);
        time=time+micros()-last_time;
        last_time=micros();
        steps--;
      }
    }
    
    ox = x;
    oy = y;
    n++;
  }
}

void spiral() {
  
  int R = 10;
  int dr = 10;
  int steps =0;
  while(R<2000) {

    steps = R;
    while(steps>0){
      currentMillis = micros();
      if(currentMillis-last_time>=1000){
        stepper(1,0);
        time=time+micros()-last_time;
        last_time=micros();
        steps--;
      }
    }
    R+=dr;
    steps = R;
    while(steps>0){
      currentMillis = micros();
      if(currentMillis-last_time>=1000){
        stepper(1,4);
        time=time+micros()-last_time;
        last_time=micros();
        steps--;
      }
    }
    R+=dr;
    Direction = !Direction;
    dr+=1;
  }
}

void tricky_spirals() {
  
  int R = 10;
  int dr = 10;
  int steps =0;
  while(R<500) {

    steps = R;
    while(steps>0){
      currentMillis = micros();
      if(currentMillis-last_time>=1000){
        stepper(1,0);
        time=time+micros()-last_time;
        last_time=micros();
        steps--;
      }
      if(random(5000)==100) Direction = !Direction;
    }
    R+=dr;
    steps = R;
    while(steps>0){
      currentMillis = micros();
      if(currentMillis-last_time>=1000){
        stepper(1,4);
        time=time+micros()-last_time;
        last_time=micros();
        steps--;
      }
      if(random(5000)==100) Direction = !Direction;
    }
    R+=dr;
    Direction = !Direction;
    dr+=1;
  }
}

//void stepper(int xw, int s);
void loop() {
   hilbert();
// vibrating_hilbert(); 
//   spiral();
//     tricky_spirals();
}

void stepper(int xw, int s){
  for (int x=0;x<xw;x++){
    delay(10);
    switch(Steps){
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

