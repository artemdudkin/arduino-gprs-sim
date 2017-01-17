/*
  GprsSim.cpp - Library for gsm modules (works with sim800 and sim900).
  Created by Artem Dudkin, Jan 14, 2017.
  Released into the public domain.
*/

#include "GprsSim.h"

//#define SUPRESS_DEBUG_MESSAGES //uncomment it for prod environment (i.e. when you do not need logs at Serial),
//#define SUPRESS_LOG_MESSAGES   //and this will reduce size of compiled code by 1k aprox.

const char AT[] PROGMEM = "AT";
const char ATF[] PROGMEM = "AT&F";
const char ATE0[] PROGMEM = "ATE0";
const char AT_CMEE[] PROGMEM = "AT+CMEE=1";
const char AT_CMGF[] PROGMEM = "AT+CMGF=1";
const char AT_CSPN[] PROGMEM = "AT+CSPN?";
const char AT_CGATT[] PROGMEM = "AT+CGATT?";
const char AT_CIPSHUT[] PROGMEM = "AT+CIPSHUT";
const char AT_CIPSTATUS[] PROGMEM = "AT+CIPSTATUS";
const char AT_CIPMUX[] PROGMEM = "AT+CIPMUX=0";
const char AT_CSTT[] PROGMEM = "AT+CSTT= \"";
const char PCP[] PROGMEM = "\",\"";
const char P1[] PROGMEM = "\"";
const char PC[] PROGMEM = "\",";
const char AT_CIICR[] PROGMEM = "AT+CIICR";
const char AT_CIFSR[] PROGMEM = "AT+CIFSR";
const char AT_CIPSTART[] PROGMEM = "AT+CIPSTART= \"TCP\",\"";
const char AT_CIPSEND[] PROGMEM = "AT+CIPSEND";
const char GET[] PROGMEM = "GET ";
const char HTTP11[] PROGMEM = " HTTP/1.1\r\nHost: ";
const char USER_AGENT[] PROGMEM = "\r\nUser-Agent: arduino-gprs-sim";
const char CON_CLOSE[] PROGMEM = "\r\nConnection: close\r\n\r\n";

#define PMBUF_SIZE 30
char pmbuf[PMBUF_SIZE];

const char OK[] PROGMEM = "OK";
const char ERR[] PROGMEM = "ERROR";
const char CGATT1[] PROGMEM = "CGATT: 1";
const char SHUT_OK[] PROGMEM = "SHUT OK";
const char IP_INITIAL[] PROGMEM = "IP INITIAL";
const char TCP_CLOSED[] PROGMEM = "TCP CLOSED";
const char IP_GPRSACT[] PROGMEM = "IP GPRSACT";
const char IP_STATUS[] PROGMEM = "IP STATUS";
const char CONNECT_OK[] PROGMEM = "CONNECT OK";
const char SEND_OK[] PROGMEM = "SEND OK";
const char HTTP[] PROGMEM = "HTTP/";

#define OKBUF_SIZE 12
char okbuf[OKBUF_SIZE];

const char MEGAFON[] PROGMEM =  "MegaFon";
const char MEGAFON_APN[] PROGMEM = "internet";
const char MEGAFON_LOGIN[] PROGMEM = "gdata";

const char BEELINE[] PROGMEM =  "Beeline";
const char BEELINE_APN[] PROGMEM = "internet.beeline.ru";
const char BEELINE_LOGIN[] PROGMEM = "beeline";

const char MTS[] PROGMEM =  "MTS";
const char MTS_APN[] PROGMEM = "internet.mts.ru";
const char MTS_LOGIN[] PROGMEM = "mts";

const char WIN[] PROGMEM =  "WIN";
const char WIN_APN[] PROGMEM = "internet";
const char WIN_LOGIN[] PROGMEM = "internet";

#define APN_BUF_SIZE 20
char apn_buf[APN_BUF_SIZE];

