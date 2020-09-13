Simulation
==========

A set of physics simulations.

### Dependencies

* [GLFW3](https://www.glfw.org/)
* [Dear ImGui](https://github.com/ocornut/imgui) (embedded)
* [GLM](https://glm.g-truc.net/) (embedded)

### Targets

* Native OpenGL ES3
* HTML5 + WebGL2 ([emscripten](https://emscripten.org/))

### Cloning

```shell script
git clone --recursive
```

### Compiling

Native:

```shell script
mkdir build && cd build
cmake ..
make
./simulation
```

HTML5:

```shell script
source path/to/emsdk_env.sh
mkdir build-web && cd build-web
emcmake cmake ..
emmake make
emrun --serve_after_close simulation.html
```
