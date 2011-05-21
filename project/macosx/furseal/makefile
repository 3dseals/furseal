#
# makefile
#
#  Created on: 2011-5-16
#      Author: Lional King
#
#   Copyright (c) 2011 netsurfers
#


BASE_DIR = ../../..

CXX         = g++
CXXFLAGS    = -O2 -fno-strict-aliasing -Wall -s
DEFINES     = -DNDEBUG -DFS_MACOSX
INCLUDES    = -I$(BASE_DIR)/include -I$(BASE_DIR)/project/include/zlib -I$(BASE_DIR)/project/include/libpng -I$(BASE_DIR)/project/include/freetype -I/usr/X11R6/include
AR          = ar
ARFLAGS     = rs
SRCS        = $(shell find $(BASE_DIR)/sources -name "*.cpp")
OBJS        = $(addsuffix .o, $(basename $(SRCS)))
TARGET      = $(BASE_DIR)/library/libfurseal.a


.PHONY: all clean

all: $(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $(TARGET) $(OBJS)


.SUFFIXES: .o .cpp

.cpp.o:
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@
