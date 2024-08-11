#include <string>


class DistanceSensor{
  public:
    void Setup(int16_t distanceEmpty, int16_t distanceFull);
    int CalculateDistanceInPercentage();
    int CalculateDistanceinLiter();
    long GetDistanceInCm();

  private:
    long GetAverageDistanceInCm();
    long GetDistanceFromSensorInCm();
};