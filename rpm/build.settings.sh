#!/bin/bash

# Common settings script.

TURNVERSION=4.5.1.2
BUILDDIR=~/rpmbuild
ARCH=`uname -p`

WGETOPTIONS="--no-check-certificate"
RPMOPTIONS="-ivh --force"


