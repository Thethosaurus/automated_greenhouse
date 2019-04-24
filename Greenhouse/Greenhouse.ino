#include <dht.h>

const int temperaturePin = 0;
int val = 0; //value for storing moisture value 
int soilPin = A0;//Declare a variable for the soil moisture sensor 
int soilPower = 1;//Variable for Soil moisture Power

dht DHT;
#define DHT11_PIN 7
  
void setup() {
  Serial.begin(9600);

  pinMode(soilPower, OUTPUT);//Set D7 as an OUTPUT
  digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor
}

void loop() {
  temp_check();
  delay(1000);
  soil_moisture();
//  delay(1000);
}

float getVoltage(int pin)
{
  // This function has one input parameter, the analog pin number
  // to read. You might notice that this function does not have
  // "void" in front of it; this is because it returns a floating-
  // point value, which is the true voltage on that pin (0 to 5V).

  // You can write your own functions that take in parameters
  // and return values. Here's how:

    // To take in parameters, put their type and name in the
    // parenthesis after the function name (see above). You can
    // have multiple parameters, separated with commas.

    // To return a value, put the type BEFORE the function name
    // (see "float", above), and use a return() statement in your code
    // to actually return the value (see below).

    // If you don't need to get any parameters, you can just put
    // "()" after the function name.

    // If you don't need to return a value, just write "void" before
    // the function name.

  // Here's the return statement for this function. We're doing
  // all the math we need to do within this statement:

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


