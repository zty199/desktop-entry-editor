#!/bin/sh

realpath="`realpath $0`"
appname="`basename $realpath | cut -d. -f1`"
dirname="`dirname $realpath`"

tmp="${dirname#?}"
if [ "${dirname%$tmp}" != "/" ]; then
    dirname=$PWD/$dirname
fi

export LD_LIBRARY_PATH="$dirname/../lib":$LD_LIBRARY_PATH

$dirname/$appname "$@"
