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
  return GetAverageDistanceInCm();
}

long DistanceSensor::GetAverageDistanceInCm() {
  const int numMeasurements = 10;
  int measurements[numMeasurements];
  long totalDistance = 0;

  for (int i = 0; i < numMeasurements; i++) {
    measurements[i] = GetDistanceFromSensorInCm(); // Assuming GetDistanceFromSensorInCm() returns the measured value
    totalDistance += measurements[i];
  }

  // Sort the measurements in ascending order
  for (int i = 0; i < numMeasurements - 1; i++) {
    for (int j = i + 1; j < numMeasurements; j++) {
      if (measurements[j] < measurements[i]) {
        int temp = measurements[i];
        measurements[i] = measurements[j];
        measurements[j] = temp;
      }
    }
  }

  // Ignore the two smallest and two largest values
  long sumWithoutExtremes = 0;
  for (int i = 2; i < numMeasurements - 2; i++) {
    sumWithoutExtremes += measurements[i];
  }

  // Calculate the average of the remaining values
  long averageDistance = sumWithoutExtremes / (numMeasurements - 4);
  return averageDistance;
}

long DistanceSensor::GetDistanceFromSensorInCm()
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

