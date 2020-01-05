#include "assert.h"

unsigned long _millis() {
  timeb t_now;
  ftime(&t_now);
  return t_now.time * 1000 + t_now.millitm;
}




Assert::Assert() {
  assert_ok       = 0;
  assert_total    = 0;
  test_time_start = 0;
}

void Assert::test_start(const char *name){
  if (test_time_start == 0){
    test_time_start = _millis();
    cout << "\r\n";
  }
  current_test_ok = true;
  current_test_name = name;
}

void Assert::test_finish(){
  if (current_test_ok) { 
    cout << " [ok] " << current_test_name << "\r\n"; } else { cout << " [failed] " << current_test_name << "\r\n"; 
  }
}

void Assert::print_results_and_quit() {
	cout << "\r\n\r\n\tAsserts total: " << assert_total << ", passed: " << assert_ok;
	if (assert_total != assert_ok) {
		cout << ", FAILED:" << (assert_total - assert_ok);
	}
	cout << " [" << _millis() - test_time_start << " ms]\r\n\r\n";
	exit( assert_total == assert_ok ? EXIT_SUCCESS : EXIT_FAILURE );
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