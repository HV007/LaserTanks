# Dependencies
Open terminal and run the following commands to install the dependencies:
## Ubuntu
1. `sudo apt install g++`
2. `sudo apt-get install -libsdl2-dev`
3. `sudo apt-get install -libsdl2-ttf-dev`
4. `sudo apt-get install -libsdl2-net-dev`
5. `sudo apt-get install -libsdl2-mixer-dev`
6. `sudo apt-get install npm`

## MacOS
1. `brew install sdl2 sdl2_ttf sdl2_net sdl2_mixer npm`

# Game Instructions
After installing the dependencies. Do the following steps (Only one server needs to be created in a multiplayer game):
## Server
1. We need one server to coordinate between the clients. 
2. Change to server folder: `cd server/`
3. Make the executable: `make`
4. Run the executable: `./server`
5. Enter the total players playing (this can be upto 4)
6. Keep the server open in a seperate terminal window.


## Client Side
All the players should do the following steps:
1. Change to client folder: `cd client/`
2. Make the executable; `make`
3. Run the executable: `./game`
4. Enjoy!

# Game Play
Refer to `LaserTank.pdf` to know about the various rules and features of the game. (Although its quite simple: shoot and kill)

# Resources
1. A very comprehensive and easy-to-use tutorial for SDL: [Lazy Foo' Productions](https://lazyfoo.net/tutorials/SDL/index.php)
2. Maze generation algorithm and implementation: [Dllu/Maze](https://en.wikipedia.org/wiki/User:Dllu/Maze)
3. A tutorial on networking with SDL, a bit long but explains from scratch: [thecplusplusguy](https://www.youtube.com/watch?v=iJfC4-yNnzY)
