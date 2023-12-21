# chisel8
Compact Hexadecimal Interpretive Programming – 8-bit (Chip-8 Interpreter), written in C with use of the [Raylib](https://github.com/raysan5/raylib) and [Raygui](https://github.com/raysan5/raygui) libraries.

### This project is a "Work in Progress"
It currently only has a GUI and is able to read in ROM files + prints out their content.

---
# Build instructions:
### *NIX:
```zsh
mkdir build && cd ./build
cmake ..
make -j 6 # '6' -> How many cores you want to use
./chisel8
```
That's it. Remember to pull the git submodules before building and install the necessary raylib + raygui dependencies for your platform.

---
### Windows:
No idea, good luck. If you figured it out, let me know per PR.
