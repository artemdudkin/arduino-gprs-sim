#include "NeoSWSerial.h"

NeoSWSerial::NeoSWSerial(int pinRx, int pinTx){
  flow_current  = 0;
  flow_next     = 0;
  buf_length    = 0;
  buf_reading   = false;
}
void NeoSWSerial::begin(int rate){
}
void NeoSWSerial::listen(){
}
char NeoSWSerial::read(){
  check_read(); //check if available was called
  const char* msg = (flow_current < 0 ? flow_err[-flow_current] : flow[flow_current].resp);
  return msg[flow_pos++];
}
int NeoSWSerial::available(){
  check_available(); //check if command recieved via print* is ok
  const char* msg = (flow_current < 0 ? flow_err[-flow_current] : flow[flow_current].resp);
  return (msg[flow_pos] != '\0' ? 1 : 0);
}

void NeoSWSerial::print( const char *msg){
  check_buf_reading();//checks if it is time after read (and clear buf if it is true) 
  int i=0;
  do {
    buf[buf_length++] = msg[i++];
  } while (buf[buf_length-1] != '\0');
}

void NeoSWSerial::print( const __FlashStringHelper *msg){
  check_buf_reading();//checks if it is time after read (and clear buf if it is true) 
  int i=0;
  do {
    buf[buf_length++] = msg[i++];
  } while (buf[buf_length-1] != '\0');
}

void NeoSWSerial::print( const char c){
  check_buf_reading();//checks if it is time after read (and clear buf if it is true) 
  buf[buf_length++] = c;
  buf[buf_length]   = '\0';
}

void NeoSWSerial::print( int msg){
//  cout << msg;
}

void NeoSWSerial::print( long msg){
//  cout << msg;
}

void NeoSWSerial::println( const char *msg){
  print(msg);
}
void NeoSWSerial::println( const __FlashStringHelper *msg){
  print(msg);
}
void NeoSWSerial::println( const char c){
  print(c);
}
void NeoSWSerial::println( int msg){
  print(msg);
}
void NeoSWSerial::println( long msg){
  print(msg);
}



//-----------------------
//
// ABOUT FIXTURES
//
//-----------------------

//check if command recieved via print* is ok
void NeoSWSerial::check_available(){
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
void NeoSWSerial::check_read(){
  if (!buf_reading) {
    buf_reading = true;
    flow_pos = 0;
  
    flow_current = -3;
  }
}
//checks if it is time after read (and clear buf if it is true) 
void NeoSWSerial::check_buf_reading(){
  if (buf_reading) {
    buf_reading = false;
    buf_length  = 0;
  }
}


//-----------------------
// Static Members
//-----------------------

const char *NeoSWSerial::flow_err[]  = {"", "ERR_WRONG_CMD", "ERR_FLOW_IS_OVER", "ERR_MISSED_AVAILABLE"};
int         NeoSWSerial::flow_length = 0;
FlowItem   *NeoSWSerial::flow        = new FlowItem[FLOW_SIZE];

void NeoSWSerial::flow_add(const char *cmd, const char *resp){
  if (flow_length < FLOW_SIZE) {
    flow[flow_length].cmd  = cmd;
    flow[flow_length].resp = resp;
    flow_length++;
  }
}

void NeoSWSerial::flow_clear(){
  flow_length  = 0;
}
