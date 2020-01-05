#pragma once

#include <stdlib.h>
#include <sys/timeb.h>
#include <iostream>
using namespace std;

class Assert
{
  public:
    Assert();
    void equal(long, long);
    void less(long, long);
    void greater(long, long);
    
    void test_start(const char*);
    void test_finish();
    void print_results_and_quit();    
    
  private:    
    int assert_ok;
    int assert_total;
    bool current_test_ok;
    const char* current_test_name;
    unsigned long test_time_start;
};

extern Assert assert;

#define START(func, name) assert.test_start(name); func(); assert.test_finish();