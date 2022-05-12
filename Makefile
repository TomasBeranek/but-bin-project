CPPC=g++
CPPC_OPTS=-Wall -Ofast -std=c++11
SRC_DIR=src
BIN=bin-project

SRC=$(SRC_DIR)/game_of_life.cpp $(SRC_DIR)/main.cpp $(SRC_DIR)/ea_operations.cpp
HDR=$(SRC_DIR)/game_of_life.h $(SRC_DIR)/ea_operations.h

TEST_COMMAND={ time ./$(BIN); } > /dev/null

all: run

run:
	./$(BIN)

install: compile

compile: clean
	$(CPPC) $(CPPC_OPTS) $(SRC) $(HDR) -o $(BIN) -ljsoncpp -pthread

clean:
	rm -rf $(BIN) best_solution.gol debug

test: install
	python3 measure.py "$(TEST_COMMAND)"

debug: clean
	$(CPPC) $(CPPC_OPTS) -I$(SRC_DIR) $(SRC_DIR)/game_of_life.cpp tests/main.cpp $(SRC_DIR)/ea_operations.cpp $(HDR) -o debug -ljsoncpp
	./debug

remove_zip:
	rm xberan46.zip

zip: remove_zip
	zip -r xberan46.zip patterns/ src/ config.json measure.py Makefile

statistics:
	python3 statistics.py "./$(BIN)"

graph:
	python3 show_graphs.py statistics.csv statistics2.csv
