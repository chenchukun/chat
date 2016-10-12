#!/bin/bash
g++ ./src/*.cpp -pthread -I/usr/local/include  -pthread -L/usr/local/lib -lprotobuf -lpthread -I./include -lreactor -L./lib -std=c++11 -o chat -DDEBUG -lhiredis -g
