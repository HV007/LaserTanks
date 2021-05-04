CXX = g++
CPPFLAGS = -w -std=c++17
LIBS = -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image

SOURCES=Main.cpp Maze.cpp Tank.cpp Texture.cpp Timer.cpp Health.cpp Player.cpp
OBJECTS=Main.o Maze.o Tank.o Texture.o Timer.o Health.o Player.o
EXECUTABLE=game

all: 
	$(CXX) -c $(CPPFLAGS) $(LIBS) $(SOURCES) 
	$(CXX) $(LIBS)  $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE)