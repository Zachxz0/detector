SUBDIRS=proto utils net detector commu app
export CC INCLUDE BUILD_DIR

all:$(SUBDIRS)
$(SUBDIRS):ECHO
	$(MAKE) -C $@
ECHO: 
	@echo $(SUBDIRS) begin complie
CLEAN:
	@$(RM) $(BUILD_DIR)/*.o
	$rm -rf $(BIN_DIR)