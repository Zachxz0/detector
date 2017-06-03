#env path#
DEFINE = -D USE_OPENCV -D DEBUG
LD:=ld
CC:= g++
JC:=javac
CPP_FLAGS:=-fPIC -c
CURR_DIR=$(shell pwd)
INCLUDE = -I$(CURR_DIR)/include/zoson -I$(CURR_DIR)/../caffe/include -I$(CURR_DIR)/include/zoson/proto -I/usr/include/ -I/usr/local/cuda-8.0/include -I$(CURR_DIR)/../caffe/build/src -I/usr/lib/jvm/java-8-oracle/include/linux -I/usr/lib/jvm/java-8-openjdk-amd64/include -I/usr/lib/jvm/java-8-openjdk-amd64/include -I/usr/lib/jvm/java-8-openjdk-amd64/include/linux
BUILD_DIR = build
BIN_DIR = bin
SDK_DIR = sdk
JAVA_DIR = java
TEST = $(BIN_DIR)/test
EXEC = $(BIN_DIR)/main
FFMPEG_LIBS = -lavutil -lavcodec -lavformat -lswscale -lpostproc  -lswresample 

LIBS = -lprotobuf -lcaffe  -lboost_thread -lboost_system -lopencv_core -lopencv_imgproc -lopencv_highgui -lgflags -lopencv_ml -lc -lstdc++ -lglog $(FFMPEG_LIBS)
LIBS_DIR = -L/usr/local/lib -L$(CURR_DIR)/../caffe/build/lib -L/usr/lib/x86_64-linux-gnu

LIB_DIR = lib
LIB_NAME = detector

##proto env
PROJECT=zoson
PROTO_NAME = $(PROJECT)
PROTO_PATH = include/$(PROJECT)/proto
PROTO_S = $(CURR_DIR)/src/$(PROJECT)/proto/$(PROTO_NAME).proto
PROTO_H = $(PROTO_PATH)/$(PROTO_NAME).pb.h 
PROTO_C = $(PROTO_PATH)/$(PROTO_NAME).pb.cc
PROTO = $(PROTO_H) 
PROTO_C_SRC = src/$(PROJECT)/proto/$(PROTO_NAME).pb.cpp


# All of the directories containing code.
SRC_DIRS := $(shell find * -type d -exec bash -c "find {} -maxdepth 1 \
	\( -name '*.cpp' -o -name '*.proto' -o -name '*.java' \) | grep -q ." \; -print)
ALL_BUILD_DIRS := $(sort $(BUILD_DIR) $(addprefix $(BUILD_DIR)/, $(SRC_DIRS)) \
	 $(addprefix $(BUILD_DIR)/, $(LIB_DIR)) $(addprefix $(BUILD_DIR)/java/, $(LIB_DIR)))

CXX_SRCS := $(shell find src/$(PROJECT) -name "*.cpp")
CXX_TEST_SRCS := $(shell find test/ -name "*.cpp")
CXX_TOOL_SRCS := $(shell find src/tools/ -name "*.cpp")
TARGET := $(addprefix $(BUILD_DIR)/, ${CXX_SRCS:.cpp=.o})
TEST_TARGET := $(addprefix $(BUILD_DIR)/, ${CXX_TEST_SRCS:.cpp=.o})
TOOL_TARGET := $(addprefix $(BUILD_DIR)/, ${CXX_TOOL_SRCS:.cpp=.o})

all : $(PROTO) $(ALL_BUILD_DIRS) $(TARGET) $(BUILD_DIR)/$(LIB_DIR)/lib$(LIB_NAME).so $(TEST) $(EXEC)
#compile
$(ALL_BUILD_DIRS):
	@ mkdir -p $@

$(BUILD_DIR)/%.o : %.cpp
	@echo $(CC) $< 
	@$(CC) $(CPP_FLAGS) $< -o $@ $(INCLUDE) $(DEFINE) 

$(TEST):$(TEST_TARGET) $(TARGET) 
	@echo $(CC) $@
	@$(CC) $^ -o $@  $(LIBS_DIR) $(LIBS) 

$(EXEC):$(TOOL_TARGET) $(TARGET)
	@echo $(CC) $@
	@$(CC) $^ -o $@  $(LIBS_DIR) $(LIBS) 

