CPPC=g++
CPPC_OPTS=-Wall -Werror
SRC_DIR=src
MAPS_DIR=maps
BIN=bin-project
MAP_SIZE=64

OUTPUT_FILE=$(MAPS_DIR)/output.gol
SRC=$(SRC_DIR)/game_of_life.cpp $(SRC_DIR)/main.cpp
HDR=$(SRC_DIR)/game_of_life.h

all: run

run:
	./$(BIN) $(MAP_SIZE) $(OUTPUT_FILE)

install: compile

compile: clean
	$(CPPC) $(CPPC_OPTS) $(SRC) $(HDR) -o $(BIN)

clean:
	rm -rf $(BIN)
