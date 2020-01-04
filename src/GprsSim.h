/*
  GprsSim.h - Library for gsm modules (works with sim800 and sim900).
  Created by Artem Dudkin, Jan 14, 2017.
  Released into the public domain.
*/

#ifndef GprsSim_h
#define GprsSim_h

#ifndef _TEST_
#include <Arduino.h>
#include "NeoSWSerial.h"
#include <avr/pgmspace.h>
#else
#include "../test/mock/Arduino.h"
#include "../test/mock/NeoSWSerial.h"
#include "../test/mock/pgmspace.h"
#include <iostream>
#include <cstring>
using namespace std;
#endif

typedef void (*VoidFunction) ();

class GprsSim
{
  public:
    GprsSim(int pinRx, int pinTx, byte* buf, const int BUF_SIZE);
    void setLog(bool);
    void setDebug(bool);
    void setResponceCallback(VoidFunction cb);
    void begin();
        
    int init();
    int attachGPRS(const char *apn, const char *login, const char *password);
    int checkGPRS();
    int http_get(const char *host, const int port, char *url_buf[], const int URL_BUF_COUNT);
    
    int start();
    int send( char host[], int port, char *url_buf[], const int URL_BUF_COUNT);
  private:
    int _pinRx, _pinTx;
    NeoSWSerial _serial;    
    byte* _buf;
    int _BUF_SIZE;
    VoidFunction _responce_cb;
    bool _log; 
    bool _debug;
    bool _began;    
    
    bool cmd(const char *AT_cmd_string, 
             const int cmd_timeout, const int cmd_char_timeout, const int cmd_ttfb, 
             const char *response_string);
    bool cmd(const char *AT_cmd_string, 
             const int cmd_timeout, const int cmd_char_timeout, const int cmd_ttfb, 
             const char *response_string, 
             const bool compare_result, 
             const int try_count_max, const int delay_between_try);
    bool cmd(const char *AT_cmd_string,
             const int cmd_timeout, const int cmd_char_timeout, const int cmd_ttfb,
             const char *response_string,
             const bool compare_result,
             const int try_count_max, const int delay_between_try,
             const char *err_msg, const char *ok_msg);
    bool check_responce(const int cmd_timeout, const int cmd_char_timeout, const int cmd_ttfs,
                        const char *response_string);
                        
    void print( const char *msg);
    void println( const char *msg);
    void println( const char c);
    void println( int c);
    void println( long c);
    
    void log( const char *msg);
    void log( const __FlashStringHelper *msg);
    void log( const char c);
    void log( int msg);
    void log( long msg);
    
    void debug( const char *msg);
    void debug( const __FlashStringHelper *msg);
    void debug( const char c);
    void debug( int msg);
    void debug( long msg);
};

#endif
