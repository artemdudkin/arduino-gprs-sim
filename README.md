# arduino-gprs-sim
Arduino lib for gsm modules (works with sim800 and sim900) 7k+0.3k.

Can send http requests (work with SMS is under construction).

## Installation

Copy folder GprsSim to Arduino/library folder.

## Usage

```cpp

#define BUF_SIZE 100
byte buf[BUF_SIZE];

char host[] = "example.com";
int port = 8080;
char url[] = "/log?source=xyz&data=";
char data[] = "123";
char *urls[] = {url, data};

GprsSim gs(5, 6, buf, BUF_SIZE); //will use pins 5 and 6 as RX and TX (with SoftwareSerial)
                                 //and buf as shared buffer to read data from gprs module 

gs.start();

gs.send(host, port, urls, 2);

```

## Tests

Want to unit test on development box (not Arduino). Looked into numerous
solutions. Liked approach of: https://github.com/IronSavior/dsm2_tx.

Uses his Makefile which compiles using system (not Arduino) compiler, tested on MacOS.

```sh
make
./runtests
```
Resources:

http://stackoverflow.com/questions/780819/how-can-i-unit-test-arduino-code

https://github.com/mvgrimes/smartthings-super-sprinker

