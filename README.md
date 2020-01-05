# arduino-gprs-sim
Arduino lib for gsm modules (works with sim800 and sim900) 9k+0.3k.
Can send http requests (only GET), work with SMS is under construction.

[![Build Status](https://travis-ci.org/artemdudkin/arduino-gprs-sim.svg?branch=master)](https://travis-ci.org/artemdudkin/arduino-gprs-sim)

## Installation

Download ZIP from GitHub and import it at Arduino IDE.
You should have [NeoSWSerial](https://github.com/SlashDevin/NeoSWSerial) too.

## Usage

```cpp

#include "NeoSWSerial.h"
#include "GprsSim.h"

#define BUF_SIZE 100
byte buf[BUF_SIZE]; // buffer for AT commands and module answers

char host[] = "example.com";
int port = 8080;
char url[] = "/log?source=xyz&data=";
char data[] = "123";
char *urls[] = {url, data};

GprsSim gs(5, 6, buf, BUF_SIZE); //will use pins 5 and 6 as RX and TX (with NeoSWSerial)
                                 //and buf as shared buffer to read data from gprs module 

gs.start();

gs.send(host, port, urls, 2);

Serial.println((char*)buf); // print HTTP answer, something like
                            //  
                            //  HTTP/1.1 200 OK
                            //  Date: Thu, 04 Jan 2018 10:47:01 GMT
                            //  Connection: close
                            //  Content-Length: 29
                            //  
                            //  {"resultCode":0,"count":6996}
                            //  

```
Also there are ./example folder with schematics and sketch for ds18b20 temperature sensor sending measurements by sim800l. 

## Tests

Want to run unit tests on development box (not Arduino), and it is possible - thanks to [mvgrimes](https://github.com/mvgrimes/smartthings-super-sprinker) and [IronSavior](https://github.com/IronSavior/dsm2_tx).

```sh
# linux
make
./runtests

# win 10 + mingw-w64
mingw32-make
runtests.exe
```
Resources:

http://stackoverflow.com/questions/780819/how-can-i-unit-test-arduino-code

