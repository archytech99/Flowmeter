#include "Flowmeter.h"

volatile byte pulseCount;

#if defined(__AVR__)
  void pulseCounter(void) { pulseCount++; }
#elif defined(ESP32)
  void IRAM_ATTR pulseCounter(void) { pulseCount++; }
#else
  void pulseCounter(void) { pulseCount++; }
#endif

Flowmeter::Flowmeter(byte _sensor)
{
  sensorPin = _sensor;
  sensorInterrupt = digitalPinToInterrupt(sensorPin);
  oldTime = 0;
}

/*
  Set Debug Measurement Flowmeter
 */
void Flowmeter::debug(bool _set = false, int _min = 9, int _max = 49)
{
  if (_set)
  {
    pulseCount = random(_min, _max);
  }
}

/*
  Initial Setup Flowmeter
 */
void Flowmeter::init(int _uplit, double _tolit)
{
  pinMode(sensorPin, INPUT_PULLUP);
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  set(_tolit, _uplit);
}

/*
  The measurement of discharge water - Liter per Second
 */
void Flowmeter::loop(void)
{
  if ((millis() - oldTime) > 1000L)
  {
    detachInterrupt(sensorInterrupt);

    float _flowrate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    float _liter = _flowrate / 60;
    oldTime = millis();
    pulseCount = 0;
    liters = (tolerance > 0.00f) ? (_liter - (_liter * tolerance)) : _liter;
    totalLiters += liters;
    sumTotalLiter(totalLiters);

    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  }
}

/*
  Change value of Calibration Factor and Tolerance
 */
void Flowmeter::reset(void)
{
  upLiters = 0;
  totalLiters = 0.0;
}

/*
  Change value of Calibration Factor and Tolerance
 */
void Flowmeter::set(float _cal, float _tol, long maxlim)
{
  calibrationFactor = _cal;
  tolerance = _tol / 100;
  max_limit = maxlim;
}

/*
  Change value of Calibration Factor and Tolerance
 */
void Flowmeter::set(double _liter, int _upliter)
{
  upLiters = _upliter;
  totalLiters = _liter;
}
