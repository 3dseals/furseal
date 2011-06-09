#!/bin/sh
#
#   Copyright (c) 2011 netsurfers
#



cd `dirname $0`

ndk-build

cp obj/local/armeabi/libfurseal.a ../../library
cp library/libfreetype.a ../../library
cp library/libpng.a ../../library
cp library/libz.a ../../library
