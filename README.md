# SCEd - Simple Creative Engine (for design)
A base OpenGL project for 2D/3D grapics

---

## Dependencies
This project already ships with:
- **GLAD**
- **GLM**
- **GLFW** (in 'libs/' for Linux, macOS, Windows)

So you don't need to install anything extra

---

## Building

### Linux / macOS

``` bash
cmake -S . -B build
cmake --build build
./build/sced
```

### Windows

``` bash
cmake -S . -B build
cmake --build build --config Release
.\build\Release\sced.exe
```
