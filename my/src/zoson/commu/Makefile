all:$(BUILD_DIR)/commu.o
$(BUILD_DIR)/commu.o:commu.cpp
	$(CC) $(CPP_FLAGS) $^ -o $@ $(INCLUDE) $(DEFINE)
.Phony:clean
clean:
	rm -f *.o