# chisel8
Compact Hexadecimal Interpretive Programming – 8-bit (Chip-8 Interpreter), written in C with use of the [Raylib](https://github.com/raysan5/raylib) and [Raygui](https://github.com/raysan5/raygui) libraries.

### This project is a "Work in Progress"
TODO: Update Feature List

<div style="display: flex; flex-wrap: wrap; gap: 10px;">
  <img width="1392" alt="Menu" src="https://github.com/npxtune/chisel8/assets/42376598/8b0904e6-fcd9-45c4-b86f-4aa8b66d3da3", style="width: 30%; max-width: 200px;">
  <img width="1392" alt="IBM LOGO" src="https://github.com/npxtune/chisel8/assets/42376598/0d8103d3-1dfe-4b22-9c7f-8ee36eef8438", style="width: 30%; max-width: 200px;">
  <img width="1392" alt="Chip-8 TTS LOGO" src="https://github.com/npxtune/chisel8/assets/42376598/d0baf3a1-1352-40e5-9a67-c4a0b36c44bc", style="width: 30%; max-width: 200px;">
</div>

---
# Build instructions:
### MacOS & Linux:
```zsh
mkdir build && cd ./build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j 6 # '6' -> How many cores you want to use
./chisel8
```
That's it. Remember to pull the git submodules before building and install the necessary raylib + raygui dependencies for your platform.

---
### Windows:
No idea, good luck. If you figured it out, let me know per PR.