$(BUILD_DIR)/$(LIB_DIR)/lib$(LIB_NAME).so:$(TARGET)
	@echo $(LD) $@
	@$(CC) -shared -o $@ $^ $(LIBS_DIR) $(LIBS)

#run test
runtest:
	$(BIN_DIR)/test
#install
install:
	sudo cp $(LIB_SO) /usr/local/lib

##proto
$(PROTO):$(PROTO_S)
	@echo protoc $^
	@protoc -I=$(CURR_DIR)/src/$(PROJECT)/proto --cpp_out=$(CURR_DIR)/include/$(PROJECT)/proto $^
	@cp $(PROTO_C) $(PROTO_C_SRC)

##caffe
caffe:ECHO_CAFFE COMPILE_CAFFE INSTALL_CAFFE
COMPILE_CAFFE:
	@$(MAKE) -C ../caffe -j8
ECHO_CAFFE:
	@echo caffe begin compile and install
CAFFE_LIB = ../caffe/build/lib/libcaffe.so.1.0.0-rc3
INSTALL_CAFFE:$(CAFFE_LIB)
	sudo cp $^ /usr/local/lib/libcaffe.so
	cp $^ $(CURR_DIR)/lib/libcaffe.so

##java todo
JAVA_SRCS := $(shell find java/src -name "*.java")
JNI_SRCS := $(shell find java/jni -name "*.cpp")
JNI_TARGET = $(addprefix $(BUILD_DIR)/, ${JNI_SRCS:.cpp=.o})
JAVA_TARGET = $(addprefix $(BUILD_DIR)/, ${JAVA_SRCS:.java=.class})
JAVA_JAR_PATH:=java/lib
JAVA_LIB_SET := $(shell for i in java/lib/*.jar;do LIB_SET=$$i:$$LIB_SET ;done;echo $$LIB_SET)
java_proto:
	protoc -I=./src/$(PROJECT)/proto --java_out=./java/src/ ./src/$(PROJECT)/proto/$(PROJECT).proto
	protoc -I=./src/$(PROJECT)/proto --java_out=./java/src/ ./src/$(PROJECT)/proto/sdk.proto

java:java_src $(JNI_TARGET) $(BUILD_DIR)/java/$(LIB_DIR)/lib$(LIB_NAME)_jni.so $(BUILD_DIR)/java/$(LIB_DIR)/detect.jar

javah:
	javah -d java/jni -classpath $(BUILD_DIR)/java/src com.zoson.detection.jni.JniInterface

$(BUILD_DIR)/java/$(LIB_DIR)/lib$(LIB_NAME)_jni.so:$(JNI_TARGET)
	@echo $(LD) $@
	@$(CC) -shared -o $@ $^ $(LIBS_DIR) $(LIBS) -L/usr/local/lib/ -ldetector -lcaffe

java_src:$(JAVA_SRCS)
	@echo $(JC) $^
	@$(JC) -d $(BUILD_DIR)/java/src -classpath $(JAVA_LIB_SET) $^

$(BUILD_DIR)/java/$(LIB_DIR)/detect.jar:
	@echo jar $@
	@jar cvf $@ $(BUILD_DIR)/java/src

##qt-proto
qt-proto:$(PROTO_S)
	protoc -I=$(CURR_DIR)/src/zoson/proto --cpp_out=$(CURR_DIR)/../vision-qt $^

#make sdk
sdk:
	@echo "making sdk"
	@rm -rf $(SDK_DIR)
	@mkdir $(SDK_DIR)
	@mkdir $(SDK_DIR)/lib
	@mkdir $(SDK_DIR)/jar
	@mkdir $(SDK_DIR)/model
	@cp ./sdktest/* $(SDK_DIR)/
	@cp $(BUILD_DIR)/lib/* $(SDK_DIR)/lib/
	@cp $(BUILD_DIR)/java/lib/*.jar $(SDK_DIR)/jar/
	@cp $(BUILD_DIR)/java/lib/*.so $(SDK_DIR)/lib
	@cp -rf $(CURR_DIR)/data $(SDK_DIR)/data
	@cp -rf $(CURR_DIR)/config $(SDK_DIR)/config
	@echo "finish package"

.PHONY : all clean java
clean :
	rm -rf  $(BUILD_DIR)/*
	rm include/$(PROJECT)/proto/*
	rm $(BIN_DIR)/*
	rm $(PROTO_C_SRC)