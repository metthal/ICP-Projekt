# Variables
PROJECT = bludiste2014
BIN_CLIENT = $(PROJECT)-cli
BIN_SERVER = $(PROJECT)-server

CXX = $(shell command -v g++-4.8 >/dev/null 2>&1 && echo -n "g++-4.8" || echo -n "g++")
RM = rm -rf
TAR = tar -czf

CFLAGS = -Wall -Wextra -c --std=c++11
CFLAGS_RELEASE = -O2
CFLAGS_DEBUG = -O0 -g -DDEBUG
QFLAGS =
QFLAGS_DEBUG = CONFIG+=debug CONFIG+=declarative_debug CONFIG+=qml_debug

LFLAGS = -lpthread -lboost_system -lboost_filesystem
LFLAGS += $(shell command -v g++-4.8 >/dev/null 2>&1 && echo -n "-static-libstdc++")

SRC_FOLDER = src
BIN_FOLDER = bin
OBJ_FOLDER = obj
GUI_FOLDER = gui
GUI_WORK_FOLDER = gui-work

TAR_FILE = xmilko01.tgz
PACKED_FILES = $(SRC_FOLDER) $(HEADER_FOLDER) Makefile $(DOXYFILE) $(GUI_FOLDER)
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
debug: QFLAGS += $(QFLAGS_DEBUG)
debug: build

build: server client gui

server: $(COMMON_OBJS) $(SERVER_OBJS)
	$(CXX) $^ -o $(BIN_FOLDER)/$(BIN_SERVER) $(LFLAGS)

client: $(COMMON_OBJS) $(CLIENT_OBJS)
	$(CXX) $^ -o $(BIN_FOLDER)/$(BIN_CLIENT) $(LFLAGS)
	
gui:
	@cd $(GUI_FOLDER) && qmake -o ../$(GUI_WORK_FOLDER)/Makefile $(QFLAGS)
	$(MAKE) CXX=$(CXX) -C $(GUI_WORK_FOLDER)

$(OBJ_FOLDER)/server/%.o: $(SRC_FOLDER)/server/%.cpp
	@mkdir -p $(BIN_FOLDER) $(OBJ_FOLDER)/server
	$(CXX) $(INCLUDES) $(CFLAGS) $< -o $@

$(OBJ_FOLDER)/client/%.o: $(SRC_FOLDER)/client/%.cpp
	@mkdir -p $(BIN_FOLDER) $(OBJ_FOLDER)/client
	$(CXX) $(INCLUDES) $(CFLAGS) $< -o $@

$(OBJ_FOLDER)/common/%.o: $(SRC_FOLDER)/common/%.cpp
	@mkdir -p $(BIN_FOLDER) $(OBJ_FOLDER)/common
	$(CXX) $(INCLUDES) $(CFLAGS) $< -o $@

clean:
	$(RM) $(BIN_FOLDER) $(OBJ_FOLDER) $(TAR_FILE) $(DOXY_DIR) $(GUI_WORK_FOLDER)

pack:
	$(TAR) $(TAR_FILE) $(PACKED_FILES)

run:
	@./$(BIN_FOLDER)/$(BIN_SERVER) &
	@./$(BIN_FOLDER)/$(BIN_CLIENT)

run-gui:
	@./$(BIN_FOLDER)/$(BIN_SERVER) &
	@./$(BIN_FOLDER)/$(PROJECT)

doxygen:
	@command -v doxygen >/dev/null 2>&1 \
		&& { doxygen $(DOXYFILE); true; } \
		|| echo "icp-make: Doxygen not installed."

.PHONY: build release debug server client gui run run-gui clean pack doxygen
