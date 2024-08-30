#define BLYNK_TEMPLATE_NAME "LDR"
#define BLYNK_AUTH_TOKEN "YGLGPk9GR5t4ijOHnvr1-Ns_l2zadiEW"
#define BLYNK_TEMPLATE_ID "TMPL31cf_WSWx"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT11.h>
#include <LiquidCrystal_I2C.h>

char ssid[] = "Sujal's Phone";
char pass[] = "mittal@2003";

DHT11 dht11(D4);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the LCD with I2C address 0x27, 16 columns, and 2 rows

const int pirPin = D5; // PIR sensor connected to D5
const int motorPin = D6; // Motor connected to D6
const int ledPin = D7; // LED connected to D7

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  pinMode(A0, INPUT); // LDR
  pinMode(ledPin, OUTPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(pirPin, INPUT);
  lcd.begin(); // Initialize the LCD
  lcd.backlight(); // Turn on the LCD backlight
}

void loop() {
  Blynk.run();

  int temperature = 0;
  int humidity = 0;
  int result = dht11.readTemperatureHumidity(temperature, humidity);
  if (result == 0) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C\tHumidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  } else {
    Serial.println(DHT11::getErrorString(result));
  }

  int ldrValue = analogRead(A0);
  int pwmValue = map(ldrValue, 1024, 200, 1023, 0);
  analogWrite(D0, pwmValue); // Adjust PWM for LDR
  Blynk.virtualWrite(V0, pwmValue);
  Serial.println(ldrValue);

  // Check for darkness and control LED
  if (ldrValue < 500) { // Darkness threshold
    digitalWrite(ledPin, HIGH); // Turn on LED
  } else {
    digitalWrite(ledPin, LOW); // Turn off LED
  }

  // Control motor based on temperature
  if (temperature > 20) {
    digitalWrite(motorPin, HIGH); // Turn on motor
  } else {
    digitalWrite(motorPin, LOW); // Turn off motor
  }

  // Check PIR sensor for presence detection
  int pirState = digitalRead(pirPin);
  if (pirState == HIGH) {
    digitalWrite(ledPin, HIGH); // Turn on LED if presence detected
    digitalWrite(motorPin, HIGH); // Turn on motor if presence detected
    lcd.clear();
    lcd.print("Person detected");
    Blynk.virtualWrite(V1, "Person detected");
  } else {
    lcd.clear();
    lcd.print("No person");
    Blynk.virtualWrite(V1, "No person");
  }

  delay(200); // Adjust delay as needed
}
