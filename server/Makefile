CXX = g++
CPPFLAGS = -w -std=c++17
LIBS = -lSDL2 -lSDL2_net

SOURCES=Main.cpp Network.cpp
OBJECTS=Main.o Network.o
EXECUTABLE=server

all: 
	$(CXX) -c $(CPPFLAGS) $(LIBS) $(SOURCES) 
	$(CXX) $(LIBS)  $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE)