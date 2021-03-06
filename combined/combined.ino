
//[1,2,3] channel 1 steering (left set)
//[1,2,3] channel 2 throttle (right set)
//[PxA,PxW,PxB] ==> [1,2,3] 

// MCP4142 --> ARDUINO
// CS(1) --> 10
// SI(3) --> 11
// SCK(2) --> 13
// VSS(4) --> GND
// VDD(14) --> +5V

#include <SPI.h>
// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 10;
const int WIPER_PIN = A0;        // Goes to X9C103P VW pin - Analog voltage output of pot
const float V_REF = 5.0;          // Change if using different Vref
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = A1; // analog pin connected to X output
const int Y_pin = A2; // analog pin connected to Y output
int x_val = 503;
int y_val = 62;
int y_val_old = 62;

void setup() {
  Serial.begin(9600);
  Serial.print ("Initial Voltage Setting: ");
  PrintVoltage();                             // Print X9C103P power up value

  // set the slaveSelectPin as an output:
  pinMode (slaveSelectPin, OUTPUT);
  // initialize SPI:
  SPI.begin();
 }

void loop() {

  Serial.print("Switch:  ");
  Serial.print(digitalRead(SW_pin));
  x_val = 128 - analogRead(X_pin) / 8;  //forward revere
//  y_val = 20;  //left right
  y_val = analogRead(Y_pin) / 8;  //left right

  Serial.print("\n");
  Serial.print("X-axis: ");
  Serial.print(x_val);
  Serial.print("\n");
  Serial.print("Y-axis: ");
  Serial.println(y_val);
  Serial.print("\n\n");
  
  int CommandByte = 16; // to Write to Pot 1
  digitalPotWrite(CommandByte, x_val);
  delay(10);
  
  CommandByte = 0; // to Write to Pot 0
  digitalPotWrite(CommandByte, y_val);
  y_val_old = y_val;
  delay(10);  
}

int digitalPotWrite(int CommandByte, int value) {
// take the SS pin low to select the chip:
  digitalWrite(slaveSelectPin,LOW);
  // send in the address and value via SPI:
  SPI.transfer(CommandByte);
  SPI.transfer(value);
  // take the SS pin high to de-select the chip:
  digitalWrite(slaveSelectPin,HIGH);
  PrintVoltage();
}

void PrintVoltage()
{
  return;
  int sampleADC = analogRead(WIPER_PIN);      // Take reading on wiper pin
  float volts = (sampleADC * V_REF) / 1023.0; // Convert to voltage
  Serial.println("   ADC = ");
  Serial.print(sampleADC);
  Serial.print("tVoltage = ");
  Serial.println(volts, 3);
}
