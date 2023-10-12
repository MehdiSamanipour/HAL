SOURCES= test.cpp 
DEPS=vector.hpp
OBJECTS= $(SOURCES:.cpp=.o)
EXE=prog
CXX=g++
all:$(OBJECTS)
	$(CXX) $^ -o $(EXE) -lncurses
%.o: %.cpp %.h
	$(CXX) -c $< -o $@

clean :
	-rm -f *.o $(EXE)
 .PHONY : help
help :
	@echo all
	@echo clean
	@echo help