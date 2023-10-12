SOURCES =  test.cpp
OBJECTS = $(addprefix $(BUILD_DIR)/,$(SOURCES:.cpp=.o))
#OBJECTS = $(SOURCES:.cpp=.o)
#DEPS = $(SOURCES:.cpp=.d)
DEPS = $(addprefix $(BUILD_DIR)/,$(SOURCES:.cpp=.d))
EXE = $(addprefix $(BIN_DIR)/,prog)
CXXFLAGS = -I.
ARCH?=x86-64
#making for x86-64
ifeq (${ARCH},x86-64)
CXX=g++
BUILD_DIR=build/x86-64
BIN_DIR=bin/x86-64
endif
#making for arm
ifeq (${ARCH},arm)
CXX=arm-rpizw-g++
BUILD_DIR=build/arm
BIN_DIR=bin/arm
endif
$(EXE): $(DEPS) $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@  $(OBJECTS) -pthread
$(BUILD_DIR)/%.o: %.cpp  $(BUILD_DIR)/%.d
	$(CXX) $(CXXFLAGS)  -o $@ -c $<
$(BUILD_DIR)/%.d: %.cpp
	$(CXX) -MT $@  -MM $(CXXFLAGS) $< > $@
#	$(CXX) -MT $(@:.d=.o) -MM $(CXXFLAGS) $< >> $@
ifneq ($(MAKECMDGOALS), clean)
-include $(DEPS)
endif