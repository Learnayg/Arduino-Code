/*This code was written by learnayg 2021*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7,8);
const byte address[6] = "00001";

#include <Servo.h>
Servo cameraMount;

const int forward=6;
const int backward=5;
const int turnLeft=10;
const int turnRight=9;
const int buzzer=2;
void setup(){
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();
  
  cameraMount.attach(3);
  pinMode(forward, OUTPUT);
  pinMode(backward, OUTPUT);
  pinMode(turnLeft, OUTPUT);
  pinMode(turnRight, OUTPUT);
}
void loop(){
  checkBattery();
  // Read the message sent from the remote
  char message[32] = "";
  radio.read(&message, sizeof(message));
  String command=String(message);
  
  // Seperate the values
  String forwardSpeed=String(command.charAt(0))+String(command.charAt(1))+String(command.charAt(2));
  String backwardSpeed=String(command.charAt(3))+String(command.charAt(4))+String(command.charAt(5));
  String turnLeftSpeed=String(command.charAt(6))+String(command.charAt(7))+String(command.charAt(8));
  String turnRightSpeed=String(command.charAt(9))+String(command.charAt(10))+String(command.charAt(11));
  String cameraMountPosition=String(command.charAt(12))+String(command.charAt(13))+String(command.charAt(14));
  String buzzerState=String(command.charAt(15))+String(command.charAt(16))+String(command.charAt(17))+String(command.charAt(18));
  
  cameraMount.write(cameraMountPosition.toInt());
  analogWrite(forward,forwardSpeed.toInt());
  analogWrite(backward,backwardSpeed.toInt());
  analogWrite(turnLeft,turnLeftSpeed.toInt());
  analogWrite(turnRight,turnRightSpeed.toInt());
  
  if(buzzerState=="HIGH"){
    digitalWrite(buzzer,HIGH);
  }else{
    digitalWrite(buzzer,LOW);
  }
  delay(25);
}
void checkBattery(){
  const int batteryLight=4;
  const int analogInput=A5;
  float voltageOut;
  float batteryVoltage;
  float resistor1=100000.0;  // Resistance of Resistor1 (100K)
  float resistor2=10000.0;   // Resistance of Resistor2 (10K)

  pinMode(batteryLight, OUTPUT);
  pinMode(analogInput,INPUT);

  int rawValue=analogRead(analogInput);
  voltageOut=(rawValue*5)/1024.0;
  batteryVoltage=voltageOut/(resistor2/(resistor1+resistor2));
  // If the battery voltage is to low turn the LED on
  if(batteryVoltage<9){
    digitalWrite(batteryLight,HIGH);
  }else{
    digitalWrite(batteryLight,LOW);
  }
}
