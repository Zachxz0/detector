LD:=ld
CPP:=gcc
CURR_DIR=$(shell pwd)
CPPFLAGS:= -c -fPIC
# the libfile name is libDemo.so
LIB=$(CURR_DIR)/../lib/libdetectorJni.so
#the jni.h path
pathlix:=-I/usr/lib/jvm/java-8-oracle/include -I$(CURR_DIR)/../../include/zoson/
pathinclude:=-I/usr/lib/jvm/java-8-oracle/include/linux
objects=$(CURR_DIR)/detector.o


$(LIB):$(objects)
	$(LD) -shared -o $(LIB) $(objects) -L$(CURR_DIR)/../lib/ -lDetector


$(objects):com_zoson_detection_jni_DetectorJni.cpp
	$(CPP) -o $(objects) $(pathlix) $(pathinclude) $(CPPFLAGS)  com_zoson_detection_jni_DetectorJni.cpp


clean:
	rm -f $(objects)