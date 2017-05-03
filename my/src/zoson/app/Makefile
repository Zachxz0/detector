all:$(BUILD_DIR)/dcontext.o
$(BUILD_DIR)/dcontext.o:dcontext.cpp
	$(CC) $(CPP_FLAGS) $^ -o $@ $(INCLUDE) $(DEFINE)
.Phony:clean
clean:
	rm -f *.o