#define LOGIN_BUF_SIZE 10
char login_buf[LOGIN_BUF_SIZE];








GprsSim::GprsSim(int pinRx, int pinTx, byte* buf, const int BUF_SIZE) : _serial(pinRx, pinTx)
{
  _pinRx       = pinRx;
  _pinTx       = pinTx;
  _buf         = buf;
  _BUF_SIZE    = BUF_SIZE;
  _responce_cb = NULL;
  
  _log=true;
  _debug=true;
  _began=false;
}

void GprsSim::begin(){
  _began=true;
  _serial.begin(9600);
  delay(500);
}

void GprsSim::setLog(bool value){
  _log = value;
}
void GprsSim::setDebug(bool value){
  _log = value;
}

void GprsSim::setResponceCallback(VoidFunction cb)
{
  _responce_cb = cb;
}


int GprsSim::init() 
{
  if (!_began) begin();
  int ok = 0;

  //just test
  strcpy_P(pmbuf, AT); strcpy_P(okbuf, OK);
  if (ok==0) ok = cmd( pmbuf,  500, 50, 0, okbuf, true, 5,  500) ? 0: 1; //"AT" -> OK
  
  //reset to factory settings
  strcpy_P(pmbuf, ATF); strcpy_P(okbuf, OK);
  if (ok==0) ok = cmd( pmbuf, 1000, 50, 0, okbuf, true, 3,  500) ? 0: 2;//"AT&F" -> OK
  
  //turn echo off
  strcpy_P(pmbuf, ATE0); strcpy_P(okbuf, OK);
  if (ok==0) ok = cmd( pmbuf,  500, 50, 0, okbuf, true, 3,  500) ? 0: 3;//"ATE0" -> OK

  //get error with codes (insted if simple ERROR)
  strcpy_P(pmbuf, AT_CMEE); strcpy_P(okbuf, OK);
  if (ok==0) ok = cmd( pmbuf,  500, 50, 0, okbuf, true, 5, 1000) ? 0: 4;//AT+CMEE=1 -> OK

  //switch sms message format to text mode
  strcpy_P(pmbuf, AT_CMGF); strcpy_P(okbuf, OK);
  if (ok==0) ok = cmd( pmbuf,  500, 50, 0, okbuf, true, 5, 1000) ? 0: 4;//AT+CMGF=1 -> OK
  
  //get sysop name (fail if no SIM)
  strcpy_P(pmbuf, AT_CSPN); strcpy_P(okbuf, ERR);
  if (ok==0) ok = cmd( pmbuf,  500, 50, 0, okbuf, false, 5, 1000) ? 0: 5;//"AT+CSPN?" -> ! ERROR 

  return ok;
}


