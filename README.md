# Smart Water Measurement Library for Arduino

## Version 1.0.1

## Installation Instructions

The following instructions can be used to install the latest version of the library in the Arduino IDE:

1. Download [the latest library ZIP file](https://github.com/archytech99/Flowmeter/archive/master.zip)
2. In the Arduino IDE, select `Sketch` > `Include Library` > `Add .ZIP Library...`.  Browse to where the downloaded ZIP file is located and click `Open`. The Flowmeter library's examples will now appear under `File` > `Examples` > `Flowmeter`.

## Examples

### Simple example

```C++
#include <Flowmeter.h>
#define PIN_SENSOR 2
Flowmeter fw(PIN_SENSOR);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  int irq = digitalPinToInterrupt(PIN_SENSOR);
  if (irq < 0) {
    Serial.println("Flowmeter: invalid interrupt pin.");
    Serial.println("UNO/Nano: use D2 or D3.");
    Serial.println("Mega 2560: use D2, D3, D18, D19, D20, D21.");
    Serial.println("ESP32: use a valid GPIO interrupt pin.");
  }

  fw.begin(0.208f, 0.0f); // calibration, tolerance %

}

unsigned long oldLoop = 0;
void loop() {
  // put your main code here, to run repeatedly:
  fw.update();

  if ((millis()-oldLoop) > 1000L && fw.getFlowLps() != 0.0f) {
    oldLoop = millis();
    Serial.print("Flow LPS: ");
    Serial.println(fw.getFlowLps(), 4);
    Serial.print("Flow LPM: ");
    Serial.println(fw.getFlowLpm(), 3);
    Serial.print("Flow LPH: ");
    Serial.println(fw.getFlowLph(), 2);

    Serial.print("Total L: ");
    Serial.println(fw.getTotalLiters(), 3);
    Serial.println();
  }
}
```

## Supported boards (tested targets)

This library targets Arduino-compatible cores for:

- Arduino UNO (ATmega328P) - interrupt pins: D2, D3
- Arduino Nano (ATmega328P) - interrupt pins: D2, D3
- Arduino Mega 2560 (including Mega 2560 Pro) - interrupt pins: D2, D3, D18, D19, D20, D21
- ESP32 - most GPIOs support interrupts; avoid strapping pins and input-only pins

Recommended ESP32 GPIOs:

- 4, 5, 13, 14, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33

Avoid on ESP32:

- Strapping pins: GPIO0, GPIO2, GPIO12, GPIO15
- Input-only pins: GPIO34, GPIO35, GPIO36, GPIO39

If you select a pin without interrupt support, `begin()` returns early and the library will not count pulses.
