#!/bin/bash

ARGUMENT=$1

make $ARGUMENT CROSS_COMPILE=/opt/toolchains/arm-2009q3/bin/arm-none-linux-gnueabi- ARCH=arm
