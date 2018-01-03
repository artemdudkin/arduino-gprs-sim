# arduino-gprs-sim
Arduino lib for gsm modules (works with sim800 and sim900) 9k+0.3k.

Can send http requests (only GET), work with SMS is under construction.

## Installation

Copy folder GprsSim to Arduino/library folder.
You should have [NeoSWSerial](https://github.com/SlashDevin/NeoSWSerial) also.

## Usage

```cpp

#include "NeoSWSerial.h"
#include "GprsSim.h"

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

Want to run unit tests on development box (not Arduino), and it is possible - thanks to [smartthings-super-sprinker](https://github.com/mvgrimes/smartthings-super-sprinker) and [dsm2_tx](https://github.com/IronSavior/dsm2_tx).

(tested on MacOS)

```sh
make
./runtests
```
Resources:

http://stackoverflow.com/questions/780819/how-can-i-unit-test-arduino-code

