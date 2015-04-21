

#define STEPS_PER_REVOLUTION 64*16
#define L0 8
#define L1 9
#define L2 10
#define L3 11

char pins_left[]={L0,L1,L2,L3};

uint8_t stepper_states[36] = {
   LOW,  LOW,  LOW, HIGH, // 0
   LOW,  LOW, HIGH, HIGH, // 1
   LOW,  LOW, HIGH,  LOW, // 2
   LOW, HIGH, HIGH,  LOW, // 3
   LOW, HIGH,  LOW,  LOW, // 4
  HIGH, HIGH,  LOW,  LOW, // 5
  HIGH,  LOW,  LOW,  LOW, // 6
  HIGH,  LOW,  LOW, HIGH, // 7
   LOW,  LOW,  LOW,  LOW  // OFF STATE 
};


void setup(){
    for(int i=0; i<4; i++) {
      pinMode(pins_left[i],OUTPUT);
    }
}

void loop() {
  static int dir = 1;
  if (random(1000)== 1) {
    delay(200);
    dir*=-1;
  } 
  doStep(dir); 
  delayMicroseconds(1000); 
}

void doStep(char d) {
 
  static char step = 8; // OFF STEP
  for(int i=0; i<4; i++) {
    digitalWrite(pins_left[i],stepper_states[i+(step<<2)]);
  }
  step+=d;
  if(step>7) step=0;
  if(step<0) step=7;
}

