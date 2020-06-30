SRC_DIR := ./src
OBJ_DIR := ./bin
LIB_DIR := ./lib
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(LIB_DIR)/%.o,$(SRC_FILES))

CPPFLAGS := -std=c++17 -I includes/
LDFLAGS := 

run: bin/app
	./bin/app

bin/app: bin/main.o $(OBJ_FILES)
	g++ $(LDFLAGS) -o $@ $^


bin/main.o: main.cpp
	g++ $(CPPFLAGS) -c -o $@ $<


$(LIB_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CPPFLAGS) -c -o $@ $<
