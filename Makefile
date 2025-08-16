CXX=g++
CXXFLAGS=-std=c++17 -O2 -pthread
TARGET=scheduler
SRC=src/main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
