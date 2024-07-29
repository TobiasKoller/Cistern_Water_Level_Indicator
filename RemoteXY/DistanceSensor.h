#include <string>


class DistanceSensor{
  public:
    void Setup(int16_t minDistance, int16_t maxDistance);
    int CalculateDistanceInPercentage();
    int CalculateDistanceinLiter();
    long GetDistanceInCm();

  private:
};