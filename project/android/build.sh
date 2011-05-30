#!/bin/sh
#
#   Copyright (c) 2011 netsurfers
#



cd `dirname $0`

ndk-build

cp bin/ndk/local/armeabi ../../library/

