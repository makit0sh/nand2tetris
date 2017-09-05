CXX = g++
TARGET = bin/VMtranslator
CXXFLAGS = -Wall -O2 -pipe
SRCS = VMtranslator.cpp Parser.cpp CodeWriter.cpp
OBJS := $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

clean:
	rm -f $(TARGET) $(OBJS)