int GprsSim::attachGPRS( const char *apn,
                     const char *login,
                     const char *password
){
  int ok = 0;

  strcpy_P(pmbuf, AT_CGATT); strcpy_P(okbuf, CGATT1);
  if (ok==0) ok = cmd( pmbuf, 500, 50, 0, okbuf, true, 1, 500) ? 0 : 1;//"AT+CGATT?" -> CGATT: 1
  
  //Reset the IP session if any
  strcpy_P(pmbuf, AT_CIPSHUT); strcpy_P(okbuf, SHUT_OK);
  if (ok==0) ok = cmd( pmbuf, 500, 50, 0, okbuf, true, 0, 0) ? 0 : 2; //"AT+CIPSHUT" -> SHUT OK
  
  //Check if the IP stack is initialized
  strcpy_P(pmbuf, AT_CIPSTATUS); strcpy_P(okbuf, IP_INITIAL);
  if (ok==0) ok = cmd( pmbuf, 500, 50, 0, okbuf, true, 0, 0) ? 0 : 3;  //"AT+CIPSTATUS" - > IP INITIAL
  
  //To keep things simple, I'm setting up a single connection mode
  strcpy_P(pmbuf, AT_CIPMUX); strcpy_P(okbuf, OK);
  if (ok==0) ok = cmd( pmbuf, 500, 50, 0, "OK", true, 0, 0) ? 0 : 4; //"AT+CIPMUX=0" - > OK 

  if (ok==0) {
    //Start the task, based on the SIM card you are using, you need to know the APN, username and password for
    //AT+CSTT= "<apn>","<login>","password" -> OK
    strcpy_P(pmbuf, AT_CSTT); strcpy_P(okbuf, OK);
    print(pmbuf); //"AT+CSTT= \""
    print(apn);
    strcpy_P(pmbuf, PCP);
    print(pmbuf); //"\",\""
    print(login);
    strcpy_P(pmbuf, PCP);
    print(pmbuf); //"\",\""
    print(password);
    strcpy_P(pmbuf, P1);
    println(pmbuf); //"\""
    ok = check_responce( 1000, 50, 0, okbuf) ? 0: 5; // -> OK
  }
  //Now bring up the wireless. Please note, the response to this might take some time
  strcpy_P(pmbuf, AT_CIICR); strcpy_P(okbuf, OK);
  if (ok==0) ok = cmd( pmbuf, 5000, 50, 0, okbuf, true, 0, 0) ? 0 : 6;//"AT+CIICR" -> OK
  
  //Get the local IP address
  strcpy_P(pmbuf, AT_CIFSR); strcpy_P(okbuf, ERR);
  if (ok==0) ok = cmd( pmbuf, 500, 50, 0, okbuf, false, 0, 0) ? 0 : 7;//"AT+CIFSR" -> ! ERROR
  return ok;
}


int GprsSim::checkGPRS()
{ //ok if AT+CIPSTATUS -> TCP CLOSED or IP GPRSACT or IP STATUS
  //      and AT+CIFSR is not ERROR

  int ok = 0;
  strcpy_P(pmbuf, AT_CIPSTATUS); strcpy_P(okbuf, TCP_CLOSED);
  ok = cmd( pmbuf, 2000, 50, 0, okbuf) ? 0 : 1;//"AT+CIPSTATUS" -> TCP CLOSED
  strcpy_P(okbuf, IP_GPRSACT);
  if (ok!=0) ok = (strstr((char*)_buf, okbuf) != NULL) ? 0 : 1; //IP GPRSACT is also good answer for AT+CIPSTATUS
  strcpy_P(okbuf, IP_STATUS);
  if (ok!=0) ok = (strstr((char*)_buf, okbuf)  != NULL) ? 0 : 1; //IP STATUS is also good answer for AT+CIPSTATUS

  //get IP to ensure GPRS is really active
  strcpy_P(pmbuf, AT_CIFSR); 
  strcpy_P(okbuf, ERR);
  if (ok==0) ok = cmd( pmbuf, 500, 50, 0, okbuf, false, 0, 0) ? 0 : 2;//"AT+CIFSR" -> ! ERROR
    
  return ok;
}


