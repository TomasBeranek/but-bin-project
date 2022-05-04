CPPC=g++
CPPC_OPTS=-Wall
SRC_DIR=src
BIN=bin-project

SRC=$(SRC_DIR)/game_of_life.cpp $(SRC_DIR)/main.cpp
HDR=$(SRC_DIR)/game_of_life.h

TEST_COMMAND={ time ./$(BIN) $(MAP_SIZE) $(OUTPUT_FILE); } > /dev/null

all: run

run:
	./$(BIN)

install: compile

compile: clean
	$(CPPC) $(CPPC_OPTS) $(SRC) $(HDR) -o $(BIN) -ljsoncpp

clean:
	rm -rf $(BIN)

test: install
	python3 measure.py "$(TEST_COMMAND)"
