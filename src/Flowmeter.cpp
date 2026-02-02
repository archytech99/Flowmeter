#include "Flowmeter.h"

volatile uint32_t pulseCount = 0;

#if defined(ESP32)
void IRAM_ATTR Flowmeter::isrHandler()
{
  pulseCount++;
}
#else
void Flowmeter::isrHandler()
{
  pulseCount++;
}
#endif

Flowmeter::Flowmeter(uint8_t pin)
    : _pin(pin),
      _interrupt(digitalPinToInterrupt(pin)),
      _calFactor(1.0f),
      _tolerance(0.0f),
      _totalMl(0),
      _flowLps(0.0f),
      _lastMs(0)
{
}

void Flowmeter::begin(float calibrationFactor, float tolerancePercent)
{
  _calFactor = calibrationFactor;
  _tolerance = tolerancePercent / 100.0f;

  pinMode(_pin, INPUT_PULLUP);
  attachInterrupt(_interrupt, isrHandler, FALLING);
  _lastMs = millis();
}

void Flowmeter::update()
{
  unsigned long now = millis();
  unsigned long deltaMs = now - _lastMs;

  if (deltaMs < 1000)
    return;

  _lastMs = now;

  // atomic snapshot
  noInterrupts();
  uint32_t pulse = pulseCount;
  pulseCount = 0;
  interrupts();

  if (pulse == 0)
  {
    _flowLps = 0.0f;
    return;
  }

  // frequency (Hz)
  float freq = (1000.0f * pulse) / deltaMs;

  // Liter per second
  float lps = freq / _calFactor;

  if (_tolerance > 0.0f)
    lps *= (1.0f - _tolerance);

  _flowLps = lps;

  // Integrasi volume → mL
  uint64_t deltaMl = (uint64_t)(lps * (deltaMs / 1000.0f) * 1000.0f);
  _totalMl += deltaMl;
}

void Flowmeter::reset()
{
  noInterrupts();
  pulseCount = 0;
  interrupts();

  _totalMl = 0;
  _flowLps = 0.0f;
}

float Flowmeter::getFlowLps() const
{
  return _flowLps;
}

double Flowmeter::getTotalLiters() const
{
  return _totalMl / 1000.0;
}

uint64_t Flowmeter::getTotalMilliLiters() const
{
  return _totalMl;
}
