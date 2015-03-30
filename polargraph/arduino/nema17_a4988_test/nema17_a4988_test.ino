// Red:    A+
// Green:  A-
// Yellow: B+
// Blue:   B-

// RGYB ~ OK
// RGBY ~ OK bums
// YBGR ~ OK bums

// L L L Full Step 2 Phase
// H L L Half Step 1-2 Phase
// L H L Quarter Step W1-2 Phase
// H H L Eighth Step 2W1-2 Phase
// H H H Sixteenth Step 4W1-2 Phase

#define ENABLE 11
#define STEP 10
#define DIR  9

#define DELAY 1000
#define MD 1000

int x;
void setup() { 
  pinMode(ENABLE,OUTPUT); // Enable 
  pinMode(STEP,OUTPUT); // Step 
  pinMode(DIR,OUTPUT); // Dir 
  digitalWrite(ENABLE,LOW); // Set Enable low 
}

void loop() {
  digitalWrite(DIR,HIGH); // Set Dir high
  for(x = 0; x < MD; x++) {
    int d = 0; //abs(x-MD/2); 
    digitalWrite(STEP,HIGH+d); // Output high 
    delayMicroseconds(DELAY); // Wait 1/2 a ms 
    digitalWrite(STEP,LOW); // Output low 
    delayMicroseconds(DELAY+d);
  }
  delay(1000); // pause one second
  
  digitalWrite(DIR, LOW); // Set Dir low
  for(x = 0; x < MD; x++) {
    int d = 0; //abs(x-MD/2); 
    digitalWrite(STEP,HIGH); // Output high 
    delayMicroseconds(DELAY+d); // Wait 1/2 a ms 
    digitalWrite(STEP,LOW); // Output low 
    delayMicroseconds(DELAY+d); // Wait 1/2 a ms 
  } 
  delay(1000); // pause one second 
}
