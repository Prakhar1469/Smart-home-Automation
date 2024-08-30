#define BLYNK_TEMPLATE_NAME "LDR"
#define BLYNK_AUTH_TOKEN "YGLGPk9GR5t4ijOHnvr1-Ns_l2zadiEW"
#define BLYNK_TEMPLATE_ID "TMPL31cf_WSWx"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT11.h>

char ssid[] = "Sujal's Phone";
char pass[] = "mittal@2003";

DHT11 dht11(D4);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN,ssid,pass);
  pinMode(A0,INPUT);

}


void loop() {
  // put your main code here, to run repeatedly:
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
        // Print error message based on the error code.
        Serial.println(DHT11::getErrorString(result));
  }

  int ldrValue = analogRead(A0);

  int pwmValue =  map(ldrValue,1024,200,1023,0);

  analogWrite(D0,pwmValue);
  Blynk.virtualWrite(V0,pwmValue);

  Serial.println(ldrValue);
  delay(200);

  

}