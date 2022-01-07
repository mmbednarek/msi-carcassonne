#!/usr/bin/env bash

PROTO_SOURCES_PATH="src/Proto/api.cmake"

proto_sources=$(find . -path "./api/proto/*.proto")

echo $proto_sources

protoc --proto_path=./api/proto --cpp_out=src/Proto $proto_sources

# copy generated headers to include path
for header in $(find . -path "./src/Proto/*.pb.h"); do
  echo "coping $header to include/Carcassonne/Proto"
  cp $header ./include/Carcassonne/Proto
done

SOURCES_SCRIPT="set(proto_sources \n"
for source in $(find . -path "./src/Proto/*.pb.cc"); do
  SOURCES_SCRIPT+="\t$(basename $source)\n"
  echo "adding $source to $PROTO_SOURCES_PATH"
done
SOURCES_SCRIPT+=")"
echo -ne $SOURCES_SCRIPT >$PROTO_SOURCES_PATH
