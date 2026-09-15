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

  // calibrationFactor is in Hz per (L/s): getFlowLps() == frequency / calibrationFactor.
  // Example: a sensor spec'd as F(Hz) = 0.2 * Q(L/min) has 12 Hz per L/s, so call
  // begin(12.0f, 0.0f). Derive your own value as:
  //   calibrationFactor = pulseFrequencyAtOneLiterPerMinute / 60.0f
  //
  // Returns false (and leaves the instance inert) if `pin` has no usable interrupt
  // on this board/core. Check the return value if you want to detect and report
  // that condition instead of silently getting zero flow forever - the previous
  // version of this function returned void, so failures could not be observed by
  // the caller. Call isValid() at any time to check status again.
  bool begin(float calibrationFactor, float tolerancePercent = 0.0f);

  void update();
  void reset();
  bool set(float cal, float fac = 0.0f);

  // True if begin() succeeded (a valid interrupt pin was attached).
  bool isValid() const;

  float getFlowLps() const;
  float getFlowLpm() const;
  float getFlowLph() const;
  double getTotalLiters() const;
  uint64_t getTotalMilliLiters() const;

private:
  uint8_t _pin;
  int8_t _interrupt;
  float _calFactor;

  // tolerancePercent/100, applied as lps *= (1 - _tolerance). This is a one-way
  // downward correction for a sensor that is known to over-read, NOT a symmetric
  // error-margin/uncertainty setting. If your readings are too LOW, adjust
  // calibrationFactor instead - this field cannot correct for under-reading.
  float _tolerance;

  uint64_t _totalMl;

  // Fractional milliliters carried over between update() calls so they are not
  // discarded by integer truncation; see integrateVolumeMl().
  double _mlRemainder;

  float _flowLps;
  bool _valid;

  unsigned long _lastMs;

  // Pure helper, factored out so it can be unit tested on a host without hardware:
  // computes the whole-milliliter volume to add for one measurement interval and
  // updates carryRemainderMl with whatever fractional mL is left over, so no
  // volume is silently lost to truncation across many short update() cycles.
  static uint64_t integrateVolumeMl(
      float lps,
      unsigned long deltaMs,
      double &carryRemainderMl
  );
};

#endif
