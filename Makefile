CXX=g++
CXXFLAGS=-Wall -pthread --std=c++11
SRC_PATH=./src
OUT_PATH=./bin
FILES=main.cpp ControlConsole.cpp ConsoleThread.cpp
OBJECTS=$(FILES:%.cpp=$(SRC_PATH)/%.o)

all: conctrl procesoctrl

procesoctrl: procesoctrl.o
	$(CXX) -o $(SRC_PATH)/$@ $(SRC_PATH)/$< $(CXXFLAGS)

procesoctrl.o: $(SRC_PATH)/ControllerProcess.cpp
	$(CXX) -c $(CXXFLAGS) -I$(SRC_PATH) $< -o $(SRC_PATH)/$@

conctrl: main.o ControlConsole.o ConsoleThread.o
	$(CXX) -o $(SRC_PATH)/$@ $(OBJECTS) $(CXXFLAGS)

main.o: $(SRC_PATH)/main.cpp
	$(CXX) -c $(CXXFLAGS) -I$(SRC_PATH) $< -o $(SRC_PATH)/$@

ControlConsole.o: $(SRC_PATH)/ControlConsole.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $(SRC_PATH)/$@

ConsoleThread.o: $(SRC_PATH)/ConsoleThread.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $(SRC_PATH)/$@

init: $(SRC_PATH)/conctrl
	mkdir -p $(OUT_PATH)
	mv $< $(OUT_PATH)
clean:
	rm $(SRC_PATH)/*.o
	rm $(SRC_PATH)/conctrl
	rm $(SRC_PATH)/procesoctrl
