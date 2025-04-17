#!/bin/bash
cmake -S . -B ./build
cmake --build ./build
rm ./compile_commands.json
ln ./build/compile_commands.json ./compile_commands.json
echo "linking compile_commands.json"