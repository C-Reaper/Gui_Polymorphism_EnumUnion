# Project README

## Overview
This project demonstrates the use of polymorphism in C programming using enums and unions. It includes a simple GUI application that can be built for Linux, Windows, Wine, and WebAssembly.

## Features
- Polymorphism using enums and unions
- Basic GUI functionality (drawing shapes, handling user input)
- Cross-platform build support

## Project Structure
```
Gui_Polymorphism_EnumUnion/
├── build/              # .exe files produced by Main.c
├── src/                # Source code
│   ├── Main.c          # Entry point
│   └── *.h             # Header-based C-files, without *.c files that implement it
├── Makefile.linux      # Linux Build configuration
├── Makefile.windows    # Windows Build configuration
├── Makefile.wine       # Wine Build configuration
└── README.md           # This file
```

### Prerequisites
- C/C++ Compiler and Debugger (GCC, Clang)
- Make utility
- Standard development tools
- X11 library for Linux builds

## Build & Run
### Linux
To build the project on Linux:
```sh
cd Gui_Polymorphism_EnumUnion/
make -f Makefile.linux all
```
To run the application:
```sh
./build/Main
```

### Windows
To build the project on Windows:
```sh
cd Gui_Polymorphism_EnumUnion/
make -f Makefile.windows all
```
To run the application:
```sh
.\build\Main.exe
```

### Wine
To build the project for Windows using Wine:
```sh
cd Gui_Polymorphism_EnumUnion/
make -f Makefile.wine all
```
To run the application:
```sh
wine ./build/Main.exe
```

### WebAssembly (Emscripten)
To build the project for WebAssembly:
```sh
cd Gui_Polymorphism_EnumUnion/
make -f Makefile.web all
```
To serve and run the application:
```sh
emrun --no_browser --port 8080 ./build/index.html
```

### Build Options
- `make -f Makefile.(os) all`: Build output
- `make -f Makefile.(os) do`: Build + executable output
- `make -f Makefile.(os) clean`: Remove build artifacts