// +--------------------------------------------+
// |
// | Stero Cooler
// |
// | Written by Eric Kenny
// |
// | Thanks to:
// |  - http://www.instructables.com/id/Use-Arduino-with-TIP120-transistor-to-control-moto/
// |  - https://blog.adafruit.com/2012/11/30/tutorial-arduino-lesson-3-rgb-leds-arduino/
// |  - http://playground.arduino.cc/Code/Timer
// +--------------------------------------------+
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)

OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// RGB LED PIN DEFINITION
int redPin = 11;
int greenPin = 10;
int bluePin = 9;

int relayPin = 1;

// Temperature (F) level definition for RGB LED
int cool = 72;
int warm = 82;
int hot = 92;

int tempF = 0;
int TIP120pin = 8;

void setup() {
  // SET RGB PIN MODE
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT); 
  
  // SET TIP-120 PIN MODE
  pinMode(TIP120pin, OUTPUT);
  
  pinMode(relayPin, INPUT);

  // Start up the Dallas Temperature IC Control library
  sensors.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement

}

void loop() {
  
  // If relay is triggered from Stero's USB port, turn on...
  if (analogRead(relayPin))
  {
    
    sensors.requestTemperatures(); // Send the command to get temperatures
    tempF = sensors.getTempFByIndex(0); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
    
    // Set color of RGB LED based on reading from temperature sensor
    // While also setting the speed of the fan
    if (tempF <= cool)
    {
       setLEDgreen();
       setFanLow();
    } else if (tempF <= warm) {
       setLEDyellow();
       setFanLow();
    } else if (tempF <= hot) {
       setLEDorange();
       setFanMedium();
    } else if (tempF > hot) {
       setLEDred();
       setFanHigh();
    } 
  } else if (!analogRead(relayPin)) {
    // Run cool down procedure
    executeCoolDown();
  } else {
    // go into power save mode 
    executeStandBy();
  }
}

// ----------------------------------------------
// |      RGB SETCOLOR FUNCTION DEFINITION      |
// ----------------------------------------------
void setColor(int red, int green, int blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}

// ----------------------------------------------
// |        INDIVIDUAL COLOR FUNCTIONS          |
// ----------------------------------------------
void setLEDoff()
{
  setColor(0, 0, 0);
}
void setLEDblue()
{
  setColor(0, 0, 255);
}
void setLEDgreen()
{
  setColor(0, 255, 0);
}
void setLEDyellow()
{
  setColor(255, 255, 0);
}
void setLEDorange()
{
  setColor(0xFF, 0x8C, 0x0);
}
void setLEDred()
{
  setColor(255, 0, 0);
}
void setLEDwhite()
{
  setColor(255, 255, 255);
}

// ----------------------------------------------
// |           FAN SPEED FUNCTIONS              |
// ----------------------------------------------
void setFanOff()
{
  analogWrite(TIP120pin, 0);
}
void setFanLow()
{
  analogWrite(TIP120pin, 64);
}
void setFanMedium()
{
  analogWrite(TIP120pin, 128);
}
void setFanHigh()
{
  analogWrite(TIP120pin, 255);
}

void executeCoolDown()
{
  setLEDblue();
  // Spin fan at low for 5 minutes
  for (int i = 0; i < 300; i++)
  {
    setFanLow();
    delay(1000); 
  }
}

void executeStandBy()
{
  setLEDoff();
  setFanOff();
}
