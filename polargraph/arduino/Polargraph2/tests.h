#ifndef TESTS
#define TESTS
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
  float steps= 36.0;
  for (int i = 0; i<steps; i++) {
    move({R * cos(i*PI*2/steps), R * sin(i*PI*2/ steps)});
//    delay(300);
  }
}

void wigly(float R) {
  float steps= 36.0;
  
  struct vector v = {R,0};
  
  float alpha = 10.0/180.0*PI;
  double cp = cos(alpha);
  double sp = sin(alpha);
  double cm = cos(-alpha);
  double sm = sin(-alpha);
  
  while (1) { 
    
    R=0.1+random(100)/100.0;
    float c = random(36);
    for (int i = 0; i<c; i++) {
      norm(&v, R);
      v.x= v.x*cp-v.y*sp;
      v.y= v.x*sp+v.y*cp;
      move(v);
    }
    c = 18+random(36);
    R=0.1+random(100)/100.0;
    for (int i = 0; i<c; i++) {
      norm(&v, R);
      v.x = v.x*cm-v.y*sm;
      v.y = v.x*sm+v.y*cm;
      move(v);
    }  
  }
}



void crossV() {
  int st = 100;
  int sh = st>>1; 
  move({ sh,   0}); delay(1000);
  move({-sh,   0}); delay(1000);
  move({   0, sh}); delay(1000);
  move({   0,-sh}); delay(1000);
  move({ -sh,  0}); delay(1000);
  move({  sh,  0}); delay(1000);
  move({   0,-sh}); delay(1000);
  move({   0, sh}); delay(1000);
  delay(5000);
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

void Test() {
  
  // extend left
  for (int i=0;i<STEPS_PER_REVOLUTION;i++) {
    doStepL(1);
    delayMicroseconds(MINIMUM_DELAY_BETWEEN_STEPS);
  }
  delay(1000);
  
  // contract left
  for (int i=0;i<STEPS_PER_REVOLUTION;i++) {
    doStepL(-1);
    delayMicroseconds(MINIMUM_DELAY_BETWEEN_STEPS);
  }
  delay(1000);
  
  // extend right
  for (int i=0;i<STEPS_PER_REVOLUTION;i++) {
    doStepR(1);
    delayMicroseconds(MINIMUM_DELAY_BETWEEN_STEPS);
  }
  delay(1000);
  
  // contract right
  for (int i=0;i<STEPS_PER_REVOLUTION;i++) {
    doStepR(-1);
    delayMicroseconds(MINIMUM_DELAY_BETWEEN_STEPS);
  }
  delay(1000);
  
///  delay(5000);
  
}

void spiral() {
  float i = 1;
  float di = 0.5;
  while(i<1000) {  
    move({ i,   0}); i+=di;
    move({ 0,   i}); i+=di;
    move({-i,   0}); i+=di;
    move({ 0,  -i}); i+=di;
  }
}

void spiral_exp() {
  float i = 1;
  float di = 0.1;
  float s = random(20)+10;
  int d = 200;
  while(i<s) {  
    move({ i,   0});delay(d); i+=di;
    move({ 0,   i});delay(d); i+=di;
    move({-i,   0});delay(d); i+=di;
    move({ 0,  -i});delay(d); i+=di;
    di+=0.1;
  }
}

#endif