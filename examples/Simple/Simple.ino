#include "FX29K.h"

FX29K scale(FX29K0, 0010, &Wire);

void setup(){
  Wire.begin();
  Serial.begin(115200);
  scale.tare(1000);
  Serial.println("Grams\tPounds\tRaw");
  Serial.println("--------------------");
}

void loop(){
  uint16_t raw = scale.getRawBridgeData();
  float g = scale.getGrams();
  float lb = scale.getPounds();
  Serial.print(g, 1);
  Serial.print("\t");
  Serial.print(lb, 3);
  Serial.print("\t");
  Serial.println(raw);
  delay(1000);
}
