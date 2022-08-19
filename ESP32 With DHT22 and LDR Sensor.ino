//aridoutofrange -- Ramadhirra Azzahra Putri

//Libraries
#include "DHTesp.h"
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

//OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, & Wire, -1);

//DHT
#define dhtPin 13
DHTesp dht;

//RGB LED Pins
#define redPin 19
#define greenPin 18
#define bluePin 15

//LDR 
#define ldrPin A0
int adcValue = 0;
const float GAMMA = 0.7;
const float resLoad10 = 50;

// Buzzer
#define buzzerPin 4

void setup() {

  // Declaration for an SSD1306 display connected to I2C 
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }
  dht.setup(dhtPin, DHTesp::DHT22);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(buzzerPin, OUTPUT);
  pinMode(ldrPin, INPUT);
}

void loop() {
  //Object Declaration
  TempAndHumidity data = dht.getTempAndHumidity();
  float temp = data.temperature; //Get temp data
  float humid = data.humidity; //Get humidity data

  if (temp < 30.00) {
    setColor(HIGH, LOW, HIGH); //Set LED color to green
    noTone(buzzerPin);
    Serial.println("Normal Temperature");
    Serial.println("__________________");

    //Display data to OLED screen
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 40);
    display.println("Normal Temperature");
    display.display();
    delay(1000);
  } else if (temp > 30.00 && temp < 50.00) {
    setColor(LOW, LOW, HIGH); //Set LED color to yellow
    noTone(buzzerPin);
    Serial.println("Okay Temperature");
    Serial.println("__________________");

    //Display data to OLED screen
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 40);
    display.println("Okay Temperature");
    display.display();
    delay(1000);
  } else if (temp > 50.00) {
    setColor(LOW, HIGH, HIGH); //Set LED color to red
    tone(buzzerPin, 500);
    Serial.println("Dangerous Temperature");
    Serial.println("__________________");

    //Display data to OLED screen
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 40);
    display.println("Dangerous Temperature");
    display.display();
    delay(1000);
  }

  //Convert analog LDR data to lux
  adcValue = analogRead(ldrPin);
  float voltage = adcValue / 4095. * 5;
  float resistance = 2000 * voltage / (1 - voltage / 5);
  float lux = pow(resLoad10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));

  //Print to serial monitor
  Serial.println("Temperature: " + String(temp, 2) + " °C");
  Serial.println("Humidity: " + String(humid, 1) + " %");
  Serial.println("Lux level: " + String(lux, 2) + " lux");

  //Display data to OLED screen
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);

  display.println("Temp: " + String(temp, 2) + " " + ((char) 247) + "C");
  display.display();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 20);
  display.println("Humidity: " + String(humid, 1) + " %");
  display.display();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 30);
  display.println("Lux: " + String(lux, 1) + " lux");
  display.display();

  delay(1000);
}

//Method to change RGB LED's color
void setColor(int red, int green, int blue) {
  //RGB colors are represented in 0 to 255, so in digital, 255 will be high and 0 will be low
  digitalWrite(redPin, red);
  digitalWrite(greenPin, green);
  digitalWrite(bluePin, blue);
}
