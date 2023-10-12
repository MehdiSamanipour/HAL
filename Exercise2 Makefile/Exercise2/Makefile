SOURCES= part1.cpp part2.cpp main.cpp
DEPS=part1.h part2.h
OBJECTS= $(SOURCES:.cpp=.o)
EXE=exe
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
