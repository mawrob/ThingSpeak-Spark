// This #include statement was automatically added by the Spark IDE.
#include "newThingSpeak.h"

newThingSpeakLibrary::newThingSpeak newThingSpeak ("PutYourThingSpeakKeyHere");

// Define the pins we're going to use
int led = D0;  // The led is connected here
int button = D1; // button is connected to D1
int led2 = D7; // This one is the built-in tiny one to the right of the USB jack
int analogPercent = 0; // variable to store the ThingSpeak analog value converted to percent
int analog = 0; // variable to store the analog value
unsigned long analogUpdate = 16000; // rate in ms to update ThingSpeak
unsigned long ioUpdate = 900;
unsigned long lastAnalogTime = 0; 
unsigned long lastioTime = 0;
int digitalIn = 0; // variable to store the read value for the button

// This routine runs only once upon reset
void setup() {
  Serial.begin(9600);
  // Initialize D0 + D7 pin as output
  // It's important you do this here, inside the setup() function rather than outside it or in the loop function.
  delay(2000);
  Serial.println('Starting up ...');
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);
  
  Spark.function("led", ledControl); // define a function to control the led through the Spark api
  
  Spark.variable("AI", &analog, INT); // define a function to read the analog input through the Spark api
  
  Spark.variable("DI", &digitalIn, INT); // define a function to read the digital in through the Spark api

  pinMode(button, INPUT_PULLDOWN); // define the button pin as an input
  
}

// This routine gets called repeatedly, like once every 5-15 milliseconds.
// Spark firmware interleaves background CPU activity associated with WiFi + Cloud activity with your code. 
// Make sure none of your code delays or blocks for too long (like more than 5 seconds), or weird things can happen.
void loop() {

      if(millis()-lastAnalogTime >analogUpdate) {
        updateAnalog(); // update ThingSpeak every analogUpdate ms
    } 
    
      if(millis()-lastioTime > ioUpdate) {
        digitalIn = digitalRead(button);       // read the input pin
        analog = analogRead(A0); // read the analog input and return the raw 12-bit value
        lastioTime = millis();
    }     

}

// function to read the analog value and write to ThingSpeak
void  updateAnalog() {
    
    Serial.println("Loop start");
    
    analogPercent =100*analogRead(A0)/4095;
    bool valSet = newThingSpeak.recordValue(1, String(analogPercent, DEC));
    if(valSet) {
        Serial.println("Value set to field 1 : " + String(analogPercent, DEC));
    } else {
        Serial.println("Value not set successfully");
    }
    
    bool valsSent = newThingSpeak.sendValues();
    if(valsSent) {
        Serial.println("Value successfully sent to thingspeak");
    } else {
        Serial.println("Sending to thingspeak failed");
    }
    lastAnalogTime = millis();
}


// function to control the led through the Spark.io api. Returns 1 if led is on, 0 if led if off, and -1 if the control message was undefined
int ledControl(String controlString) {
    
    Serial.println("Entered ledControl loop ");    
    
    Serial.println(controlString);

    if (controlString == "on")
    {
        digitalWrite(led, HIGH);   // Turn ON the LED pins
        digitalWrite(led2, HIGH);
        return 1;
    }
    else if (controlString == "off")
    {
        digitalWrite(led, LOW);    // Turn OFF the LED pins
        digitalWrite(led2, LOW);
        return 0; 
    }
    else
    {
        return -1;
    }

}