CC:=g++
OUTPUT=build/TIC_TAC_TOE
SRC_DIR := src
OBJ_DIR := build/obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS := 
CPPFLAGS := 
CXXFLAGS := -std=c++17


 
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p  build
	mkdir -p build/obj
	$(CC)   $(CXXFLAGS)   -c -o $@ $^  

$(OUTPUT): $(OBJ_FILES)
	$(CC)  -o $@ $^   $(CXXFLAGS) 
clean :
	rm -rf build
build:
	mkdir build
	mkdir build/obj

