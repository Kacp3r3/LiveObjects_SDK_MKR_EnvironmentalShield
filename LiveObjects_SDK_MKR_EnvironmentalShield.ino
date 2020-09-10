#include "arduino_secrets.h"
#include "MPL115A1.h"
#include "PMSA.h"
#include "TSL235R.h"
#include "TGS2600.h"
#include <LiveObjects.h>
#include <DHT.h>

#define PRESSURE_CS_PIN 7
#define DHT_PIN 3
#define PMS_SET 2
#define PMS_RESET 1
#define TSL235R_DATA 6
#define TGS2600_ENABLE 0
#define TGS2600_DATA A1

MPL115A1 pressure(PRESSURE_CS_PIN);
DHT dht(DHT_PIN,DHT11);
PMSA pmsa(Serial1,PMS_RESET, PMS_SET);
TSL235R light(TSL235R_DATA);
//TGS2600 pollutions(TGS2600_ENABLE,TGS2600_DATA);

int last=10;
int messageRate=1000;

void setup() 
{
  //Begin all sensors
  pmsa.begin();
  pressure.begin();
  dht.begin();
  light.begin();
  //pollutions.begin();

  //Configure and connect to LiveObjects
  lo.addParameter("messageRate",messageRate);
  lo.setSecurity(NONE);
  lo.begin();
  lo.connect();
}

void loop() 
{
  if(millis()-last > messageRate)
  {
    StaticJsonDocument<256> doc;

    //PMSA003
    pmsa.readValues();
    doc["03um"] = pmsa.values.particles_03um;
    doc["05um"] = pmsa.values.particles_05um;
    doc["10um"]= pmsa.values.particles_10um;
    doc["25um"]= pmsa.values.particles_25um;
    doc["50um"]= pmsa.values.particles_50um;
    doc["100um"]= pmsa.values.particles_100um;
    JsonObject tmp = doc.as<JsonObject>();
    lo.addObjectToPayload("PMSA003", tmp);
    doc.clear();

    //DHT
    doc["Temperature"] = dht.readTemperature();
    doc["Humidity"] = dht.readHumidity();
    lo.addObjectToPayload("DHT", tmp);
    doc.clear();

    //MPL115A1
    doc["Pressure"] = pressure.readPressure();
    lo.addObjectToPayload("MPL115A1", tmp);
    doc.clear();

    //TSL235R
    doc["Light_Energy"] = light.readLightEnergy();
    lo.addObjectToPayload("TSL235R", tmp);
    doc.clear();

    //TGS2600
    // doc["Pollutions"] = pollutions.readPollutions();
    // lo.addObjectToPayload("TGS2600", tmp);
    // doc.clear();

    //Battery information
    lo.addPowerStatus();

    //Send Data
    lo.sendData();
    last = millis();
  }

  //Maintaining the connection
  lo.loop();
}  