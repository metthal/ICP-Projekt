# Variables
PROJECT = bludiste2014
CLIENT = $(PROJECT)
SERVER = $(PROJECT)-server

CC = g++
RM = rm -rf
TAR = tar -czf

STD_CPP11 = -std=c++11

CFLAGS = -Wall -Wextra -c
CFLAGS_RELEASE = -O3 -s
CFLAGS_DEBUG = -O0 -g -DDEBUG

SRC_FOLDER = src
BIN_FOLDER = bin

TAR_FILE = xmilko01.tgz
PACKED_FILES = $(SRC_FOLDER) $(HEADER_FOLDER) Makefile $(DOXYFILE)
DOXYFILE = doxyconfig
DOXY_DIR = doc

SHELL = /bin/bash

CLIENT_INCLUDES = -Iinc/client -Iinc/common
SERVER_INCLUDES = -Iinc/common -Iinc/server

S_SOURCES = $(wildcard $(SRC_FOLDER)/server/*.cpp)
S_OBJECTS = $(notdir $(patsubst %.cpp, %.s.o, $(S_SOURCES)))

C_SOURCES = $(wildcard $(SRC_FOLDER)/client/*.cpp)
C_OBJECTS = $(notdir $(patsubst %.cpp, %.c.o, $(C_SOURCES)))

X_SOURCES = $(wildcard $(SRC_FOLDER)/common/*.cpp)
S_OBJECTS += $(notdir $(patsubst %.cpp, %.xs.o, $(X_SOURCES)))
C_OBJECTS += $(notdir $(patsubst %.cpp, %.xc.o, $(X_SOURCES)))

# Targets
release: CFLAGS += $(CFLAGS_RELEASE)
release: build

debug: CFLAGS += $(CFLAGS_DEBUG)
debug: build

build: CFLAGS += $(STD_CPP11)
build: server client

server: $(S_OBJECTS)
	@echo $(S_OBJECTS)
	$(CC) $(addprefix $(BIN_FOLDER)/server/, $(S_OBJECTS)) -o $(BIN_FOLDER)/server/$(SERVER)

client: $(C_OBJECTS)
	@echo $(C_OBJECTS)
	$(CC) $(addprefix $(BIN_FOLDER)/client/, $(C_OBJECTS)) -o $(BIN_FOLDER)/client/$(CLIENT)
	
%.s.o: $(SRC_FOLDER)/server/%.cpp
	@mkdir -p bin/server
	$(CC) $(SERVER_INCLUDES) $(CFLAGS) $< -o $(BIN_FOLDER)/server/$@

%.xs.o: $(SRC_FOLDER)/common/%.cpp
	@mkdir -p bin/server
	$(CC) $(SERVER_INCLUDES) $(CFLAGS) $< -o $(BIN_FOLDER)/server/$@

%.c.o: $(SRC_FOLDER)/client/%.cpp
	@mkdir -p bin/client
	$(CC) $(CLIENT_INCLUDES) $(CFLAGS) $< -o $(BIN_FOLDER)/client/$@

%.xc.o: $(SRC_FOLDER)/common/%.cpp
	@mkdir -p bin/client
	$(CC) $(CLIENT_INCLUDES) $(CFLAGS) $< -o $(BIN_FOLDER)/client/$@

clean:
	$(RM) $(BIN_FOLDER) $(TAR_FILE) $(DOXY_DIR)

pack:
	$(TAR) $(TAR_FILE) $(PACKED_FILES)

run:
	./$(BIN_FOLDER)/server/$(SERVER) &
	./$(BIN_FOLDER)/client/$(CLIENT)

doxygen:
	@command -v doxygen >/dev/null 2>&1 \
		&& { doxygen $(DOXYFILE); true; } \
		|| echo "icp-make: Doxygen not installed."

statistics:
	@echo -n "Lines of code: " && wc -l $(SRC_FILES) $(HEADER_FILES) | tail -n 1 | sed -r "s/[ ]*([0-9]+).*/\1/g"
	@echo -n "Size of code: " && du -hsc $(SRC_FILES) $(HEADER_FILES) | tail -n 1 | cut -f 1
	@echo -n "Lines of code (with tests): " && wc -l $(SRC_FILES) $(TEST_SRC_FILES) $(HEADER_FILES) $(TEST_HEADER_FILES) | tail -n 1 | sed -r "s/[ ]*([0-9]+).*/\1/g"
	@echo -n "Size of code (with tests): " && du -hsc $(SRC_FILES) $(TEST_SRC_FILES) $(HEADER_FILES) $(TEST_HEADER_FILES) | tail -n 1 | cut -f 1
	@test -e ini && echo -n "Size of executable: " && du -hs ini | cut -f 1 || echo "ini-make: Executable not found."

.PHONY: build release debug clean pack doxygen statistics
