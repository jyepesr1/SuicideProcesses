CXX=g++
CXXFLAGS=-Wall
SRC_PATH=./src
OUT_PATH=./bin
FILES=main.cpp ControlConsole.cpp
OBJECTS=$(FILES:%.cpp=$(SRC_PATH)/%.o)

all: conctrl

conctrl: main ControlConsole
	$(CXX) -o $(SRC_PATH)/conctrl $(OBJECTS)

main: $(SRC_PATH)/main.cpp
	$(CXX) -c $(CXXFLAGS) -I$(SRC_PATH) $< -o $(SRC_PATH)/$@.o

ControlConsole: $(SRC_PATH)/ControlConsole.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $(SRC_PATH)/$@.o

init: $(SRC_PATH)/conctrl
	mkdir -p $(OUT_PATH)
	mv $< $(OUT_PATH)
clean:
	rm $(SRC_PATH)/*.o
	rm $(OUT_PATH)/conctrl
