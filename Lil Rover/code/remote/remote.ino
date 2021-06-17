/*This code was written by learnayg 2021*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7,8);
const byte address[6] = "00001";

const int cameraMountControl=A2;
const int driveControl=A4;
const int turnControl=A6;
const int lightButton=3;
const int buzzerButton=2;
String buzzerState;
void setup(){
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
  
  pinMode(cameraMountControl,INPUT);
  pinMode(driveControl,INPUT);
  pinMode(turnControl,INPUT);
  pinMode(lightButton,INPUT_PULLUP);
  pinMode(buzzerButton,INPUT_PULLUP);
}
void loop(){
  if(digitalRead(buzzerButton)==0){
    buzzerState="HIGH";
  }else{
    buzzerState="LOW";
  }
  int cameraMountControlReading=analogRead(cameraMountControl);
  int cameraMountPosition=(180./1023.)*cameraMountControlReading;

  int driveControlReading=analogRead(driveControl);
  int forward;
  int backward;
  if(driveControlReading<=511){         // DRIVE FORWARD
    forward=(-255./511.)*driveControlReading+255.;
    backward=0;
  }else if(driveControlReading>=513){   // DRIVE BACKWARD
    forward=0;
    backward=(255./513.)*driveControlReading-255.;
  }else{                                // STOP
    forward=0;
    backward=0;
  }
  
  int turnLeft;
  int turnRight;
  int turnControlReading=analogRead(turnControl);
  if(turnControlReading<=511){          // TURN RIGHT
    turnRight=(-255./511.)*turnControlReading+255.;
    turnLeft=0;
  }else if(turnControlReading>=513){    // TURN LEFT
    turnRight=0;
    turnLeft=(255./513.)*turnControlReading-255.;
  }else{                                // DON'T TURN
    turnLeft=0;
    turnRight=0;
  }
  
  // This ensures that the values each have three characters
  // this makes it easier to read on the receiver's side
  int stringLength;
  String forwardSpeed=String(forward);          // Convert the value to a string
  stringLength = int(forwardSpeed.length());    // Read the length of the string
  for(int i=stringLength; i<3; i++){            // If the length of the string is less than 
    forwardSpeed = String("0" + forwardSpeed);  // three characters add a zero in the front
  }
  String backwardSpeed=String(backward);
  stringLength = int(backwardSpeed.length());
  for(int i=stringLength; i<3; i++){
    backwardSpeed = String("0" + backwardSpeed);
  }
  String turnLeftSpeed=String(turnLeft);
  stringLength = int(turnLeftSpeed.length());
  for(int i=stringLength; i<3; i++){
    turnLeftSpeed = String("0" + turnLeftSpeed);
  }
  String turnRightSpeed=String(turnRight);
  stringLength = int(turnRightSpeed.length());
  for(int i=stringLength; i<3; i++){
    turnRightSpeed = String("0" + turnRightSpeed);
  }
  String cameraMountSend=String(cameraMountPosition);
  stringLength = int(cameraMountSend.length());
  for(int i=stringLength; i<3; i++){
    cameraMountSend = String("0" + cameraMountSend);
  }
  // Combine all of the values into one string
  String message=forwardSpeed+backwardSpeed+turnLeftSpeed+turnRightSpeed+cameraMountSend+buzzerState;
  int textLength=int(message.length())+1;
  char textSend[32]="";
  // Convert the string into a char
  message.toCharArray(textSend,textLength);
  radio.write(&textSend, sizeof(textSend));
}