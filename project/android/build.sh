#!/bin/sh
#
#   Copyright (c) 2011 netsurfers
#



cd `dirname $0`

ndk-build

cp obj/local/armeabi/libfurseal.a ../../library/
rm -rf libs obj

