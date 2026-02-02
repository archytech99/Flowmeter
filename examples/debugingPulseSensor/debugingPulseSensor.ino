#include <Flowmeter.h>
#define PIN_SENSOR 2
Flowmeter fw(PIN_SENSOR);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  fw.begin(450.0f, 1.5f); // calibration, tolerance %

}

unsigned long oldLoop = 0;
void loop() {
  // put your main code here, to run repeatedly:
  fw.update();

  if ((millis()-oldLoop) > 1000L) {
    oldLoop = millis();
    Serial.print("Flow LPS: ");
    Serial.println(fw.getFlowLps(), 4);

    Serial.print("Total L: ");
    Serial.println(fw.getTotalLiters(), 3);
    Serial.println();
  }
}
