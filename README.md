# arduino-gprs-sim
Arduino lib for gsm modules (works with sim800 and sim900) 7k+0.3k.

## Testing

Want to unit test on development box (not Arduino). Looked into numerous
solutions. Liked approach of: https://github.com/IronSavior/dsm2_tx.

Uses his Makefile which compiles using system (not Arduino) compiler, tested on MacOS.

```sh
make
./runtests
```

### Resources:

http://stackoverflow.com/questions/780819/how-can-i-unit-test-arduino-code

https://github.com/mvgrimes/smartthings-super-sprinker

