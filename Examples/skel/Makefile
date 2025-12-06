CURRENT_DIR := $(shell pwd)
CXX := g++
EXE := oglopp-app

HEADERS_DIR := include/
SOURCE_DIR := src/
BUILD_DIR := build/

CXX_OPTIONS := -Wall
LIBRARIES := -Wl,-rpath,/usr/local/lib -lglfw -loglopp

LINK_OPTIONS 	:= $(LIBRARIES)
COMPILE_OPTIONS	:= -I$(HEADERS_DIR) -g3 -O0

SOURCE_FILES := $(wildcard $(SOURCE_DIR)*.cpp) $(wildcard $(SOURCE_DIR)*/*.cpp)
OBJECT_FILES := $(patsubst $(SOURCE_DIR)%.cpp,$(BUILD_DIR)%.o,$(SOURCE_FILES))

.PHONY: all
all: $(EXE)

$(EXE): $(OBJECT_FILES)
	$(CXX) $(CXX_OPTIONS) $^ -o $@ $(LINK_OPTIONS)

$(BUILD_DIR)%.o: $(SOURCE_DIR)%.cpp
	-mkdir -p $(dir $@)
	$(CXX) $(CXX_OPTIONS) -c $(COMPILE_OPTIONS) $^ -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR) $(EXE)

.PHONY: rebuild
rebuild: clean all

.PHONY: help
help:
	@echo make ---------- Compile and link
	@echo make clean ---- Remove all object files
	@echo make rebuild -- Same as \`"make clean ; make"\` - Cleaning is required to \"rebuild\" header files
	@echo make help ----- Show this help menu
