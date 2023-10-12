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

# Setup the CFLAGS to ensure that the relevant warnings, includes and defines.
CXXFLAGS+=-Wall -D_REENTRANT -DOS_LINUX -I$(BASEPATH)/inc

# Build the main target
main: $(OBJS)
	g++ $(CXXFLAGS) -o main $(OBJS) -L$(LIBPATH) -lOSApi -lrt -lpthread

# Build all targets
all: main

# Clean up object files and executable
clean:
	rm -f $(OBJS) main