int GprsSim::http_get( const char *host,
                   const int port,
                   char *url_buf[],
                   const int URL_BUF_COUNT
){
  int ok = 0;

  //Start the connection, TCP, domain name, port
  //"AT+CIPSTART= \"TCP\",<host>,<port> -> OK
  strcpy_P(pmbuf, AT_CIPSTART); strcpy_P(okbuf, OK);
  print(pmbuf); //"AT+CIPSTART= \"TCP\",\""
  print(host);
  strcpy_P(pmbuf, PC);
  print(pmbuf);//"\","
  println(port);
  ok = check_responce(1000, 50, 0, okbuf) ? 0: 1; // -> OK

  //wait for connection
  strcpy_P(okbuf, CONNECT_OK);
  if (ok==0) ok = check_responce(10000, 50, 0, okbuf) ? 0: 2; // -> CONNECT OK

  //Request initiation of data sending (the request)*
  strcpy_P(pmbuf, AT_CIPSEND);
  if (ok==0) ok = cmd( pmbuf, 5000, 50, 0, ">", true, 3, 500) ? 0: 3;//"AT+CIPSEND"

  if (ok==0) { //Send
               //  GET <url> HTTP/1.1
               //  Host: <host>
               //  User-Agent: arduino-gprs-sim
               //  Connection: close
    strcpy_P(pmbuf, GET);
    print(pmbuf);//"GET "
    for(int i=0; i<URL_BUF_COUNT; i++) {
      print(url_buf[i]);    
    }
    strcpy_P(pmbuf, HTTP11); strcpy_P(okbuf, SEND_OK);
    print(pmbuf);//" HTTP/1.1\r\nHost: "
    print(host);
    strcpy_P(pmbuf, USER_AGENT);
    print(pmbuf);//"\r\nUser-Agent: arduino-gprs-sim"
    strcpy_P(pmbuf, CON_CLOSE);
    print(pmbuf);//"\r\nConnection: close\r\n\r\n"
    println((char)0x1a);
    ok = check_responce(10000, 50, 0, okbuf) ? 0: 4; //waiting for SEND OK

    //get responce
    //(sometimes responce comes with SEND OK - we check for http responce header here - if any, don't wait
    //(or esle we should wait for it)
    strcpy_P(okbuf, HTTP);
    if (ok==0 && (strstr((char*)_buf, okbuf) == NULL)) { // "HTTP/"?
      check_responce(10000, 50, 0, "");
    }
  }

  return ok;
}










int GprsSim::start()
{  int gsm_state = init();
  
  log(F("gsm:: state "));
  log(gsm_state);
  log(F("\r\n"));
  
  // 0 = ok
  // 1 = Cannot send AT command
  // 2 = Cannot reset
  // 3 = Cannot echo off
  // 4 = Cannot setup error messages (with codes)
  // 5 = Not connected to sysop

  int sysop = -1;
  if (gsm_state == 0) {
    strcpy_P(apn_buf, MEGAFON); 
    if (strstr((char*)_buf, apn_buf) != NULL) sysop = 1; //MegaFon
    else {
      strcpy_P(apn_buf, BEELINE); 
      if (strstr((char*)_buf, apn_buf) != NULL) sysop = 2; //Beeline
      else {
        strcpy_P(apn_buf, MTS); 
        if (strstr((char*)_buf, apn_buf) != NULL) sysop = 3; //MTS
        else {
          strcpy_P(apn_buf, WIN); 
          if (strstr((char*)_buf, apn_buf) != NULL) sysop = 4; //WIN
        }
      }
    }

    log(F("gsm:: sysop="));
    log(sysop);
    log(F("\r\n"));
    //-1 = unknown
    // 1 = MegaFon
    // 2 = Beeline
    // 3 = MTS
    // 4 = WIN
  }

  int gprs_state = -1;
  if (gsm_state == 0 && sysop > 0) {
    
    if (sysop == 1) strcpy_P(apn_buf, MEGAFON_APN); 
    else if(sysop == 2) strcpy_P(apn_buf, BEELINE_APN); 
    else if(sysop == 3) strcpy_P(apn_buf, MTS_APN);
    else if(sysop == 4) strcpy_P(apn_buf, WIN_APN);
    
    if (sysop == 1) strcpy_P(login_buf, MEGAFON_LOGIN);
    else if (sysop == 2) strcpy_P(login_buf, BEELINE_LOGIN);
    else if (sysop == 3) strcpy_P(login_buf, MTS_LOGIN);
    else if (sysop == 4) strcpy_P(login_buf, WIN_LOGIN);

    gprs_state = attachGPRS(apn_buf, login_buf, login_buf);
    
    log(F("gsm:: gprs state "));
    log(gprs_state);
    log(F("\r\n"));
    //-1 = unknown sysop
    // 0 = ok
    // 1 = GPRS not activated at modem
    // 2 = Cannot reset IP session
    // 3 = Cannot initialize IP stack
    // 4 = Cannot setup simple connection
    // 5 = Cannot setup GPRS connection
    // 6 = Cannot start GPRS connection
    // 7 = Cannot get local IP
  }

  return gprs_state;
}

