CPPC=g++
CPPC_OPTS=-Wall
SRC_DIR=src
BIN=bin-project

SRC=$(SRC_DIR)/game_of_life.cpp $(SRC_DIR)/main.cpp $(SRC_DIR)/ea_operations.cpp
HDR=$(SRC_DIR)/game_of_life.h $(SRC_DIR)/ea_operations.h

TEST_COMMAND={ time ./$(BIN) $(MAP_SIZE) $(OUTPUT_FILE); } > /dev/null

all: run

run:
	./$(BIN)

install: compile

compile: clean
	$(CPPC) $(CPPC_OPTS) $(SRC) $(HDR) -o $(BIN) -ljsoncpp

clean:
	rm -rf $(BIN) best_solution.gol

test: install
	python3 measure.py "$(TEST_COMMAND)"
