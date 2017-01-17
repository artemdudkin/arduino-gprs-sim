#include "assert.h"
#include "mock/Arduino.h"
#include "mock/SoftwareSerial.h"
#include "GprsSim.h"



void test_millis() {
  unsigned long start = millis();
  int i=1;
  while( millis() - start < 5000 ) {
    sleep(1);
    
    long ms = millis();
    assert.less((1000*i + (i>0? i*3: 3)), ms-start);
    i++;
  }
  unsigned long end = millis();
  assert.less(5015, end-start);
}



void test_delay() {
  unsigned long start = millis();
  int i=1;
  while( millis() - start < 5000 ) {
    delay(250);
    
    long ms = millis();
    assert.greater(250*i-2, ms-start);
    assert.less((250*i + (i>0? i*2: 2)), ms-start);
    i++;
  }
  unsigned long end = millis();
  assert.less(5015, end-start);  
}



#define XBUF_SIZE 30
byte xbuf[XBUF_SIZE];

void test_gprs_start_no_AT_answer() {
  GprsSim g(5, 6, xbuf, XBUF_SIZE);
  int status = g.start();
  assert.equal(-1, status);
}

void test_gprs_start_ok() {
  SoftwareSerial::flow_add("AT", "ERR");
  SoftwareSerial::flow_add("AT", "OK");  
  SoftwareSerial::flow_add("AT&F", "OK");    
  SoftwareSerial::flow_add("ATE0", "OK");    

  GprsSim g(5, 6, xbuf, XBUF_SIZE);
  int status = g.start();
  assert.equal(-1, status);
}


int main(int argc, char **argv){
  initialize_mock_arduino();
  
  START( test_millis, "millis");
  START( test_delay,  "delay");
  START( test_gprs_start_no_AT_answer,  "gprs_start_no_AT_answer");
  START( test_gprs_start_ok,  "gprs_start_ok");  

  assert.test_results();
}
