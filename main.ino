int moisturePin = A0;         // analog input pin A0 for soil moisture sensor

#include <DHT.h>              // header file for humidity and temperature sensor
#define DHTPIN 3              // pin to which DHT11 data pin is connected
#define DHTTYPE DHT11         // DHT11 sensor model
DHT dht(DHTPIN, DHTTYPE);     // creating a DHT object

const int flowSensorPin = 1;  // pin for the flow sensor
volatile int flowSensorInterruptCount = 0; // Initialize the flow sensor interrupt count

int mqPin = A1;               // Analog input pin for MQ sensor
int ledPin = 12;              // On-board LED pin for mq sensor

// pins for ultrasonic sensors
const int trigPin = 10;
const int echoPin = 9;
// variables for duration and distance measurements using ultrasonic sensor
long duration;
int distance;

void setup()
{
  Serial.begin(9600);          // Initialize serial communication at 9600 baud

  dht.begin();                 //initialize DHT sensor

  pinMode(flowSensorPin, INPUT); // Set the flow sensor pin as input
  attachInterrupt(digitalPinToInterrupt(flowSensorPin), flowSensorInterrupt, RISING); // Set the interrupt for the flow sensor pin
  
  pinMode(ledPin, OUTPUT);     // Set LED pin as output of mq sensor

  // setup pins for ultrasonic sensors
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop()
{
  delay(2000);                // Wait for 2 seconds before reading data

  int moistureValue = analogRead(moisturePin); // Read soil  moisture sensor value
  // Print soil moisture value to serial monitor
  Serial.print("Soil Moisture: ");
  Serial.print(moistureValue);
  
  // reading temperature and humidity using DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  // Print temperature and humidity to the serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
  
  float flowRate = (flowSensorInterruptCount / 7.5); // Calculate the flow rate (7.5 is the number of pulses per liter for the flow sensor)
  // print flow rate to serial monitor 
  Serial.print("Flow rate: ");
  Serial.print(flowRate);
  Serial.println(" L/min");
  flowSensorInterruptCount = 0;     // Reset the flow sensor interrupt count
  
  int mqValue = analogRead(mqPin); // Read analog value from MQ sensor
  // Print mq sensor value to serial monitor
  Serial.print("MQ Sensor value: ");
  Serial.println(mqValue); 
  if (mqValue > 500)
  { 
    // If mq sensor value is above threshold
    digitalWrite(ledPin, HIGH);    // Turn on LED
  }
  else
    digitalWrite(ledPin, LOW);     // Turn off LED

  // send a 10 microsecond pulse to trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = duration * 0.034/2;        // measure duration of echo pulse using ultrasonic sensors
  // print distance measured by ultrasonic sensor to serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.println(); // Print new line  

  delay(10000);                        // Delay for a short time before reading data again
}

void flowSensorInterrupt() 
{
  flowSensorInterruptCount++;         // Increment the flow sensor interrupt count
}