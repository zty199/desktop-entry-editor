#!/bin/bash
# this file is used to auto-generate .qm file from .ts file.

cd $(dirname $0)

PROJECT_NAME=$1
lrelease ./src/${PROJECT_NAME}.pro
