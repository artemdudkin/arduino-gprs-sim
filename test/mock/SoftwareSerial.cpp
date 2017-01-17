#include "SoftwareSerial.h"

#include <iostream>
using namespace std;

SoftwareSerial::SoftwareSerial(int pinRx, int pinTx){
  flow_current  = 0;
  flow_next     = 0;
  buf_length    = 0;
  buf_reading   = false;
}
void SoftwareSerial::begin(int rate){
}
void SoftwareSerial::listen(){
}
char SoftwareSerial::read(){
  check_read(); //check if available was called
  const char* msg = (flow_current < 0 ? flow_err[-flow_current] : flow[flow_current].resp);
  return msg[flow_pos++];
}
int SoftwareSerial::available(){
  check_available(); //check if command recieved via print* is ok
  const char* msg = (flow_current < 0 ? flow_err[-flow_current] : flow[flow_current].resp);
  return (msg[flow_pos] != '\0' ? 1 : 0);
}

void SoftwareSerial::print( const char *msg){
  check_buf_reading();//checks if it is time after read (and clear buf if it is true) 
  int i=0;
  do {
    buf[buf_length++] = msg[i++];
  } while (buf[buf_length-1] != '\0');
}

void SoftwareSerial::print( const __FlashStringHelper *msg){
  check_buf_reading();//checks if it is time after read (and clear buf if it is true) 
  int i=0;
  do {
    buf[buf_length++] = msg[i++];
  } while (buf[buf_length-1] != '\0');
}

void SoftwareSerial::print( const char c){
  check_buf_reading();//checks if it is time after read (and clear buf if it is true) 
  buf[buf_length++] = c;
  buf[buf_length]   = '\0';
}

void SoftwareSerial::print( int msg){
//  cout << msg;
}

void SoftwareSerial::print( long msg){
//  cout << msg;
}

void SoftwareSerial::println( const char *msg){
  print(msg);
}
void SoftwareSerial::println( const __FlashStringHelper *msg){
  print(msg);
}
void SoftwareSerial::println( const char c){
  print(c);
}
void SoftwareSerial::println( int msg){
  print(msg);
}
void SoftwareSerial::println( long msg){
  print(msg);
}



//-----------------------
//
// ABOUT FIXTURES
//
//-----------------------

//check if command recieved via print* is ok
void SoftwareSerial::check_available(){
  if (!buf_reading) {
    buf_reading = true;
    flow_pos = 0;
    
    if (flow_next >= flow_length) {
      flow_current = -2;
    } else if (strcmp(buf, flow[flow_next].cmd) == 0) {
      flow_current = flow_next;
      flow_next++;
    } else {
      flow_current = -1;
    }
  }
}
//check if available was called
void SoftwareSerial::check_read(){
  if (!buf_reading) {
    buf_reading = true;
    flow_pos = 0;
  
    flow_current = -3;
  }
}
//checks if it is time after read (and clear buf if it is true) 
void SoftwareSerial::check_buf_reading(){
  if (buf_reading) {
    buf_reading = false;
    buf_length  = 0;
  }
}


//-----------------------
// Static Members
//-----------------------

const char *SoftwareSerial::flow_err[]  = {"", "ERR_WRONG_CMD", "ERR_FLOW_IS_OVER", "ERR_MISSED_AVAILABLE"};
int         SoftwareSerial::flow_length = 0;
FlowItem   *SoftwareSerial::flow        = new FlowItem[FLOW_SIZE];

void SoftwareSerial::flow_add(const char *cmd, const char *resp){
  if (flow_length < FLOW_SIZE) {
    flow[flow_length].cmd  = cmd;
    flow[flow_length].resp = resp;
    flow_length++;
  }
}

void SoftwareSerial::flow_clear(){
  flow_length  = 0;
}
