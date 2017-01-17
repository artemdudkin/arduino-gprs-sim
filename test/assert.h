#pragma once

#include "mock/arduino.h"

class Assert
{
  public:
    Assert();
    void equal(long, long);
    void less(long, long);
    void greater(long, long);
    
    void test_start(const char*);
    void test_finish();
    void test_results();    
    
  private:    
    int assert_ok;
    int assert_total;
    int test_time_start;
    bool current_test_ok;
    const char* current_test_name;
};

extern Assert assert;

#define START(func, name) assert.test_start(name); func(); assert.test_finish();