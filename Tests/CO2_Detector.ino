#include "MQ135.h"


#define SENSOR 32

void setup()
{
  Serial.begin(9600);
    delay(5000);
}


  void loop()
  {
    MQ135 gasSensor = MQ135(SENSOR);
    float air_quality = gasSensor.getPPM();
    Serial.print("Air Quality: ");  
    Serial.print(air_quality);
    Serial.println("  PPM");   
    Serial.println();

    Serial.println("Waiting...");

    delay(2000);    
}