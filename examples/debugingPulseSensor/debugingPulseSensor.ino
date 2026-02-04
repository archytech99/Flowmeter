#include <Flowmeter.h>
#define PIN_SENSOR 2
Flowmeter fw(PIN_SENSOR);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  int irq = digitalPinToInterrupt(PIN_SENSOR);
  if (irq < 0) {
    Serial.println("Flowmeter: invalid interrupt pin.");
    Serial.println("UNO/Nano: use D2 or D3.");
    Serial.println("Mega 2560: use D2, D3, D18, D19, D20, D21.");
    Serial.println("ESP32: avoid GPIO0, GPIO2, GPIO12, GPIO15, GPIO34-39.");
    Serial.println("ESP32: recommended GPIOs: 4, 5, 13, 14, 16, 17, 18, 19, 21, 22, 23, 25-27, 32, 33.");
  }

  fw.begin(12.7f, 0.0f); // calibration, tolerance %

}

unsigned long oldLoop = 0;
void loop() {
  // put your main code here, to run repeatedly:
  fw.update();

  if ((millis()-oldLoop) > 1000L && fw.getFlowLps() > 0.0001f) {
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
