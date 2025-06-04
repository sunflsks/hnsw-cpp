OUT     := hnsw
CXX     := c++
SANFLAGS := -fsanitize=address,undefined
CXXFLAGS= -ggdb3 -Wall -Werror -Ieigen -Iinclude -std=c++14 -O0 $(SANFLAGS)

all: clean
	$(CXX) src/*.cpp $(CXXFLAGS) -o $(OUT)

clean:
	rm -rf $(OUT) $(OUT).dSYM
