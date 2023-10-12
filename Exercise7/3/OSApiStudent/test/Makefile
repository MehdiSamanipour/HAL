# Quick and dirty (does not handle changes in h-file)

# Get all .cpp files in the current directory
SRCS=$(wildcard *.cpp)

# Replace .cpp with .o to get object file names
OBJS=$(SRCS:.cpp=.o)

# Specify the base path for the project
BASEPATH=..

# Determine whether this is a debug build or not
ifdef DEBUG
CXXFLAGS=-ggdb -O0
LIBPATH=$(BASEPATH)/lib/host/debug
else
CXXFLAGS=-O2
LIBPATH=$(BASEPATH)/lib/host/release
endif

# Update compiler and library paths for Raspberry Pi Zero W
CXX=arm-linux-gnueabihf-g++
CXXFLAGS+=-Wall -D_REENTRANT -DOS_LINUX -I$(BASEPATH)/inc
LIBS=-L/home/legion/ISU_2023/Exercise7/3/OSApiStudent/lib/host/debug -lOSApi -lrt -lpthread

# Build the main target
main: $(OBJS)
	$(CXX) $(CXXFLAGS) -o main $(OBJS) $(LIBS)

# Build all targets
all: main

# Clean up object files and executable
clean:
	rm -f $(OBJS) main
