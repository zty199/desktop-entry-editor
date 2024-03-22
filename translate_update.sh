#!/bin/bash
# this file is used to auto-update .ts file.

cd $(dirname $0)

PROJECT_NAME=$1
lupdate ./src/${PROJECT_NAME}.pro -no-obsolete
