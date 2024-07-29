#include "DistanceSensor.h"
#include <Arduino.h>

int16_t _distanceEmpty = 0;
int16_t _distanceFull = 0;


#define PIN_D2 2;
#define PIN_D3 3;
#define PIN_D4 4;

const int TRIGGER = PIN_D2;
const int ECHO = PIN_D3;


void DistanceSensor::Setup(int16_t distanceEmpty, int16_t distanceFull)
{
  _distanceEmpty = distanceEmpty;
  _distanceFull = distanceFull;

  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
}

int DistanceSensor::CalculateDistanceInPercentage() {
    
    long distanceInCm = GetDistanceInCm();
    Serial.println("distanceInCm=");
    Serial.println(std::to_string(distanceInCm).c_str());


    Serial.println("percentage distance full");
    Serial.println(std::to_string(_distanceFull).c_str());
    Serial.println("percentage distance empty");
    Serial.println(std::to_string(_distanceEmpty).c_str());

    // Calculate the range of values
    int value_range = _distanceEmpty - _distanceFull;

    Serial.println("value_range");
    Serial.println(std::to_string(value_range).c_str());
    // Calculate the percentage
    double percentage = 100 - (static_cast<double>(distanceInCm - _distanceFull) / value_range * 100);

    // Ensure the percentage is within [0, 100]
    percentage = std::max(0.0, std::min(100.0, percentage));

    return static_cast<int>(percentage);
}

int DistanceSensor::CalculateDistanceinLiter()
{
  return -1;
}

long DistanceSensor::GetDistanceInCm()
{
  long distanceInCm = 0;
  long duration = 0;
  
  digitalWrite(TRIGGER, LOW);
  delay(5);
  digitalWrite(TRIGGER, HIGH);
  delay(10);
  digitalWrite(TRIGGER, LOW);

  duration = pulseIn(ECHO, HIGH);
  //check if distance is between 200cm and 0cm
  distanceInCm = (duration / 2) / 29.1;

  if (distanceInCm >= 200 || distanceInCm <= 0)
  {
    distanceInCm = 0;
  }
  
  return distanceInCm;
}

