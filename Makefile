SOURCES = test/runtests.cpp \
          test/assert.cpp \
          test/mock/fake_serial.cpp \
          test/mock/Arduino.cpp \
          test/mock/SoftwareSerial.cpp \
          test/mock/pgmspace.cpp \
          lib/GprsSim.cpp 

OBJECTS := $(addsuffix .o, $(addprefix .build/, $(basename $(SOURCES))))
DEPFILES := $(subst .o,.dep, $(subst .build/,.deps/, $(OBJECTS)))
TESTCPPFLAGS = -D_TEST_ -Ilib -Itest -Iarduino
CPPDEPFLAGS = -MMD -MP -MF .deps/$(basename $<).dep
RUNTESTS := $(if $(COMSPEC), runtests.exe, runtests)

all: runtests

.build/%.o: %.cpp
	mkdir -p .deps/$(dir $<)
	mkdir -p .build/$(dir $<)
	$(COMPILE.cpp) $(TESTCPPFLAGS) $(CPPDEPFLAGS) -o $@ $<

runtests: $(OBJECTS)
	$(CC) $(OBJECTS) -lstdc++ -o $@

clean:
	@rm -rf .deps/ .build/ $(RUNTESTS)

-include $(DEPFILES)
