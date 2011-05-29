#
# makefile
#
#  Created on: 2011-3-2
#      Author: Lional King
#
#   Copyright (c) 2011 netsurfers
#


BASE_DIR = ../../..


.PHONY: all clean

all: clean
	for i in $(BASE_DIR)/sample/sample*; do [ -d $$i ] && cp makefile.sub $$i/makefile; done
	mv $(BASE_DIR)/sample/sample01_debug/furseal_logo.cpp $(BASE_DIR)/sample
	for i in $(BASE_DIR)/sample/sample*; do [ -d $$i ] && cp $(BASE_DIR)/sample/furseal_logo.cpp $$i/.; done
	rm $(BASE_DIR)/sample/furseal_logo.cpp
	for i in $(BASE_DIR)/sample/sample*; do [ -d $$i ] && make -C $$i; done

clean:
	rm -f $(BASE_DIR)/sample/bin/*
	rm -f $(BASE_DIR)/sample/sample*/*.o
	rm -f $(BASE_DIR)/sample/sample*/makefile
	mv $(BASE_DIR)/sample/sample01_debug/furseal_logo.cpp $(BASE_DIR)/sample
	rm -f $(BASE_DIR)/sample/sample*/furseal_logo.cpp
	mv $(BASE_DIR)/sample/furseal_logo.cpp $(BASE_DIR)/sample/sample01_debug
