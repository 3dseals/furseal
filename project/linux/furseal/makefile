#
# makefile
#
#  Created on: 2011-3-2
#      Author: Lional King
#
#   Copyright (c) 2011 netsurfers
#

BASE_DIR = ../../..

CXX         = g++
CXXFLAGS    = -O2 -fno-strict-aliasing -Wall -s
DEFINES     = -DNDEBUG -DFS_LINUX
INCLUDES    = -I$(BASE_DIR)/include -I$(BASE_DIR)/sources -I$(BASE_DIR)/project/include/zlib -I$(BASE_DIR)/project/include/libpng -I$(BASE_DIR)/project/include/freetype

AR          = ar
ARFLAGS     = rs
SRCS        = $(shell find $(BASE_DIR)/sources -name "*.cpp")
OBJS        = $(addsuffix .o, $(basename $(SRCS)))
TARGET      = $(BASE_DIR)/library/libfurseal.a


.PHONY: all clean

all: $(TARGET)
	cp $(BASE_DIR)/project/linux/library/libz.a $(BASE_DIR)/library
	cp $(BASE_DIR)/project/linux/library/libpng.a $(BASE_DIR)/library
	cp $(BASE_DIR)/project/linux/library/libfreetype.a $(BASE_DIR)/library

clean:
	rm -f $(TARGET) $(OBJS)
	rm -f $(BASE_DIR)/library/libz.a
	rm -f $(BASE_DIR)/library/libpng.a
	rm -f $(BASE_DIR)/library/libfreetype.a

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $(TARGET) $(OBJS)


.SUFFIXES: .o .cpp

.cpp.o:
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@