int GprsSim::send( char host[],
               int port,
               char *url_buf[],
               const int URL_BUF_COUNT
){
  _serial.listen();
  
  int gprs_state = checkGPRS();
  if (gprs_state == 0) {
    log(F("gsm:: gprs ok"));
    log(F("\r\n"));
  } else {
    gprs_state = start();
  }
  
  if (gprs_state == 0) {
    long lasttime = millis();

    int ok = http_get( host, port, url_buf, URL_BUF_COUNT);
    // 0 = ok
    // 1 = Cannot setup tcp connection
    // 2 = Cannot start tcp connection
    // 3 = Cannot start tcp request
    // 4 = Cannot send tcp data

    log(F("gsm:: sent "));
    log(ok);
    log(F(" [")); 
    log((long)(millis() - lasttime)); 
    log(F("ms]"));
    log(F("\r\n"));
    return ok;
  } else {
    log(F("gsm:: cannot send"));
    log(F("\r\n"));
  }
  return 1;
}






















bool GprsSim::cmd( const char *AT_cmd_string,
               const int cmd_timeout,
               const int cmd_char_timeout,
               const int cmd_ttfb,
               const char *response_string
){
  _serial.listen();
  println(AT_cmd_string);
  return check_responce( cmd_timeout, cmd_char_timeout, cmd_ttfb, response_string);
}


bool GprsSim::cmd( const char *AT_cmd_string,
               const int cmd_timeout,
               const int cmd_char_timeout,
               const int cmd_ttfb,
               const char *response_string,
               const bool compare_result,
               const int try_count_max,
               const int delay_between_try
){
  int try_counter = 0;
  bool ok = cmd( AT_cmd_string, cmd_timeout, cmd_char_timeout, cmd_ttfb, response_string);
  while ((ok != compare_result) && (try_counter < try_count_max)) {

    debug(F("--RETRY "));
    debug(F("\r\n"));
      
    delay(delay_between_try);
    try_counter++;
    ok = cmd( AT_cmd_string, cmd_timeout, cmd_char_timeout, cmd_ttfb, response_string);
  }
  return (ok == compare_result);
}


bool GprsSim::cmd( const char *AT_cmd_string,
               const int cmd_timeout,
               const int cmd_char_timeout,
               const int cmd_ttfb,
               const char *response_string,
               const bool compare_result,
               const int try_count_max,
               const int delay_between_try,
               const char *err_msg,
               const char *ok_msg
){
  bool ok = cmd( AT_cmd_string, cmd_timeout, cmd_char_timeout, cmd_ttfb,response_string,
                 compare_result, try_count_max, delay_between_try);
  if (!ok) {
    log(F("gsm:: "));
    log(err_msg);
    log(F("\r\n"));
    return false;
  } else {
    if (ok_msg != NULL) {
      log(F("gsm:: "));
      log(ok_msg);
      log(F("\r\n"));
    }
  }
  return true;
}


// sample lib(gsmlib): http://www.gsmlib.org/download.html
// sample interaction: http://vsblogs.wordpress.com/2013/11/28/tcp-connection-over-gprs-using-sim900-and-at-commands/

