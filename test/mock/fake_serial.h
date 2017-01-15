#pragma once

#include <iostream>

class FakeSerial {
public:
  void begin(unsigned long);
  void end();
//  size_t write(const unsigned char*, size_t);

    void print( const char *msg);
    void print( const unsigned char *msg);
    void print( const char c);
    void print( int msg);
    void print( long msg);
};

extern FakeSerial Serial;
