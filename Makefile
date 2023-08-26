CXX = g++

IOPTS :=  -ggdb -Wall -I/network/Programming/GLAD/include
LOPTS := -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl 
OBJECT_FILES := $(patsubst Sources/%.cpp,%.o,$(wildcard Sources/*.cpp))

all: main

main: $(OBJECT_FILES) gl.o 
	$(CXX) $(IOPTS) $(OBJECT_FILES) gl.o -o main $(LOPTS) 

%.o: Sources/%.cpp
	$(CXX) $(IOPTS) -c Sources/$*.cpp 

gl.o: /network/Programming/GLAD/src/gl.c
	$(CXX) $(IOPTS) -c /network/Programming/GLAD/src/gl.c


clean:
	rm -f main *.o
