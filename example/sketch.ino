#include "NeoSWSerial.h"
#include "GprsSim.h"
#include <OneWire.h>
#include <DallasTemperature.h>


//-----------------------------------------------------------------------------------------
//
// SIM900
//
//-----------------------------------------------------------------------------------------

#define BUF_SIZE 150
byte buf[BUF_SIZE];

GprsSim gs(9, 8, buf, BUF_SIZE);

void sim800_setup(){
  Serial.println("gprs seting up...");
  long lasttime=millis();  
  
  int ok = gs.start();
  
  Serial.print("gprs setup done [");Serial.print(millis() - lasttime); Serial.println("ms]");  
}


char host[] = "194.67.209.146";
int port = 8089;
char url[] = "/log?source=xyz&data=";
#define DATA_SIZE 16
char data[DATA_SIZE];
char *urls[] = {url, data};



//--------------------------------------------------------------------------------------
//
// Temperature
//
//--------------------------------------------------------------------------------------

// DS18S20 Temperature chip i/o
OneWire ds(6); 
DallasTemperature sensors(&ds);

byte temperature_device_count = 0;

void temperature_setup(){  
  //temperature sensor
  sensors.begin();
  sensors.setResolution(TEMP_11_BIT); // 11 bits (instead of 12 by default)
  
  temperature_device_count = sensors.getDeviceCount();
  Serial.print("temp device count = ");
  Serial.println(temperature_device_count);
  
  Serial.println("temp setup done");
}

void temperature_get(){  
  sensors.requestTemperatures();
  
  Serial.print("temp device count = ");
  Serial.println(temperature_device_count);    
  
  byte pos = 0;
  for (int8_t i=0; i<temperature_device_count; i++) {
    float t = sensors.getTempCByIndex(i);
    int8_t t1=(int8_t)trunc(t);
    int8_t t2=(int8_t)round((t-trunc(t)) * 100);

    Serial.print("; temp = ");    
    Serial.print(t1);    
    Serial.print(t2);        
    
//    data_add( i, t1, t2);
    if (pos<DATA_SIZE-1 && i>0) data[pos++] = ';';
    if (pos<DATA_SIZE-1 && (t1<0 || t2<0)) {
      data[pos++] = '-';
      t1 = -t1;
      t2 = -t2;
    }
    if (pos<DATA_SIZE-1) data[pos++] = char((t1/10)%10 + (byte)'0');  
    if (pos<DATA_SIZE-1) data[pos++] = char( t1%10     + (byte)'0');
    if (pos<DATA_SIZE-1) data[pos++] = '.';
    if (pos<DATA_SIZE-1) data[pos++] = char((t2/10)%10 + (byte)'0');  
    if (pos<DATA_SIZE-1) data[pos++] = char( t2%10     + (byte)'0');
  }
  if (pos<=DATA_SIZE-1) data[pos++] = 0x00;  
  Serial.print("; tall = ");
  Serial.println((char*)data);
}



//-----------------------------------------------------------------------------------------
//
// MAIN
//
//-----------------------------------------------------------------------------------------
 
void setup() 
{
  Serial.begin(9600);
//  Serial.println("start");
 
  temperature_setup();
  sim800_setup();  
  
  Serial.println("setup done.");  
}


void loop() {
  
  temperature_get();
  gs.send(host, 8089, urls, 2);
    
  delay(58000);
}

