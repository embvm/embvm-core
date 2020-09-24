#!/bin/bash

INITIAL_DIR=$(pwd)
cd /tmp
git clone git@github.com:embeddedartistry/config-files --depth 1
cd config-files
bash copy_config.sh
ls copy_config.sh
cd ../
rm -rf config-files
cd $INITIAL_DIR
