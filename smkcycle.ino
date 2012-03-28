// OPTIONS
const boolean IS_SNES = true;
const long SNES_MINIMUM_SPEED = 200;
const long SNES_PEDAL_RELEASE_DELAY = 300;

// INPUTS
const int PEDAL_IN = 2;
const int START_IN = 8;
const int LEFT_IN = 10;
const int RIGHT_IN = 9;

// OUTPUTS (For SNES)
const int A_BUTTON = 3;
const int START_BUTTON = 4;
const int LEFT_BUTTON = 12;
const int RIGHT_BUTTON = 11;

// Pedal Variables
long lastPedal = 0;
long timeSinceLastPedal = 0;
int lastPedalIn = 0;
boolean hasPedalled = false;

// Button Variables
boolean aPressed = false;
boolean startPressed = false;
boolean rightPressed = false;
boolean leftPressed = false;

void setup() {
  // SNES Button Outs
  if (IS_SNES){
    pinMode(START_BUTTON, OUTPUT);
    pinMode(A_BUTTON, OUTPUT);
    pinMode(LEFT_BUTTON, OUTPUT);
    pinMode(RIGHT_BUTTON, OUTPUT);
  }
  
  // Button and Pedal inputs
  pinMode(START_IN, INPUT);
  pinMode(LEFT_IN, INPUT);
  pinMode(RIGHT_IN, INPUT);
  pinMode(PEDAL_IN, INPUT);
  
  // Using a digital pin as +5V
  pinMode(13, OUTPUT);
  digitalWrite(13,HIGH);
  
  Serial.begin(9600);
}

void loop() {
  checkRight();
  checkLeft();
  checkStart();
  checkPedal();
}

void sPrint(String message){
 if (IS_SNES){
   String out = "";
   out += "|";
   out += message;
   out += "|";
   Serial.println(out);
 }else{
   Serial.println(message);
 }
}

void checkRight(){
  // I used inverted buttons by accident
  // easier to fix in software for now
  if (digitalRead(RIGHT_IN) == HIGH){
    if (rightPressed){
      sPrint(String('right:0'));
      rightPressed = false;
      if (IS_SNES){
        digitalWrite(RIGHT_BUTTON, LOW);
      }
    }
  }else{
    if (!rightPressed){
      sPrint(String('right:1'));
      rightPressed = true;
      if (IS_SNES){
        digitalWrite(RIGHT_BUTTON, HIGH);
      }
    }
  }
}

void checkLeft(){
  // I used inverted buttons by accident
  // easier to fix in software for now
  if (digitalRead(LEFT_IN) == HIGH){
    if (leftPressed){
      sPrint(String('left:0'));
      leftPressed = false;
      if (IS_SNES){
        digitalWrite(LEFT_BUTTON, LOW);
      }
    }
  }else{
    if (!leftPressed){
      sPrint(String('left:1'));
      leftPressed = true;
      if (IS_SNES){
        digitalWrite(LEFT_BUTTON, HIGH);  
      }
    }
  }
}

void checkStart(){
  // I used inverted buttons by accident
  // easier to fix in software for now
  if (digitalRead(START_IN) == HIGH){
    if (startPressed){
      sPrint(String('start:0'));
      startPressed = false;
      if (IS_SNES){
        digitalWrite(START_BUTTON, LOW);
      }
    }
  }else{
    if (!startPressed){
      sPrint(String('start:1'));
      startPressed = true;
      if (IS_SNES){
        digitalWrite(START_BUTTON, HIGH);  
      }
    }
  }
}

void checkPedal(){
  // check if a pedal state has changed
  if (digitalRead(PEDAL_IN) != lastPedalIn){
    lastPedalIn = digitalRead(PEDAL_IN);
    pedalStateChange();
  }
  
  // if a certain amount of time has passed we can assume
  // that the rider has stopped peddeling.
  if (hasPedalled){
    if (millis() - lastPedal > SNES_PEDAL_RELEASE_DELAY){
      releaseA();
    }
  }
}

void pedalStateChange(){
  hasPedalled = true;
  timeSinceLastPedal = millis() - lastPedal;
  lastPedal = millis();
  sPrint(String('speed:') += String(timeSinceLastPedal));
  
  if (IS_SNES){
    if (timeSinceLastPedal < MINIMUM_SPEED){
      pressA();
    }else{
      releaseA();
    }
  }
}

void pressA(){
  if (!aPressed){
    Serial.println("Press A");
  }
  aPressed = true;
  digitalWrite(A_BUTTON,HIGH);
  
}

void releaseA(){
  if (aPressed){
    Serial.println("Release A");
  }
  aPressed = false;
  digitalWrite(A_BUTTON,LOW);
}