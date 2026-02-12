#include "Config.hpp"
#include <Arduino.h>

void setup() 
{
  pinMode(Config::LED_PIN, OUTPUT);
  Serial.begin(Config::UPLOAD_RATE);
  Serial.println("Hello, world! (Setup)");
}

void loop() 
{
  digitalWrite(Config::LED_PIN, HIGH);
  Serial.println("Set to HIGH!");
  delay(1000);
  digitalWrite(Config::LED_PIN, LOW);
  Serial.println("Set to LOW!");
  delay(1000);
}
