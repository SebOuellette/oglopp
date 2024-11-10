CXX = g++
#-ggdb
IOPTS := -g3 -O0 -Wall -I../usr/include/
SO_COPTS := -fPIC -shared
SO_LOPTS := -ldl
LOPTS := -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -lglad

EXAMPLE_EXEC := example
LIB_NAME := oglopp

EXAMPLE_DIR := Examples/
SOURCE_DIR := Sources/
BUILD_DIR := build/
EXAMPLE_FILES := $(wildcard $(EXAMPLE_DIR)*.cpp)
SOURCE_FILES := $(wildcard $(SOURCE_DIR)*.cpp)

EXAMPLE_OBJECTS := $(patsubst $(EXAMPLE_DIR)%.cpp,$(BUILD_DIR)%.oex, $(EXAMPLE_FILES))
SOURCE_OBJECTS := $(patsubst $(SOURCE_DIR)%.cpp,$(BUILD_DIR)%.o, $(SOURCE_FILES))

all: liba $(EXAMPLE_EXEC)

.PHONY: liba
liba: $(SOURCE_OBJECTS)
	ar rcs $(BUILD_DIR)$(LIB_NAME).a $(SOURCE_OBJECTS)

.PHONY: libso
libso: setupPIC $(SOURCE_OBJECTS)
	$(CXX) $(SOURCE_OBJECTS) $(SO_LOPTS) -o $(BUILD_DIR)$(LIB_NAME).so $(LOPTS) 

setupPIC:
	$(eval IOPTS += $(SO_COPTS))
#$(eval LOPTS += $(SO_LOPTS))

.PHONY: $(EXAMPLE_EXEC)
$(EXAMPLE_EXEC): $(BUILD_DIR) $(SOURCE_OBJECTS) $(EXAMPLE_OBJECTS)
	$(CXX) $(SOURCE_OBJECTS) $(EXAMPLE_OBJECTS) -o $(BUILD_DIR)$(EXAMPLE_EXEC) $(LOPTS)

$(BUILD_DIR)%.o: $(SOURCE_DIR)%.cpp
	$(CXX) $(IOPTS) -c $^ -o $@

$(BUILD_DIR)%.oex: $(EXAMPLE_DIR)%.cpp
	$(CXX) $(IOPTS) -c $^ -o $@

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

.PHONY: clean
clean:
	-rm ./$(BUILD_DIR)*.o ./$(BUILD_DIR)*.oex ./$(BUILD_DIR)*.a ./$(BUILD_DIR)*.so ./$(BUILD_DIR)$(EXAMPLE_EXEC)
