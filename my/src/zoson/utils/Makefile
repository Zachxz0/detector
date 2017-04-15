all:$(BUILD_DIR)/common.o
$(BUILD_DIR)/common.o:common.cpp
	$(CC) $(CPP_FLAGS) $^ -o $@ $(INCLUDE) $(DEFINE)
.Phony:clean
clean:
	rm -f *.o