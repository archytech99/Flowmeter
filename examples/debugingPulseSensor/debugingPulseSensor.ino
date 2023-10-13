#include <Flowmeter.h>
#define PIN_SENSOR 2
Flowmeter fw(PIN_SENSOR);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  float _cal = 0.00002; //calibrator factory
  float _tol = 0.0; //tolerance of liter
  long _mlimit = 100000; //maximum limit measurement
  fw.set(_cal, _tol, _mlimit);
  
  double _liter = 0.0; //summary liter per second
  int _rliter = 0; //maximum rounding limit of liter
  fw.init(_rliter, _liter);
}

unsigned long oldLoop = 0;
void loop() {
  // put your main code here, to run repeatedly:
  fw.loop();

  if ((millis()-oldLoop) > 1000L) {
    oldLoop = millis();
    fw.debug(true); //set debug random pulse sensor
    Serial.print(F("# liter per second : "));
    Serial.println(fw.liters);
    Serial.print(F("# total liter      : "));
    Serial.print(fw.upLiters);
    Serial.print(F("^100k + "));
    Serial.println(fw.totalLiters);
    Serial.print(F("# total volume     : "));
    Serial.println(fw.getVolume());
    Serial.println();
  }
}
