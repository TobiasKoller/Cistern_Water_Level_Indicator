#include "DistanceSensor.h"
#include <Arduino.h>

int16_t _distanceEmpty = 0;
int16_t _distanceFull = 0;

constexpr int PIN_D2 = 2;
constexpr int PIN_D3 = 3;
constexpr int PIN_D4 = 4;

const int TRIGGER = PIN_D2;
const int ECHO = PIN_D3;

void DistanceSensor::Setup(int16_t distanceEmpty, int16_t distanceFull) {
  _distanceEmpty = distanceEmpty;
  _distanceFull = distanceFull;

  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
}

int DistanceSensor::CalculateDistanceInPercentage() {
  long distanceInCm = GetDistanceInCm();
  Serial.print("distanceInCm=");
  Serial.println(distanceInCm);

  Serial.print("percentage distance full=");
  Serial.println(_distanceFull);
  Serial.print("percentage distance empty=");
  Serial.println(_distanceEmpty);

  int value_range = _distanceEmpty - _distanceFull;
  Serial.print("value_range=");
  Serial.println(value_range);

  double percentage = 100 - (static_cast<double>(distanceInCm - _distanceFull) / value_range * 100);
  percentage = std::max(0.0, std::min(100.0, percentage));

  return static_cast<int>(percentage);
}

int DistanceSensor::CalculateDistanceinLiter() {
  return -1;
}

long DistanceSensor::GetDistanceInCm() {
  return GetAverageDistanceInCm();
}

long DistanceSensor::GetAverageDistanceInCm() {
  const int numMeasurements = 10;
  int measurements[numMeasurements];
  long totalDistance = 0;

  for (int i = 0; i < numMeasurements; i++) {
    measurements[i] = GetDistanceFromSensorInCm();
    totalDistance += measurements[i];
  }

  std::sort(measurements, measurements + numMeasurements);

  long sumWithoutExtremes = 0;
  for (int i = 2; i < numMeasurements - 2; i++) {
    sumWithoutExtremes += measurements[i];
  }

  long averageDistance = sumWithoutExtremes / (numMeasurements - 4);
  return averageDistance;
}

long DistanceSensor::GetDistanceFromSensorInCm() {
  long duration = 0;

  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);

  duration = pulseIn(ECHO, HIGH);
  long distanceInCm = (duration / 2) / 29.1;

  if (distanceInCm >= 200 || distanceInCm <= 0) {
    distanceInCm = 0;
  }

  return distanceInCm;
}
