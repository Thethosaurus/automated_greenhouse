#include <dht.h>

const int sensorPin = A1;
const int ledPin = 9;
const int temperaturePin = 0;
int val = 0; //value for storing moisture value 
const int soilPin = A0;//Declare a variable for the soil moisture sensor 
int soilPower = 1;//Variable for Soil moisture Power

// We'll also set up some global variables for the light level:
int lightLevel;
int calibratedlightLevel; // used to store the scaled / calibrated lightLevel
int maxThreshold = 0;     // used for setting the "max" light level
int minThreshold = 1023;   // used for setting the "min" light level

dht DHT;
#define DHT11_PIN 7
  
void setup() {
  Serial.begin(9600);

  pinMode(sensorPin, OUTPUT);
  pinMode(ledPin, OUTPUT);    // Set up the LED pin to be an output.
  pinMode(soilPower, OUTPUT);//Set D7 as an OUTPUT
  digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor
}

void loop() {
  
  temp_check();
  delay(1000);
  soil_moisture();
  delay(1000);
  photoResistor();
  delay(1000);

}

float getVoltage(int pin)
{
  return (analogRead(pin) * 0.004882814);

  // This equation converts the 0 to 1023 value that analogRead()
  // returns, into a 0.0 to 5.0 value that is the true voltage
  // being read at that pin.
}

//This is a function used to get the soil moisture content
int readSoil(){
    digitalWrite(soilPower, HIGH);//turn D7 "On"
    delay(1000);//wait 10 milliseconds 
    val = analogRead(soilPin);//Read the SIG value form sensor 
    digitalWrite(soilPower, LOW);//turn D7 "Off"
    return val;//send current moisture value
}

void temp_check(){
  float celsius, degreesF;
  int chk = DHT.read11(DHT11_PIN);
  celsius = DHT.temperature;
  degreesF = celsius * (9.0/5.0) + 32.0;
  Serial.print("Temperature (F) = ");
  Serial.print(degreesF);
  Serial.print("            Relative Humidity (%) = ");
  Serial.print(DHT.humidity);
}

void soil_moisture(){
  Serial.print("           Soil Moisture = ");    
  //get soil moisture value from the function below and print it
  Serial.println(readSoil());

  //This 1 second timefrme is used so you can test the sensor and see it change in real-time.
  //For in-plant applications, you will want to take readings much less frequently.
//  delay(1000);//take a reading every second
}

void photoResistor(){
  lightLevel = analogRead(sensorPin);  // reads the voltage on the sensorPin
  Serial.println(lightLevel);
  delay(1000);
  //autoRange();  // autoRanges the min / max values you see in your room.

  calibratedlightLevel = map(lightLevel, 0, 1023, 0, 255);  // scale the lightLevel from 0 - 1023 range to 0 - 255 range.
                                                  // the map() function applies a linear scale / offset.
//  Serial.print(calibratedlightLevel);                                                // map(inputValue, fromMin, fromMax, toMin, toMax);
  Serial.print("\t");       // tab character
  Serial.print(calibratedlightLevel);   // println prints an CRLF at the end (creates a new line after)

  analogWrite(ledPin, calibratedlightLevel);    // set the led level based on the input lightLevel.
  if(lightLevel < 10)
  {
    digitalWrite(ledPin, LOW);
  }
  else
  {
    digitalWrite(ledPin, HIGH);
  }
}

void autoRange(){
  if (lightLevel < minThreshold)  // minThreshold was initialized to 1023 -- so, if it's less, reset the threshold level.
    minThreshold = lightLevel;

  if (lightLevel > maxThreshold)  // maxThreshold was initialized to 0 -- so, if it's bigger, reset the threshold level.
    maxThreshold = lightLevel;

  // Once we have the highest and lowest values, we can stick them
  // directly into the map() function.
  // 
  // This function must run a few times to get a good range of bright and dark values in order to work.

  lightLevel = map(lightLevel, minThreshold, maxThreshold, 0, 255);
  lightLevel = constrain(lightLevel, 0, 255);
