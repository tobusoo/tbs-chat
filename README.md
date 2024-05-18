# TOBUSO-CHAT
Simple online chat in the terminal based on asynchronous client-server architecture. Written in c++20 using [FTXUI](https://github.com/ArthurSonzogni/FTXUI) for ui

## Build the project
To build a project, use the following commands. N is the number of cores on your system.
```
mkdir build
cd build
cmake ..
cd ..
cmake --build build --parallel N
```

## Start the server
To start the server, use the following commands. `p` is the port number on which the server will be started. `P` is the password for the server. `l` is the size of the queue on the server for incoming connections. By default, `l` is eqaul 50.
```
./build/bin/tbs-server -p 5050 -P 123 -l 50
```

## Start client
To launch the client, use the following commands and options. `u ` is the username. `H` is the server address in IPv4 format. `p` is the port number on which the server is running. `P` is the password for the server.
```
 ./build/bin/tbs-chat -u tobuso -H 0.0.0.0 -p 5050 -P 123
```

## Some examples
### Chat with 4 (one "disconnected") user
![image](https://github.com/tobusoo/tbs-chat/assets/106862439/39f157a8-bfbd-4b47-be34-0f65b57ad489)
![image](https://github.com/tobusoo/tbs-chat/assets/106862439/39b6a15b-c308-49d0-a594-8830181d88ec)
![image](https://github.com/tobusoo/tbs-chat/assets/106862439/7c20f152-5558-46c6-8d94-730022987b8b)

### Example with a lot of messages
![image](https://github.com/tobusoo/tbs-chat/assets/106862439/f6ae5775-f46c-4d06-86e3-d8a773c7d249)



### Online users tab
![image](https://github.com/tobusoo/tbs-chat/assets/106862439/893671e3-0a40-4dea-a77a-fc222f0317f2)

### Info tab
![image](https://github.com/tobusoo/tbs-chat/assets/106862439/594c7745-1d96-46c3-ac43-e2462db0d0f1)
