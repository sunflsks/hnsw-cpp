OUT     := hnsw
CXX     := c++
CXXFLAGS= -ggdb3 -Wall -Werror -Ieigen -Iinclude -std=c++14 -fsanitize=address,undefined -fsanitize-address-use-after-scope -O0

all: clean
	$(CXX) src/*.cpp $(CXXFLAGS) -o $(OUT)

clean:
	rm -rf $(OUT) $(OUT).dSYM
