CXX :=g++
CFLAGS := -Wall -Werror
all: Hello 
Hello: Hello.o
	$(CXX) $(CXXFLAGS) -o $@ $^
Hello.o: Hello.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean :
	-rm -f *.o Hello
 .PHONY : help
help :
	@echo all
	@echo clean
	@echo help
