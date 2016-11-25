CXX=g++
CXXFLAGS=-Wall -pthread --std=c++11
SRC_PATH=./src
OUT_PATH=./bin

all: $(SRC_PATH)/conctrl $(SRC_PATH)/procesoctrl

$(SRC_PATH)/procesoctrl: $(SRC_PATH)/procesoctrl.o $(SRC_PATH)/ControllerProcessAux.o
	$(CXX) -o $@ $^ $(CXXFLAGS)

$(SRC_PATH)/procesoctrl.o: $(SRC_PATH)/ControllerProcess.cpp
	$(CXX) -c $(CXXFLAGS) -I$(SRC_PATH) $< -o $@
	
$(SRC_PATH)/ControllerProcessAux.o: $(SRC_PATH)/ControllerProcessAux.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(SRC_PATH)/conctrl: $(SRC_PATH)/main.o $(SRC_PATH)/ControlConsole.o $(SRC_PATH)/ConsoleThread.o
	$(CXX) -o $@ $^ $(CXXFLAGS)

$(SRC_PATH)/main.o: $(SRC_PATH)/main.cpp
	$(CXX) -c $(CXXFLAGS) -I$(SRC_PATH) $< -o $@

$(SRC_PATH)/ControlConsole.o: $(SRC_PATH)/ControlConsole.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(SRC_PATH)/ConsoleThread.o: $(SRC_PATH)/ConsoleThread.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

init: all
	mkdir -p $(OUT_PATH)
	mv $(SRC_PATH)/conctrl $(OUT_PATH)
	mv $(SRC_PATH)/procesoctrl $(OUT_PATH)
	
clean:
	rm -f $(SRC_PATH)/*.o
	rm -f $(SRC_PATH)/conctrl
	rm -f $(SRC_PATH)/procesoctrl
	rm -rf $(OUT_PATH)
	rm -f $(SRC_PATH)/log.txt

install: all
	mkdir -p $(PREFIX)/bin
	mv $(SRC_PATH)/conctrl $(PREFIX)/bin
	mv $(SRC_PATH)/procesoctrl $(PREFIX)/bin
	cp -r ./examples $(PREFIX)