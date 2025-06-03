OUT     := hnsw
CXX     := c++
CXXFLAGS= -ggdb3 -Wall -Ieigen -Iinclude -std=c++14 -fsanitize=address -O0

all:
	$(CXX) src/*.cpp $(CXXFLAGS) -o $(OUT)

clean:
	rm $(OUT)