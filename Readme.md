
Installation Instructions-
1) Ubuntu-> Open terminal and run the following commands-
# Install g++
sudo apt install g++
# Installing SDL2
sudo apt-get install -libsdl2-dev
# Installing SDL2-ttf
sudo apt-get install -libsdl2-ttf-dev
# Installing SDL2-net
sudo apt-get install -libsdl2-net-dev
# Installing SDL2-mixer
sudo apt-get install -libsdl2-mixer-dev
# Install npm
sudo apt-get install npm
2) MacOS->

Game Instructions- After installing the above files. Do the following steps->
1) Server Side- We need one server to coordinate between the clients. 
# Change to server folder
cd server/
# Make the executable
make
# Run the executable
./server
# Enter the total players playing(this can be upto 4)
# Kill the port to before playing again
npx kill-port 3000

2) Client Side- All the players should do the following steps-
# Change to client folder
cd client/
# Make the executable
make
# Run the executable
./game


