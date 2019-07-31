//#define BLYNK_PRINT Serial
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <DHT.h>

//login to Blynk
char auth[] = "a9c9c82c3d954853a6f18b318cca77b4"; 

//Wi-Fi
char ssid[] = "iPhone"; 
char pass[] = "qwertyuiop";  

#define DHTPIN 2          
#define DHTTYPE DHT11 

//PIR
int ledPin = D6;
int pirPin = D7;
int state = LOW;

//HC-SR04
const int trigPin = D3; 
const int echoPin = D2; 
long duration;
int distance;

DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);  //V5 is for Humidity
  Blynk.virtualWrite(V6, t);  //V6 is for Temperature
}

void sendPIRdata(){
    if (digitalRead(pirPin) == HIGH) {
              
    digitalWrite(ledPin, HIGH);
    if (state == LOW) {
      //Serial.println("Motion Detected");
      //terminal.println("Motion Detected");
      state = HIGH;
      Blynk.virtualWrite(V9, 1);
    }
    
  } else {  
    digitalWrite(ledPin, LOW);
    if (state == HIGH){
      state = LOW;
      Blynk.virtualWrite(V9, 0);
    }
  }
}

void sendHCdata(){
 // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

// Calculating the distance
  distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
  Blynk.virtualWrite(V10, distance);
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(2000);
}

void setup()
{
  Serial.begin(115200); // See the connection status in Serial Monitor
  Blynk.begin(auth, ssid, pass);

 //PIR
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);

  //HC-SR04
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
 
  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
  timer.setInterval(1000L, sendPIRdata);
  timer.setInterval(1000L, sendHCdata);
}

void loop()
{
  Blynk.run(); // Initiates Blynk
  timer.run(); // Initiates SimpleTimer
}
