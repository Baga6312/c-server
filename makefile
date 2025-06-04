# Makefile for CMake-based project
BUILD_DIR := build
EXECUTABLE := main

all: configure build

configure:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake ..

build:
	@cd $(BUILD_DIR) && make
	@cp $(BUILD_DIR)/$(EXECUTABLE) .

clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)

run: all
	./$(EXECUTABLE)

.PHONY: all configure build clean run
