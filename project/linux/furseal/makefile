#
#   Copyright (c) 2011 netsurfers
#

BASE_DIR = ../../..

CXX         = g++
CXXFLAGS    = -O2 -fno-strict-aliasing -Wall -s
DEFINES     = -DNDEBUG -DCK_LINUX
INCLUDES    = -I$(BASE_DIR)/include -I$(BASE_DIR)/sources

AR          = ar
ARFLAGS     = rs
SRCS        = $(shell find $(BASE_DIR)/sources -name "*.cpp")
OBJS        = $(addsuffix .o, $(basename $(SRCS)))
TARGET      = $(BASE_DIR)/library/libfurseal.a


.PHONY: all clean


$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $(TARGET) $(OBJS)


.SUFFIXES: .o .cpp

.cpp.o:
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@
