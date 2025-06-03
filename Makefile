SRCS = $(wildcard *.cpp)
OBJS = ${SRCS:.cpp=.o}
OUT	= hnsw
CXXFLAGS= -ggdb3 -Wall -Ieigen -std=c++14 -fsanitize=address -O0
CC	= c++

all: $(OUT)

$(OUT): $(OBJS)
	$(CC) $(CXXFLAGS) -o $(OUT) $(OBJS) $(LFLAGS)

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OUT)
