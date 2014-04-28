# Variables
PROJECT = bludiste2014
BIN_CLIENT = $(PROJECT)
BIN_SERVER = $(PROJECT)-server

CC = g++
RM = rm -rf
TAR = tar -czf

CFLAGS = -Wall -Wextra -c --std=c++11
CFLAGS_RELEASE = -O2
CFLAGS_DEBUG = -O0 -g -DDEBUG

LFLAGS = -lpthread -lboost_system -lboost_thread-mt

SRC_FOLDER = src
BIN_FOLDER = bin
OBJ_FOLDER = obj

TAR_FILE = xmilko01.tgz
PACKED_FILES = $(SRC_FOLDER) $(HEADER_FOLDER) Makefile $(DOXYFILE)
DOXYFILE = doxyconfig
DOXY_DIR = doc

INCLUDES = -Iinc

SERVER_SRCS = $(wildcard $(SRC_FOLDER)/server/*.cpp)
SERVER_OBJS = $(addprefix $(OBJ_FOLDER)/server/, $(notdir $(patsubst %.cpp, %.o, $(SERVER_SRCS))))

CLIENT_SRCS = $(wildcard $(SRC_FOLDER)/client/*.cpp)
CLIENT_OBJS = $(addprefix $(OBJ_FOLDER)/client/, $(notdir $(patsubst %.cpp, %.o, $(CLIENT_SRCS))))

COMMON_SRCS = $(wildcard $(SRC_FOLDER)/common/*.cpp)
COMMON_OBJS = $(addprefix $(OBJ_FOLDER)/common/, $(notdir $(patsubst %.cpp, %.o, $(COMMON_SRCS))))

# Targets
release: CFLAGS += $(CFLAGS_RELEASE)
release: build

debug: CFLAGS += $(CFLAGS_DEBUG)
debug: build

build: server client

server: $(COMMON_OBJS) $(SERVER_OBJS)
	$(CC) $^ -o $(BIN_FOLDER)/$(BIN_SERVER) $(LFLAGS)

client: $(COMMON_OBJS) $(CLIENT_OBJS)
	$(CC) $^ -o $(BIN_FOLDER)/$(BIN_CLIENT) $(LFLAGS)

$(OBJ_FOLDER)/server/%.o: $(SRC_FOLDER)/server/%.cpp
	@mkdir -p $(BIN_FOLDER) $(OBJ_FOLDER)/server
	$(CC) $(INCLUDES) $(CFLAGS) $< -o $@

$(OBJ_FOLDER)/client/%.o: $(SRC_FOLDER)/client/%.cpp
	@mkdir -p $(BIN_FOLDER) $(OBJ_FOLDER)/client
	$(CC) $(INCLUDES) $(CFLAGS) $< -o $@

$(OBJ_FOLDER)/common/%.o: $(SRC_FOLDER)/common/%.cpp
	@mkdir -p $(BIN_FOLDER) $(OBJ_FOLDER)/common
	$(CC) $(INCLUDES) $(CFLAGS) $< -o $@

clean:
	$(RM) $(BIN_FOLDER) $(OBJ_FOLDER) $(TAR_FILE) $(DOXY_DIR)

pack:
	$(TAR) $(TAR_FILE) $(PACKED_FILES)

run:
	@./$(BIN_FOLDER)/$(SERVER) &
	@./$(BIN_FOLDER)/$(CLIENT)

doxygen:
	@command -v doxygen >/dev/null 2>&1 \
		&& { doxygen $(DOXYFILE); true; } \
		|| echo "icp-make: Doxygen not installed."

.PHONY: build release debug server client run clean pack doxygen
