all:$(BUILD_DIR)/proto.o
$(BUILD_DIR)/proto.o:zoson.pb.cc
	$(CC) $(CPP_FLAGS) $^ -o $@ $(INCLUDE)
.Phony:clean
clean:
	rm -f *.o