bool GprsSim::check_responce( const int cmd_timeout,
                          const int cmd_char_timeout,
                          const int cmd_ttfs,
                          const char *response_string
) {
  long t_all  = millis();
  long t_char = 0;
  int buf_pos = 0;
  _buf[buf_pos] = 0x00;

  long ttfs = -1; //time to first symbol
  long mtbs = -1; //max tme between symbols
  long tols = -1; //time of last symbol 

  int i=0;
  char c;
  while (
       ((cmd_ttfs <= 0) || (ttfs >= 0 || millis() - t_all < cmd_ttfs)) //check time to first symbol if present
    && (millis() - t_all < cmd_timeout)                               //check total time (timeout)
    && ((t_char == 0) || (millis() - t_char < cmd_char_timeout))      //check time between chars
  ) {
    while (_serial.available() > 0) {

      long t = millis();
      if (tols>0) mtbs=t-tols;
      tols = t;
      if (ttfs<0) ttfs = tols-t_all;
      
      if (buf_pos + 1 < _BUF_SIZE) {
        _buf[buf_pos] = _serial.read();
        _buf[++buf_pos] = 0x00;  // and finish currently received characters
      } else {
        c = _serial.read(); // buffer is full - read and throw it away
        //Serial.print(c); //printing these chars is quite confusing
        i++;
      }
      t_char = millis();
    }
  }

  //------DEBUG------
    debug(F("waited "));
    debug((long)(millis() - t_all));
    if (buf_pos>0) {
      debug(F("ms, ttfs="));
      debug(ttfs);
      if (mtbs>2) {
        debug(F("ms, MTBS="));
        debug(mtbs);
      }
    }
    debug(F("ms,got "));
    debug(buf_pos);
    debug(F(" bytes"));
    if (i>0) {
      debug(F(", ")); 
      debug(i); 
      debug(F(" bytes was lost\r\n"));
    }      
    debug("\r\n");

    debug(F(">> '"));
    debug((char*)_buf);
    debug(F("'\r\n"));
  //------/DEBUG------

  bool ret = false;
  if (strstr((char*)_buf, response_string) != NULL) {
    debug(F("[expected string was received]\r\n"));
    ret = true;
  } else {
    debug(F("[expected string was NOT received]\r\n"));
  }
  
  if (_responce_cb != NULL) {
    _responce_cb();
  }
  
  return ret;
}

void GprsSim::print( const char *msg)
{
  _serial.print(msg);
  
  debug(F("<< "));
  debug(msg);
  debug(F("\r\n"));
}

void GprsSim::println( const char *msg)
{
  _serial.println(msg);
  
  debug(F("<< "));
  debug(msg);
  debug(F("\r\n"));
}

void GprsSim::println( const char c){
  _serial.println(c);
  
  debug(F("<< "));
  debug(c);
  debug(F("\r\n"));
}

void GprsSim::log( const char *msg)
{
  #ifndef SUPRESS_LOG_MESSAGES
    if (_log) Serial.print(msg);
  #endif
}

void GprsSim::log( const __FlashStringHelper *msg){
  #ifndef SUPRESS_LOG_MESSAGES
    if (_log) Serial.print(msg);
  #endif
}

void GprsSim::log( const char msg)
{
  #ifndef SUPRESS_LOG_MESSAGES
    if (_log) Serial.print(msg);
  #endif
}

void GprsSim::log( int msg)
{
  #ifndef SUPRESS_LOG_MESSAGES
    if (_log) Serial.print(msg);
  #endif
}

void GprsSim::log( long msg)
{
  #ifndef SUPRESS_LOG_MESSAGES
    if (_log) Serial.print(msg);
  #endif
}


void GprsSim::debug( const char *msg)
{
  #ifndef SUPRESS_DEBUG_MESSAGES
    if (_debug) Serial.print(msg);
  #endif
}

void GprsSim::debug( const __FlashStringHelper *msg)
{
  #ifndef SUPRESS_DEBUG_MESSAGES
    if (_debug) Serial.print(msg);  
  #endif
}

void GprsSim::debug( const char msg)
{
  #ifndef SUPRESS_DEBUG_MESSAGES
    if (_debug) Serial.print(msg);  
  #endif
}


void GprsSim::debug( int msg)
{
  #ifndef SUPRESS_DEBUG_MESSAGES
    if (_debug) Serial.print(msg);
  #endif
}

void GprsSim::debug( long msg)
{
  #ifndef SUPRESS_DEBUG_MESSAGES
    if (_debug) Serial.print(msg);
  #endif  
}


