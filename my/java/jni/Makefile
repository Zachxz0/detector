LD:=ld
CPP:=g++
CURR_DIR=$(shell pwd)
CPPFLAGS:= -c -fPIC
# the libfile name is libDemo.so
LIB=$(CURR_DIR)/../lib/libdetectorJni.so
#the jni.h path
pathlix:=-I/usr/lib/jvm/java-8-openjdk-amd64/include -I/usr/lib/jvm/java-8-openjdk-amd64/include/linux -I$(CURR_DIR)/../../include/zoson/ -I$(CURR_DIR)/../../../caffe/.build_release/src/ -I$(CURR_DIR)/../../../caffe/include/
pathinclude:=-I/usr/lib/jvm/java-8-oracle/include/linux
objects=$(CURR_DIR)/detector.o 


$(LIB):$(objects)
	$(CPP) -shared -o $(LIB) $(objects) -L/usr/local/lib/ -ldetector -L/usr/local/lib/ -lcaffe


$(objects):com_zoson_detection_jni_JniInterface.cpp
	$(CPP) -o $(objects) $(pathlix) $(pathinclude) $(CPPFLAGS)  com_zoson_detection_jni_JniInterface.cpp


clean:
	rm -f $(objects)