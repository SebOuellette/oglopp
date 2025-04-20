LIB_NAME := oglopp

GLAD_HEADER := gl.h
GLAD_OBJ := gl.o
GLAD_LIBA := libglad.a
GLAD_LIBSO := libglad.so
LIBA := lib$(LIB_NAME).a
LIBSO := lib$(LIB_NAME).so
SONAME_ABI := 1
SONAME_VER := $(SONAME_ABI).0.0

INSTALL_DIR := /usr/
EXAMPLE_DIR := Examples/
SOURCE_DIR := Sources/
BUILD_DIR := build/
INCLUDE_DIR := Headers/
FULLINC_DIR := $(INCLUDE_DIR)$(LIB_NAME)

GLAD_DIR := glad/
GLAD_LIBDIR := $(GLAD_DIR)lib/
GLAD_INCDIR := $(GLAD_DIR)include/glad/
GLAD_PATH := $(GLAD_INCDIR)$(GLAD_HEADER)
GLADCOPY_DIR := $(FULLINC_DIR)/$(GLAD_DIR)
GLADCOPY_PATH := $(GLADCOPY_DIR)$(GLAD_HEADER)

EXAMPLE_FILES := $(wildcard $(EXAMPLE_DIR)*.cpp)
SOURCE_FILES := $(wildcard $(SOURCE_DIR)*.cpp)


INSTALL_IPATH := $(INSTALL_DIR)include/$(LIB_NAME)/
INSTALL_LPATH := $(INSTALL_DIR)lib/

EXAMPLE_OBJECTS := $(patsubst $(EXAMPLE_DIR)%.cpp,$(BUILD_DIR)%.oex, $(EXAMPLE_FILES))
SOURCE_OBJECTS := $(patsubst $(SOURCE_DIR)%.cpp,$(BUILD_DIR)%.o, $(SOURCE_FILES)) 
EXAMPLE_EXECS := $(patsubst $(EXAMPLE_DIR)%.cpp,$(BUILD_DIR)%, $(EXAMPLE_FILES))

GLAD_LIBO_BIN := $(BUILD_DIR)$(GLAD_OBJ)
GLAD_LIB_ACTIVE := $(GLAD_LIBA_BIN)
LIBA_BIN := $(BUILD_DIR)$(LIBA)
LIBSO_BIN := $(BUILD_DIR)$(LIBSO).$(SONAME_VER)

CXX = g++
#-ggdb
SO_COPTS := -fPIC
SO_LOPTS := -ldl -shared
IOPTS := -g3 -O0 -Wall -I$(INCLUDE_DIR) -I../usr/include/
LOPTS := -Wl,--no-as-needed -lglfw -lpthread
#-lglfw -lGL -lX11 -lpthread -lXrandr -lXi 
#-lglad
#-lassimp ; pacman -S assimp

all: libso

.PHONY:

.PHONY: help
help:
	@echo -e \
	"=== Building ===\n"\
	"make\t\tBuild 'libso' when no arguments are specified.\n"\
	"make liba\tBuild Oglopp as a static library -> '$(LIBA)'.\n"\
	"make libso\tBuild Oglopp as a shared object library -> '$(LIBSO)'.\n"\
	"make examples\tBuild the set of example applications into the '$(BUILD_DIR)' directory.\n"\
	"\t\t \\-Requires first running 'make install'.\n"\
	"make glad\tSetup, download, and compile glad to create '$(GLAD_OBJ)'.\n"\
	"\n=== Installation ===\n"\
	"make install\tInstall Oglopp headers and library (.a & .so) to '$(INSTALL_IPATH)' and '$(INSTALL_LPATH)'.\n"\
	"make uninstall\tUninstall Oglopp headers and library from '$(INSTALL_IPATH)' and '$(INSTALL_LPATH)'.\n"\
	"make strip\tStrip the library binaries to minimize file size before installation.\n"\
	"\n=== Maintenance ===\n"\
	"make help\tDisplay this help menu.\n"\
	"make cleanall\tRemove glad and compiled project executables.\n"\
	"make clean\tRemove compiled project executables, such as libraries (.a & .so), .o files, and examples.\n"\
	"make cleanglad\tRemove glad to be redownloaded."

