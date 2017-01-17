#include "assert.h"

#include <iostream>
using namespace std;

Assert::Assert() {
  assert_ok       = 0;
  assert_total    = 0;
  test_time_start = 0;
}

void Assert::test_start(const char *name){
  if (test_time_start == 0){
    test_time_start = millis();
    cout << "\r\n";
  }
  current_test_ok = true;
  current_test_name = name;
}

void Assert::test_finish(){
  if (current_test_ok) { 
    cout << " ok : " << current_test_name << "\r\n"; } else { cout << " failed : " << current_test_name << "\r\n"; 
  }
}

void Assert::test_results(){
	cout << "\r\n\r\n\tAsserts total: " << assert_total << ", passed: " << assert_ok;
	if (assert_total != assert_ok) {
		cout << ", FAILED:" << (assert_total - assert_ok);
	}
	cout << " [" << millis() - test_time_start << " ms]\r\n\r\n";
}








void Assert::equal(long expected, long actual) {
    assert_total++;
	if (expected != actual) {
		cout << "\tFailed " << current_test_name << ", expected " << expected << " and got " << actual << "\r\n";
		current_test_ok=false;
		return;
	}
	assert_ok++;
}
void Assert::less(long expected, long actual) {
    assert_total++;
	if (expected <= actual) {
		cout << "\tFailed " << current_test_name << ", expected less then " << expected << " and got " << actual << "\r\n";
		current_test_ok=false;
		return;
	}
	assert_ok++;
}
void Assert::greater(long expected, long actual) {
    assert_total++;
	if (expected >= actual) {
		cout << "\tFailed " << current_test_name << ", expected greater then " << expected << " and got " << actual << "\r\n";
		current_test_ok=false;
		return;
	}
	assert_ok++;
}

Assert assert;