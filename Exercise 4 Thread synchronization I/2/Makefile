SOURCES=test.cpp
DEPS=Vector.hpp
OBJECTS=$(SOURCES:.cpp=.o)
EXE=prog
CXX=g++

all: $(EXE)

$(EXE): $(OBJECTS) $(DEPS)
	$(CXX) $^ -o $@ -lpthread

%.o: %.cpp $(DEPS)
	$(CXX) -c $< -o $@

clean:
	-rm -f *.o $(EXE)

.PHONY: all clean
