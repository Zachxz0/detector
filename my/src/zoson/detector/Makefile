all:$(BUILD_DIR)/detector.o
$(BUILD_DIR)/detector.o:detector.cpp
	$(CC) $(CPP_FLAGS) $^ -o $@ $(INCLUDE) $(DEFINE)
.Phony:clean
clean:
	rm -f *.o