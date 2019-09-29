#!/bin/bash

function WHITE_TEXT {
  printf "\033[1;37m"
}
function NORMAL_TEXT {
  printf "\033[0m"
}
function GREEN_TEXT {
  printf "\033[1;32m"
}
function RED_TEXT {
  printf "\033[1;31m"
}

WHITE_TEXT
echo "########################################################################################"
echo "# Building Get Raw Frame...                                                            #"
echo "########################################################################################"
NORMAL_TEXT

uname -a

TARGET_BUILD_FOLDER=../build
OUTPUT_DIRECTORY=/tmp/imgs

mkdir $OUTPUT_DIRECTORY
mkdir $TARGET_BUILD_FOLDER
mkdir $TARGET_BUILD_FOLDER/raw_frame_server

rm $TARGET_BUILD_FOLDER/raw_frame_server/*
cd ../src/host/libpixyusb2_examples/raw_frame_server
make clean
make
mv ./raw_frame_server ../../../../build/raw_frame_server
mv ./raw_frame_dump ../../../../build/raw_frame_server

if [ -f ../../../../build/raw_frame_server/raw_frame_server ]; then
  GREEN_TEXT
  printf "SUCCESS "
else
  RED_TEXT
  printf "FAILURE "
fi
echo ""
