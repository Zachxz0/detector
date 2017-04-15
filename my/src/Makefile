SUB_DIRS=zoson
export CC INCLUDE BUILD_DIR APP BIN_DIR

all:$(SUB_DIRS) $(BIN_DIR)/main
$(SUB_DIRS):ECHO
	$(MAKE) -C $@

$(BIN_DIR)/main:$(BIN_DIR)/main.o $(DEPEND_OBJS)
	$(CC) $^ -o $@ -lpthread
$(BIN_DIR)/main.o: main.cpp
	$(CC) -c $^ -o $@ 

ECHO:
	@echo $(SUB_DIRS) begin complie
clean:
	@$(RM) $(BUILD_DIR)/*.o
	$rm -rf $(BIN_DIR)