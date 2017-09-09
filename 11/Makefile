CXX = g++
TARGET = bin/JackCompiler
CXXFLAGS = -Wall -O2 -pipe
SRCS = JackCompiler.cpp JackTokenizer.cpp SymbolTable.cpp VMWriter.cpp CompilationEngine.cpp
OBJS := $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

clean:
	rm -f $(TARGET) $(OBJS)
