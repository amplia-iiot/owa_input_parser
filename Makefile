#Compiler
ARM_CC = /opt/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++

#STRIP
ARM_STRIP = /opt/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-strip

#Compiler flags for the test program
#CFLAGS = -Wall -g -ggdb
CFLAGS = -Wall -mthumb -mthumb-interwork -D_REENTRANT

#OBJECTS
OBJECT = owa_inputs_adapter

#Source Files
SOURCE = ./*.cpp

#LIBS to include
LIBS = -ldl
LIBS += -lpthread


all: build

build:
	$(ARM_CC) $(CFLAGS) -o $(OBJECT) $(SOURCE) $(LIBS)
	$(ARM_STRIP) $(OBJECT)
clean:
	rm owa_inputs_adapter
