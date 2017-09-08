CXX = g++
TARGET = bin/JackAnalyzer
CXXFLAGS = -Wall -O2 -pipe
SRCS = JackAnalyzer.cpp JackTokenizer.cpp CompilationEngine.cpp
OBJS := $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

clean:
	rm -f $(TARGET) $(OBJS)
