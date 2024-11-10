CXX = g++
#-ggdb
IOPTS := -g3 -O0 -Wall -I../usr/include/
SO_COPTS := -fPIC -shared
SO_LOPTS := -ldl
LOPTS := -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -lglad

LIB_NAME := oglopp

EXAMPLE_DIR := Examples/
SOURCE_DIR := Sources/
BUILD_DIR := build/
EXAMPLE_FILES := $(wildcard $(EXAMPLE_DIR)*.cpp)
SOURCE_FILES := $(wildcard $(SOURCE_DIR)*.cpp)

EXAMPLE_OBJECTS := $(patsubst $(EXAMPLE_DIR)%.cpp,$(BUILD_DIR)%.oex, $(EXAMPLE_FILES))
SOURCE_OBJECTS := $(patsubst $(SOURCE_DIR)%.cpp,$(BUILD_DIR)%.o, $(SOURCE_FILES))
EXAMPLE_EXECS := $(patsubst $(EXAMPLE_DIR)%.cpp,$(BUILD_DIR)%, $(EXAMPLE_FILES))

all: liba examples

.PHONY: liba
liba: $(BUILD_DIR) $(SOURCE_OBJECTS)
	ar rcs $(BUILD_DIR)$(LIB_NAME).a $(SOURCE_OBJECTS)

.PHONY: libso
libso: setupPIC $(SOURCE_OBJECTS)
	$(CXX) $(SOURCE_OBJECTS) $(SO_LOPTS) -o $(BUILD_DIR)$(LIB_NAME).so $(LOPTS) 

setupPIC:
	$(eval IOPTS += $(SO_COPTS))

.PHONY: examples
examples: $(BUILD_DIR) $(EXAMPLE_EXECS)

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

.PRECIOUS: $(BUILD_DIR)
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

.PHONY: clean
clean:
	-rm ./$(BUILD_DIR)*.o ./$(BUILD_DIR)*.oex ./$(BUILD_DIR)*.a ./$(BUILD_DIR)*.so $(EXAMPLE_EXECS)
