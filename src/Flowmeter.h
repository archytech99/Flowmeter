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
private:
  byte sensorInterrupt = 0;
  byte sensorPin = 2;
  unsigned long max_limit = 100000;
  float calibrationFactor = 0.001f;
  float tolerance = 0.000f;
  unsigned long oldTime;

protected:
  long multiplier(void)
  {
    return max_limit * upLiters;
  }
  void sumTotalLiter(double _totalLiters)
  {
    if (_totalLiters > (max_limit - 4))
    {
      upLiters++;
      long _total = _totalLiters;
      float _sisa = _totalLiters - _total;
      long _liter = _totalLiters - max_limit;
      totalLiters = _liter;
      if (_sisa > 0.499)
      {
        totalLiters += 1.0;
      }
    }
  }

public:
  const float kubik = 0.001;
  double totalLiters;
  float liters;
  int upLiters = 0;

  Flowmeter(byte _sensor);
  void debug(bool _set, int _min = 9, int _max = 49);
  void init(int _uplit, double _tolit);
  void loop(void);
  void reset(void);
  void set(float _cal, float _tol, long maxlim);
  void set(double _liter, int _upliter);
  double getVolume(void)
  {
    long _mulp = multiplier();
    return ((_mulp + totalLiters) * kubik);
  }
};

#endif
