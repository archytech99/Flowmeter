#include "Flowmeter.h"

volatile byte pulseCount;

void pulseCounter(void) { pulseCount++; }

Flowmeter::Flowmeter(byte _sensor) {
  sensorPin       = _sensor;
  sensorInterrupt = digitalPinToInterrupt(sensorPin);
  oldTime         = 0;
}

/*
  Set Debug Measurement Flowmeter
 */
void Flowmeter::debug(bool _set = false) {
  if (_set)
  {
    pulseCount = random(9,49);
  }
  else
  {
    pulseCount = 0;
  }
}

/*
  Initial Setup Flowmeter
 */
void Flowmeter::init(int _uplit, double _tolit) {
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, LOW);
  attachInterrupt(sensorInterrupt, pulseCounter, RISING);
  set(_tolit, _uplit);
}

/*
  The measurement of discharge water - Liter per Second
 */
void Flowmeter::loop(void) {
  if ((millis() - oldTime) > 1000UL) {
    detachInterrupt(sensorInterrupt);

    unsigned long pulse = (1000.0 / (millis() - oldTime)) * pulseCount;
    float _flowrate     = pulse / calibrationFactor;
    float _liter        = _flowrate / 60;
    oldTime             = millis();
    liters              = _liter - (_liter * tolerance) / 60;
    totalLiters         += liters;
    pulseCount          = 0;
    sumTotalLiter(totalLiters);

    attachInterrupt(sensorInterrupt, pulseCounter, RISING);
  }
}

/*
  Change value of Calibration Factor and Tolerance
 */
void Flowmeter::reset(void) {
  liters       = 0;
  totalLiters  = 0.0;
  oldTime      = 0;
}

/*
  Change value of Calibration Factor and Tolerance
 */
void Flowmeter::set(float _cal, float _tol, long maxlim) {
  calibrationFactor = _cal;
  tolerance         = _tol / 100;
  max_limit         = maxlim;
}

/*
  Change value of Calibration Factor and Tolerance
 */
void Flowmeter::set(double _liter, int _upliter) {
  upLiters    = _upliter;
  totalLiters = _liter;
}
