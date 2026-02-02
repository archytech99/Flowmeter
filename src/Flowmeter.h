/*
 * @file       Flowmeter.h
 * @author     archytech99
 * @copyright  Copyright © 2022 ArchyTeCH
 * @date       May 2022
 */

#ifndef Flowmeter_h
#define Flowmeter_h

#include <Arduino.h>

class Flowmeter
{
public:
  explicit Flowmeter(uint8_t pin);

  void begin(float calibrationFactor, float tolerancePercent = 0.0f);
  void update();
  void reset();

  float getFlowLps() const;
  double getTotalLiters() const;
  uint64_t getTotalMilliLiters() const;

private:
  uint8_t _pin;
  uint8_t _interrupt;
  float _calFactor;
  float _tolerance;
  uint64_t _totalMl;
  float _flowLps;

  unsigned long _lastMs;

  static void isrHandler();
};

#endif
