
#include "mock/arduino.h"
#include "GprsSim.h"

using namespace std;

void millis_test() {
  unsigned long start = millis();
  cout << "millis() test start: " << start << endl;
  while( millis() - start < 10000 ) {
    cout << millis() << endl;
    sleep(1);
  }
  unsigned long end = millis();
  cout << "End of test - duration: " << end - start << "ms" << endl;
}

void delay_test() {
  unsigned long start = millis();
  cout << "delay() test start: " << start << endl;
  while( millis() - start < 10000 ) {
    cout << millis() << endl;
    delay(250);
  }
  unsigned long end = millis();
  cout << "End of test - duration: " << end - start << "ms" << endl;
}

#define XBUF_SIZE 30
byte xbuf[XBUF_SIZE];

void run_tests() {
//  millis_test();
//  delay_test();

  GprsSim g(5, 6, xbuf, XBUF_SIZE);
  g.start();
}


int main(int argc, char **argv){

  cout << "start tests\r\n\r\n";

  initialize_mock_arduino();
  run_tests();
}
