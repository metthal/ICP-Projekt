# Variables
PROJECT = bludiste2014
CLIENT = $(PROJECT)
SERVER = $(PROJECT)-server

CC = g++
RM = rm -rf
TAR = tar -czf

CFLAGS = -Wall -Wextra -c --std=c++11
CFLAGS_RELEASE = -O2 -s
CFLAGS_DEBUG = -O0 -g -DDEBUG

SRC_FOLDER = src
BIN_FOLDER = bin
OBJ_FOLDER = obj

TAR_FILE = xmilko01.tgz
PACKED_FILES = $(SRC_FOLDER) $(HEADER_FOLDER) Makefile $(DOXYFILE)
DOXYFILE = doxyconfig
DOXY_DIR = doc

CLIENT_INCLUDES = -Iinc/client -Iinc/common
SERVER_INCLUDES = -Iinc/common -Iinc/server

S_SOURCES = $(wildcard $(SRC_FOLDER)/server/*.cpp)
S_OBJECTS = $(addprefix $(OBJ_FOLDER)/, $(notdir $(patsubst %.cpp, %.s.o, $(S_SOURCES))))

C_SOURCES = $(wildcard $(SRC_FOLDER)/client/*.cpp)
C_OBJECTS = $(addprefix $(OBJ_FOLDER)/, $(notdir $(patsubst %.cpp, %.c.o, $(C_SOURCES))))

X_SOURCES = $(wildcard $(SRC_FOLDER)/common/*.cpp)
S_OBJECTS += $(addprefix $(OBJ_FOLDER)/, $(notdir $(patsubst %.cpp, %.xs.o, $(X_SOURCES))))
C_OBJECTS += $(addprefix $(OBJ_FOLDER)/, $(notdir $(patsubst %.cpp, %.xc.o, $(X_SOURCES))))

# Targets
release: CFLAGS += $(CFLAGS_RELEASE)
release: build

debug: CFLAGS += $(CFLAGS_DEBUG)
debug: build

build: server client

server: $(S_OBJECTS)
	$(CC) $^ -o $(BIN_FOLDER)/$(SERVER)

client: $(C_OBJECTS)
	$(CC) $^ -o $(BIN_FOLDER)/$(CLIENT)
	
$(OBJ_FOLDER)/%.s.o: $(SRC_FOLDER)/server/%.cpp
	@mkdir -p $(BIN_FOLDER) $(OBJ_FOLDER)
	$(CC) $(SERVER_INCLUDES) $(CFLAGS) $< -o $@

$(OBJ_FOLDER)/%.xs.o: $(SRC_FOLDER)/common/%.cpp
	@mkdir -p $(BIN_FOLDER) $(OBJ_FOLDER)
	$(CC) $(SERVER_INCLUDES) $(CFLAGS) $< -o $@

$(OBJ_FOLDER)/%.c.o: $(SRC_FOLDER)/client/%.cpp
	@mkdir -p $(BIN_FOLDER) $(OBJ_FOLDER)
	$(CC) $(CLIENT_INCLUDES) $(CFLAGS) $< -o $@

$(OBJ_FOLDER)/%.xc.o: $(SRC_FOLDER)/common/%.cpp
	@mkdir -p $(BIN_FOLDER) $(OBJ_FOLDER)
	$(CC) $(CLIENT_INCLUDES) $(CFLAGS) $< -o $@

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
