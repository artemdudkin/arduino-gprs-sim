SOURCES = runtests.cpp \
          assert.cpp \
          mock/fake_serial.cpp \
          mock/Arduino.cpp \
          mock/NeoSWSerial.cpp \
          mock/pgmspace.cpp \
          ../src/GprsSim.cpp 

OBJECTS := $(addsuffix .o, $(addprefix .build/, $(basename $(SOURCES))))
TESTCPPFLAGS = -D_TEST_
CPPDEPFLAGS = -MMD -MP -MF .deps/$(basename $<).dep
RUNTESTS := $(if $(COMSPEC), runtests.exe, runtests)

all: runtests

.build/%.o: %.cpp
	mkdir -p .deps/$(dir $<)
	mkdir -p .build/$(dir $<)
	$(COMPILE.cpp) $(TESTCPPFLAGS) $(CPPDEPFLAGS) -Wno-deprecated-declarations -o $@ $<

runtests: $(OBJECTS) 
	$(CC) $(OBJECTS) -lstdc++ -o $@

clean:
	@rm -rf .deps/ .build/ src/ $(RUNTESTS)

