#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 2); // RX, TX

const int led=13;

String command = ""; // Stores response of the HC-06 Bluetooth device
char buf[10];
float x=1.2;

// Pins used for inputs and outputs*******************************************************
const int analogInPin0 = A0;// Analog input pins
const int analogInPin1 = A1;
const int analogInPin2 = A2;
const int analogInPin3 = A3;
 
//Arrays for the 4 inputs*********************************************
float sensorValue[4] = {0,0,0,0};
float voltageValue[4] = {0,0,0,0};


void setup() {
  pinMode(led,OUTPUT);
  Serial.begin(9600);
  Serial.println("Type AT commands!");
  
  // The HC-06 defaults to 9600 according to the datasheet.
  mySerial.begin(9600);
}

void loop() {

  readSensors();
  getVoltageValue();
  //sendAndroidValues();
  
  // Read device output if available.
  if (mySerial.available()) {
    while(mySerial.available()) { // While there is more to be read, keep reading.
      command += (char)mySerial.read();
    }
    
    Serial.println(command);
    
    if(command=="1"){
      digitalWrite(led,HIGH);
    }
    else if(command=="2"){
      digitalWrite(led,LOW);
    }
    
    
    command = ""; // No repeats
  }
  
  // Read user input if available.
  if (Serial.available()){
    delay(10); // The delay is necessary to get this working!

    //sprintf(buf, "%f", x);


    
    dtostrf(x, 0, 1, buf);
    
    /***dtostrf(myFloatData, WIDTH, PRECISION, STORAGE_BUFFER);
     
     ***WIDTH is the number of characters to use in the output. For a value in the range 0.000 to 5.000, the width should be the precision +2. More is OK.

     ***PRECISION is the number of characters after the decimal point. For temperature, 1 would be sufficient.

     ***BUFFER is where the write the characters to.*/

    Serial.println(buf);
    Serial.println(strlen(buf));
    
    mySerial.write(buf);
    //sendAndroidValues();
    
  }

   delay(2000);

}



void readSensors()
{
  // read the analog in value to the sensor array
  sensorValue[0] = analogRead(analogInPin0);
  sensorValue[1] = analogRead(analogInPin1);
  sensorValue[2] = analogRead(analogInPin2);
  sensorValue[3] = analogRead(analogInPin3);
}



void getVoltageValue()
{
  for (int x = 0; x < 4; x++)
  {
    voltageValue[x] = ((sensorValue[x]/1023)*5);
  }
}

//sends the values from the sensor over serial to BT module
void sendAndroidValues()
 {
  //puts # before the values so our app knows what to do with the data
  mySerial.write('#');
  Serial.print('#');
  //for loop cycles through 4 sensors and sends values via serial
  for(int k=0; k<4; k++)
  {
    Serial.print(voltageValue[k]);
    Serial.print('+');

    mySerial.write(voltageValue[k]);
    mySerial.write('+');

    //technically not needed but I prefer to break up data values
    //so they are easier to see when debugging
  }
 Serial.print('~'); //used as an end of transmission character - used in app for string length
 Serial.println();
 delay(10);        //added a delay to eliminate missed transmissions
}
 
