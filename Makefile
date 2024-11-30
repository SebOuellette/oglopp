LIB_NAME := oglopp

GLAD_HEADER := gl.h

INSTALL_DIR := ../usr/
EXAMPLE_DIR := Examples/
SOURCE_DIR := Sources/
BUILD_DIR := build/
INCLUDE_DIR := Headers/
FULLINC_DIR := $(INCLUDE_DIR)$(LIB_NAME)

GLAD_DIR := glad/
GLAD_LIBDIR := $(GLAD_DIR)lib/
GLAD_PATH := $(GLAD_DIR)include/glad/$(GLAD_HEADER)
GLADCOPY_DIR := $(FULLINC_DIR)/$(GLAD_DIR)
GLADCOPY_PATH := $(GLADCOPY_DIR)$(GLAD_HEADER)

EXAMPLE_FILES := $(wildcard $(EXAMPLE_DIR)*.cpp)
SOURCE_FILES := $(wildcard $(SOURCE_DIR)*.cpp)


INSTALL_IPATH := $(INSTALL_DIR)include/$(LIB_NAME)/
INSTALL_LPATH := $(INSTALL_DIR)lib/

EXAMPLE_OBJECTS := $(patsubst $(EXAMPLE_DIR)%.cpp,$(BUILD_DIR)%.oex, $(EXAMPLE_FILES))
SOURCE_OBJECTS := $(patsubst $(SOURCE_DIR)%.cpp,$(BUILD_DIR)%.o, $(SOURCE_FILES))
EXAMPLE_EXECS := $(patsubst $(EXAMPLE_DIR)%.cpp,$(BUILD_DIR)%, $(EXAMPLE_FILES))

GLAD_LIB_BIN := $(GLAD_LIBDIR)libglad.a
LIB_BIN := $(BUILD_DIR)lib$(LIB_NAME).a

CXX = g++
#-ggdb
SO_COPTS := -fPIC -shared
SO_LOPTS := -ldl
IOPTS := -g3 -O0 -Wall -I$(INCLUDE_DIR) -I../usr/include/
LOPTS := -L$(GLAD_LIBDIR) -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -lglad
#-lassimp ; pacman -S assimp

all: liba examples

.PHONY:

.PHONY: liba
liba: $(GLADCOPY_PATH) $(BUILD_DIR) $(LIB_BIN)

#.PHONY: libso
#libso: setupPIC $(SOURCE_OBJECTS)
#	$(CXX) $(SOURCE_OBJECTS) $(SO_LOPTS) -o $(BUILD_DIR)$(LIB_NAME).so $(LOPTS)
#setupPIC:
#	$(eval IOPTS += $(SO_COPTS))

.PHONY: examples
examples: $(GLADCOPY_PATH) $(BUILD_DIR) $(EXAMPLE_EXECS)

.PHONY: glad
glad: $(GLADCOPY_PATH) $(GLAD_LIB_BIN)

.PHONY: install
install: $(LIB_BIN)
	-cp $(LIB_BIN) $(INSTALL_LPATH).
	-cp $(GLAD_LIB_BIN) $(INSTALL_LPATH).
	-cp -r $(INCLUDE_DIR)* $(INSTALL_DIR)include/.

.PHONY: uninstall
uninstall:
	-rm -r $(INSTALL_IPATH)
	-rm $(INSTALL_DIR)include/$(LIB_NAME).h
	-rm $(LIB_BIN)
	-rm $(GLAD_LIB_BIN)

# Build static library
$(LIB_BIN): $(SOURCE_OBJECTS)
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
	glad --api gl:core=4.6,gles2 --out-path $(GLAD_DIR) c

$(GLAD_LIB_BIN): $(GLADCOPY_PATH)
	mkdir -p $(dir $@)
	gcc -c $(GLAD_DIR)src/gl.c -I$(FULLINC_DIR) -o $@

$(GLADCOPY_PATH): $(GLAD_PATH)
	mkdir -p $(GLADCOPY_DIR)
	cp $(GLAD_PATH) $(GLADCOPY_PATH)

.PRECIOUS: $(BUILD_DIR)
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

.PHONY: clean
clean:
	-rm ./$(BUILD_DIR)*.o ./$(BUILD_DIR)*.oex ./$(BUILD_DIR)*.a ./$(BUILD_DIR)*.so $(EXAMPLE_EXECS)

.PHONY: cleanglad
cleanglad:
	-rm -r glad
