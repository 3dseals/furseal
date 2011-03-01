#!/bin/sh
#
#   Copyright (c) 2011 netsurfers
#


cd `dirname $0`

rm -f ../../library/*
rm -rf ../../documents/reference/html

make -C furseal clean
