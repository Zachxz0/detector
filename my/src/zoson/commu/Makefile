all:$(BUILD_DIR)/commu.o $(BUILD_DIR)/reactor.o $(BUILD_DIR)/detect_reactor.o 
$(BUILD_DIR)/commu.o:commu.cpp
	$(CC) $(CPP_FLAGS) $^ -o $@ $(INCLUDE) $(DEFINE)
$(BUILD_DIR)/reactor.o:reactor.cpp
	$(CC) $(CPP_FLAGS) $^ -o $@ $(INCLUDE) $(DEFINE)
$(BUILD_DIR)/detect_reactor.o:detect_reactor.cpp
	$(CC) $(CPP_FLAGS) $^ -o $@ $(INCLUDE) $(DEFINE)
.Phony:clean
clean:
	rm -f *.o