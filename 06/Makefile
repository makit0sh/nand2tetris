CXX = g++
TARGET = bin/Assembler
CXXFLAGS = -Wall -O2 -pipe
SRCS = Assembler.cpp Parser.cpp Code.cpp
OBJS := $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

clean:
	rm -f $(TARGET) $(OBJS)