.PHONY: liba
liba: $(BUILD_DIR) $(LIBA_BIN)

.PHONY: libso
libso: setupPIC $(BUILD_DIR) $(LIBSO_BIN)

.PHONY: setupPIC
setupPIC:
	$(eval IOPTS += $(SO_COPTS))
	$(eval LOPTS += $(SO_LOPTS))
	$(eval GLAD_LIB_ACTIVE = $(GLAD_LIBSO_BIN))

.PHONY: examples
examples: $(BUILD_DIR) $(EXAMPLE_EXECS)

.PHONY: glad
glad: $(GLAD_LIBO_BIN)

#========= INSTALLATION =================#
.PHONY: install
install: strip $(LIBSO_BIN)
	-cp $(LIBA_BIN) $(INSTALL_LPATH).
	-cp $(LIBSO_BIN) $(INSTALL_LPATH).
	-ln -sf $(INSTALL_LPATH)$(LIBSO).$(SONAME_VER) $(INSTALL_LPATH)$(LIBSO).$(SONAME_ABI)
	-ln -sf $(INSTALL_LPATH)$(LIBSO).$(SONAME_ABI) $(INSTALL_LPATH)$(LIBSO)
	-cp -r $(INCLUDE_DIR)* $(INSTALL_DIR)include/.

.PHONY: uninstall
uninstall:
	-rm -r $(INSTALL_IPATH)
	-rm $(INSTALL_DIR)include/$(LIB_NAME).h
	-rm $(INSTALL_LPATH)$(LIBA)
	-rm $(INSTALL_LPATH)$(LIBSO)
	-rm $(INSTALL_LPATH)$(LIBSO).$(SONAME_VER)
	-rm $(INSTALL_LPATH)$(LIBSO).$(SONAME_ABI)

# Strip executables to minimize filesize. Used before installation. 
.PHONY: strip
strip: $(LIBSO_BIN)
	-strip $(LIBA_BIN) $(LIBSO_BIN)

#========= LIBRARIES ====================#
# Build static library (.a)
$(LIBA_BIN): $(SOURCE_OBJECTS) $(GLAD_LIBO_BIN)
	ar rcs $@ $^

# Build Shared Library (.so)
$(LIBSO_BIN): $(SOURCE_OBJECTS) $(GLAD_LIBO_BIN)
	$(CXX) -Wl,-soname,$(LIBSO).$(SONAME_ABI) $^ -o $@ $(LOPTS)

#========= EXAMPLES =====================#
# Link examples
$(BUILD_DIR)%: $(BUILD_DIR)%.oex
	$(CXX) $^ -o $@ -L$(INSTALL_LPATH) $(LOPTS) -loglopp 

# Build example mains
$(BUILD_DIR)%.oex: $(EXAMPLE_DIR)%.cpp
	$(CXX) $(IOPTS) -c $^ -o $@


#========= GENERAL SOURCE CODE ==========#
# Build general source code 
$(BUILD_DIR)%.o: $(SOURCE_DIR)%.cpp
	$(CXX) $(IOPTS) -c $(firstword $^) -o $@


#========= GLAD =========================#
# Create glad resources
$(GLAD_PATH):
	mkdir -p $(GLAD_DIR)
	glad --api gl:core=4.6 --out-path $(GLAD_DIR) c
#	glad --api gl=4.6 --out-path $(GLAD_DIR) --generator c		

$(GLAD_LIBO_BIN): $(GLADCOPY_PATH)
	gcc -c $(GLAD_DIR)src/gl.c -I$(FULLINC_DIR) $(IOPTS) -o $@

$(GLADCOPY_PATH): $(GLAD_PATH)
	mkdir -p $(GLADCOPY_DIR)
	cp $(GLAD_PATH) $@

.PRECIOUS: $(BUILD_DIR)
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

#========= CLEAN ========================#
.PHONY: cleanall
cleanall: clean cleanglad

.PHONY: clean
clean:
	-rm ./$(BUILD_DIR)*.o ./$(BUILD_DIR)*.oex ./$(BUILD_DIR)*.a ./$(BUILD_DIR)*.so $(EXAMPLE_EXECS)

.PHONY: cleanglad
cleanglad:
	-rm -r glad
