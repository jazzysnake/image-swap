# Compiler options
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
BUILD_DIR := build
INCLUDE_DIR = include
LINK_DIR = lib
SRC_DIR := src
INCLUDE := -I$(INCLUDE_DIR)/webp
LINK := -L./$(LINK_DIR) -lwebp -lsharpyuv -lpthread

main: build_dir main.o util.o CIFF.o CAFF.o 
	$(CXX) $(CXXFLAGS) $(wildcard $(BUILD_DIR)/*.o) -o parser $(LINK)

build_dir:
	$(shell mkdir -p $(BUILD_DIR))

main.o: $(SRC_DIR)/main.cpp
	$(CXX) $(INCLUDE) -c $(SRC_DIR)/main.cpp -o $(BUILD_DIR)/main.o

CIFF.o: $(SRC_DIR)/CIFF.cpp $(SRC_DIR)/CIFF.h
	$(CXX) -c $(SRC_DIR)/CIFF.cpp -o $(BUILD_DIR)/CIFF.o

CAFF.o: $(SRC_DIR)/CAFF.cpp $(SRC_DIR)/CAFF.h
	$(CXX) -c $(SRC_DIR)/CAFF.cpp -o $(BUILD_DIR)/CAFF.o

util.o: $(SRC_DIR)/util.cpp $(SRC_DIR)/util.h
	$(CXX) -c $(SRC_DIR)/util.cpp -o $(BUILD_DIR)/util.o

# Clean up
clean:
	rm -rf build parser
