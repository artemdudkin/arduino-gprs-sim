#pragma once

typedef unsigned char __FlashStringHelper;

class SoftwareSerial
{
  public:
    SoftwareSerial(int pinRx, int pinTx);
    
    void begin(int rate);
    
    void listen();
    char read();
    int available();
    
    void print( const char *msg);
    void print( const __FlashStringHelper *msg);
    void print( const char c);
    void print( int msg);
    void print( long msg);
    
    void println( const char *msg);
    void println( const __FlashStringHelper *msg);
    void println( const char c);
    void println( int msg);
    void println( long msg);
  private:
};

