#include <Arduino.h>
#include "Flowmeter.h"

#if defined(EXTERNAL_NUM_INTERRUPTS)
#define FLOWMETER_MAX_INTERRUPTS EXTERNAL_NUM_INTERRUPTS
#else
#define FLOWMETER_MAX_INTERRUPTS 16
#endif

#if defined(ESP32)
#define FLOWMETER_ISR_ATTR IRAM_ATTR
#else
#define FLOWMETER_ISR_ATTR
#endif

static volatile uint32_t pulseCounts[FLOWMETER_MAX_INTERRUPTS] = {};

static void FLOWMETER_ISR_ATTR handleIsr(uint8_t index)
{
  pulseCounts[index]++;
}

#define FLOWMETER_ISR_N(n)                \
  static void FLOWMETER_ISR_ATTR isr##n() \
  {                                       \
    handleIsr(n);                         \
  }

#if FLOWMETER_MAX_INTERRUPTS > 0
FLOWMETER_ISR_N(0)
#endif
#if FLOWMETER_MAX_INTERRUPTS > 1
FLOWMETER_ISR_N(1)
#endif
#if FLOWMETER_MAX_INTERRUPTS > 2
FLOWMETER_ISR_N(2)
#endif
#if FLOWMETER_MAX_INTERRUPTS > 3
FLOWMETER_ISR_N(3)
#endif
#if FLOWMETER_MAX_INTERRUPTS > 4
FLOWMETER_ISR_N(4)
#endif
#if FLOWMETER_MAX_INTERRUPTS > 5
FLOWMETER_ISR_N(5)
#endif
#if FLOWMETER_MAX_INTERRUPTS > 6
FLOWMETER_ISR_N(6)
#endif
#if FLOWMETER_MAX_INTERRUPTS > 7
FLOWMETER_ISR_N(7)
#endif
#if FLOWMETER_MAX_INTERRUPTS > 8
FLOWMETER_ISR_N(8)
#endif
#if FLOWMETER_MAX_INTERRUPTS > 9
FLOWMETER_ISR_N(9)
#endif
#if FLOWMETER_MAX_INTERRUPTS > 10
FLOWMETER_ISR_N(10)
#endif
#if FLOWMETER_MAX_INTERRUPTS > 11
FLOWMETER_ISR_N(11)
#endif
#if FLOWMETER_MAX_INTERRUPTS > 12
FLOWMETER_ISR_N(12)
#endif
#if FLOWMETER_MAX_INTERRUPTS > 13
FLOWMETER_ISR_N(13)
#endif
#if FLOWMETER_MAX_INTERRUPTS > 14
FLOWMETER_ISR_N(14)
#endif
#if FLOWMETER_MAX_INTERRUPTS > 15
FLOWMETER_ISR_N(15)
#endif

static void (*const isrTable[FLOWMETER_MAX_INTERRUPTS])() = {
#if FLOWMETER_MAX_INTERRUPTS > 0
    isr0,
#endif
#if FLOWMETER_MAX_INTERRUPTS > 1
    isr1,
#endif
#if FLOWMETER_MAX_INTERRUPTS > 2
    isr2,
#endif
#if FLOWMETER_MAX_INTERRUPTS > 3
    isr3,
#endif
#if FLOWMETER_MAX_INTERRUPTS > 4
    isr4,
#endif
#if FLOWMETER_MAX_INTERRUPTS > 5
    isr5,
#endif
#if FLOWMETER_MAX_INTERRUPTS > 6
    isr6,
#endif
#if FLOWMETER_MAX_INTERRUPTS > 7
    isr7,
#endif
#if FLOWMETER_MAX_INTERRUPTS > 8
    isr8,
#endif
#if FLOWMETER_MAX_INTERRUPTS > 9
    isr9,
#endif
#if FLOWMETER_MAX_INTERRUPTS > 10
    isr10,
#endif
#if FLOWMETER_MAX_INTERRUPTS > 11
    isr11,
#endif
#if FLOWMETER_MAX_INTERRUPTS > 12
    isr12,
#endif
#if FLOWMETER_MAX_INTERRUPTS > 13
    isr13,
#endif
#if FLOWMETER_MAX_INTERRUPTS > 14
    isr14,
#endif
#if FLOWMETER_MAX_INTERRUPTS > 15
    isr15,
#endif
};

Flowmeter::Flowmeter(uint8_t pin)
    : _pin(pin),
      _interrupt(-1),
      _calFactor(1.0f),
      _tolerance(0.0f),
      _totalMl(0),
      _flowLps(0.0f),
      _valid(false),
      _lastMs(0)
{
}

void Flowmeter::begin(float calibrationFactor, float tolerancePercent)
{
  _interrupt = digitalPinToInterrupt(_pin);
  if (_interrupt < 0 || _interrupt >= FLOWMETER_MAX_INTERRUPTS)
  {
    _valid = false;
    return;
  }
  _valid = true;

  _calFactor = calibrationFactor;
  _tolerance = tolerancePercent / 100.0f;

  pinMode(_pin, INPUT_PULLUP);
  pulseCounts[_interrupt] = 0;
  attachInterrupt(_interrupt, isrTable[_interrupt], FALLING);
  _lastMs = millis();
}

void Flowmeter::update()
{
  if (!_valid)
    return;

  unsigned long now = millis();
  unsigned long deltaMs = now - _lastMs;

  if (deltaMs < 1000)
    return;

  _lastMs = now;

  // atomic snapshot
  noInterrupts();
  uint32_t pulse = 0;
  if (_interrupt >= 0 && _interrupt < FLOWMETER_MAX_INTERRUPTS)
  {
    pulse = pulseCounts[_interrupt];
    pulseCounts[_interrupt] = 0;
  }
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
  if (!_valid)
    return;

  noInterrupts();
  if (_interrupt >= 0 && _interrupt < FLOWMETER_MAX_INTERRUPTS)
    pulseCounts[_interrupt] = 0;
  interrupts();

  _totalMl = 0;
  _flowLps = 0.0f;
}

float Flowmeter::getFlowLps() const
{
  return _flowLps;
}

float Flowmeter::getFlowLpm() const
{
  return _flowLps * 60.0f;
}

float Flowmeter::getFlowLph() const
{
  return _flowLps * 3600.0f;
}

double Flowmeter::getTotalLiters() const
{
  return _totalMl / 1000.0;
}

uint64_t Flowmeter::getTotalMilliLiters() const
{
  return _totalMl;
}
