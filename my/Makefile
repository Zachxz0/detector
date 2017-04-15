#env path#
DEFINE = -D USE_OPENCV -D DEBUG
LD:=ld
CC:= g++
APP = vcnn
CPP_FLAGS:=-fPIC -c
CURR_DIR=$(shell pwd)
THIRD_PARTY = $(CURR_DIR)/thirdparty
INCLUDE = -I$(CURR_DIR)/include/zoson -I$(CURR_DIR)/../caffe/include -I$(CURR_DIR)/include/zoson/proto -I/usr/include/ -I/usr/local/cuda-8.0/include -I$(CURR_DIR)/../caffe/build/src
BUILD_DIR = $(CURR_DIR)/build
BIN_DIR = $(CURR_DIR)/bin
SUB_DIR = src test

FFMPEG_LIBS = -lavutil -lavcodec -lavformat -lswscale -lpostproc  -lswresample 

LIBS = -lprotobuf -lcaffe  -lboost_thread -lboost_system -lopencv_core -lopencv_imgproc -lopencv_highgui -lgflags -lopencv_ml -lc -lstdc++ -lglog $(FFMPEG_LIBS)
LIBS_DIR = -L/usr/local/lib -L$(CURR_DIR)/../caffe/build/lib -L/usr/lib/x86_64-linux-gnu
DEPEND_OBJS=$(shell ls $(BUILD_DIR)/*.o)
LIB_SO = $(CURR_DIR)/lib/libdetector.so

PROTO_NAME = zoson
PROTO_PATH = $(CURR_DIR)/include/zoson/proto
PROTO_S = $(CURR_DIR)/src/zoson/proto/$(PROTO_NAME).proto
PROTO_H = $(PROTO_PATH)/$(PROTO_NAME).pb.h 
PROTO_C = $(PROTO_PATH)/$(PROTO_NAME).pb.cc
PROTO = $(PROTO_H) 
PROTO_C_SRC = $(CURR_DIR)/src/zoson/proto/$(PROTO_NAME).pb.cc
PROTOC = "/home/zoson/proto/bin/protoc"

export CC INCLUDE BUILD_DIR BIN_DIR APP THIRD_PARTY LIBS_DIR LIBS DEFINE DEPEND_OBJS CPP_FLAGS
#script#
all:CHECK_DIR $(PROTO) $(PROTO_C_SRC) $(SUB_DIR) $(LIB_SO)
CHECK_DIR:
	mkdir -p $(BIN_DIR) $(BUILD_DIR)

$(SUB_DIR):ECHO
	$(MAKE) -C $@
ECHO:
	@echo $(SUB_DIR) begin complie

$(LIB_SO):
	$(CC) -shared -o $(LIB_SO) $(DEPEND_OBJS) $(LIBS_DIR) $(LIBS)


install:$(LIB_SO)
	sudo cp $(LIB_SO) /usr/local/lib


##proto
proto:$(PROTO)
$(PROTO):$(PROTO_S)
	protoc -I=$(CURR_DIR)/src/zoson/proto --cpp_out=$(CURR_DIR)/include/zoson/proto $^
$(PROTO_C_SRC):$(PROTO_C)
	cp $^ $@

##caffe
caffe:ECHO_CAFFE COMPILE_CAFFE INSTALL_CAFFE
COMPILE_CAFFE:
	$(MAKE) -C ../caffe -j8
ECHO_CAFFE:
	@echo caffe begin compile and install

CAFFE_LIB = ../caffe/build/lib/libcaffe.so.1.0.0-rc3
INSTALL_CAFFE:$(CAFFE_LIB)
	sudo cp $^ /usr/local/lib/libcaffe.so
	cp $^ $(CURR_DIR)/lib/libcaffe.so

##java
java:ECHO_JAVA
	$(MAKE) -C $@
ECHO_JAVA:
	@echo java begin complie

.Phony:clean run_test
clean:java_clean
	rm $(BUILD_DIR)/*.o $(BIN_DIR)/* $(CURR_DIR)/src/zoson/module/proto/zoson.pb.cc $(CURR_DIR)/include/zoson/proto/zoson.pb.h $(CURR_DIR)/test/test $(CURR_DIR)/test/test.o
java_clean:
	$(MAKE) -C java clean

##proto for qt
qt-proto:$(PROTO_S)
	protoc -I=$(CURR_DIR)/src/zoson/proto --cpp_out=$(CURR_DIR)/../vision-qt $^
runtest:
	./test/test