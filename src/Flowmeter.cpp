#include "Flowmeter.h"

volatile byte pulseCount;

void pulseCounter(void) { pulseCount++; }

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
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  set(_tolit, _uplit);
  oldTime = millis();
}

/*
  The measurement of discharge water - Liter per Second
 */
void Flowmeter::loop(void)
{
  if ((millis() - oldTime) > 999)
  {
    detachInterrupt(sensorInterrupt);

    float _flowrate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    float _liter = _flowrate / 60;
    oldTime = millis();
    liters = (tolerance > 0.000f) ? (_liter - ((_liter * tolerance) / 60)) : _liter;
    totalLiters += liters;
    pulseCount = 0;
    sumTotalLiter(totalLiters);

    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  }
}

/*
  Change value of Calibration Factor and Tolerance
 */
void Flowmeter::reset(void)
{
  liters = 0;
  totalLiters = 0.0;
  oldTime = 0;
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
