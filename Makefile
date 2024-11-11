LIB_NAME := oglopp

GLAD_HEADER := gl.h

EXAMPLE_DIR := Examples/
SOURCE_DIR := Sources/
BUILD_DIR := build/
GLAD_DIR := glad/
GLAD_PATH := $(GLAD_DIR)include/glad/$(GLAD_HEADER)
EXAMPLE_FILES := $(wildcard $(EXAMPLE_DIR)*.cpp)
SOURCE_FILES := $(wildcard $(SOURCE_DIR)*.cpp)

EXAMPLE_OBJECTS := $(patsubst $(EXAMPLE_DIR)%.cpp,$(BUILD_DIR)%.oex, $(EXAMPLE_FILES))
SOURCE_OBJECTS := $(patsubst $(SOURCE_DIR)%.cpp,$(BUILD_DIR)%.o, $(SOURCE_FILES))
EXAMPLE_EXECS := $(patsubst $(EXAMPLE_DIR)%.cpp,$(BUILD_DIR)%, $(EXAMPLE_FILES))

CXX = g++
#-ggdb
SO_COPTS := -fPIC -shared
SO_LOPTS := -ldl
IOPTS := -g3 -O0 -Wall -I../usr/include/ -Iglad/include
LOPTS := -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -lglad

all: liba examples

.PHONY: 

.PHONY: liba
liba: $(GLAD_PATH) $(BUILD_DIR) $(SOURCE_OBJECTS)
	ar rcs $(BUILD_DIR)$(LIB_NAME).a $(SOURCE_OBJECTS)

.PHONY: libso
libso: setupPIC $(SOURCE_OBJECTS)
	$(CXX) $(SOURCE_OBJECTS) $(SO_LOPTS) -o $(BUILD_DIR)$(LIB_NAME).so $(LOPTS) 

setupPIC:
	$(eval IOPTS += $(SO_COPTS))

.PHONY: examples
examples: $(GLAD_PATH) $(BUILD_DIR) $(EXAMPLE_EXECS)

# Build static library
$(BUILD_DIR)$(LIB_NAME).a: $(SOURCE_OBJECTS)
	ar rcs $@ $^

# Link examples
$(BUILD_DIR)%: $(SOURCE_OBJECTS) $(BUILD_DIR)%.oex
	$(CXX) $^ -o $@ $(LOPTS)

# Build example mains
$(BUILD_DIR)%.oex: $(EXAMPLE_DIR)%.cpp
	$(CXX) $(IOPTS) -c $^ -o $@

# Build general source code
$(BUILD_DIR)%.o: $(SOURCE_DIR)%.cpp
	$(CXX) $(IOPTS) -c $^ -o $@

# Create glad resources
$(GLAD_PATH):
	mkdir -p $(GLAD_DIR)
	glad --api gl:core=3.3,gles2 --out-path $(GLAD_DIR) c

.PRECIOUS: $(BUILD_DIR)
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

.PHONY: clean
clean:
	-rm ./$(BUILD_DIR)*.o ./$(BUILD_DIR)*.oex ./$(BUILD_DIR)*.a ./$(BUILD_DIR)*.so $(EXAMPLE_EXECS)
