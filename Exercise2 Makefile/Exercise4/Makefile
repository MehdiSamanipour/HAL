SOURCES=main.cpp server.o data.o connection.o
OBJECTS=$(SOURCES:.cpp =.o)
DEPS=$(SOURCES:.cpp=.o)
EXE=prog
CXXFLAGS=-I.
$(EXE): $(DEPS) $(OBJECTS) # << Check the $(DEPS) new dependency
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) 
 # << Rule that describes how a .d (dependency) file is created from a .cpp file
# << Similar to the assigment that you just completed %.cpp -> %.o
%.o: %.cpp
	$(CXX) -MT$@ -MM $(CXXFLAGS) $< > $@
	$(CXX) -MT$(@:.d=.o) -MM $(CXXFLAGS) $< >> $@
-include $(DEPS)
clean :
	-rm -f *.o $(EXE)
 .PHONY : help
help :
	@echo all 
	@echo clean
	@echo help
