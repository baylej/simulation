cmake_minimum_required(VERSION 3.15)

project(simulation VERSION 1.0.0 LANGUAGES C CXX)

if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
  set(EMSCRIPTEN True)
endif()

file(GLOB imgui_source deps/imgui/*.cpp)

add_executable(simulation
  src/main.cpp

  src/contexts/menu.cpp
  src/contexts/scene_3d.cpp

  src/renderer/camera.cpp
  src/renderer/renderer.cpp
  src/renderer/mesh.cpp
  src/renderer/utils.cpp

  ${imgui_source})

target_include_directories(simulation PRIVATE deps)

target_compile_definitions(simulation
  PRIVATE IMGUI_IMPL_OPENGL_ES3
  PRIVATE GLFW_INCLUDE_NONE)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  target_compile_definitions(simulation GLM_FORCE_INLINE)
endif()

if(EMSCRIPTEN)
  target_compile_definitions(simulation PRIVATE EMSCRIPTEN)
  set_target_properties(simulation PROPERTIES SUFFIX ".html")
  target_link_options(simulation PRIVATE "SHELL:-s USE_GLFW=3 -s FULL_ES3=1 -s MAX_WEBGL_VERSION=2")
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    # EXCEPTION_DEBUG=1 prints a stacktrace on exception catching
    target_compile_options(simulation PRIVATE "SHELL:-s EXCEPTION_DEBUG=1 -s DISABLE_EXCEPTION_CATCHING=0 -s DEMANGLE_SUPPORT=1")
    target_link_options(simulation PRIVATE "SHELL:-s EXCEPTION_DEBUG=1 -s DISABLE_EXCEPTION_CATCHING=0 -s DEMANGLE_SUPPORT=1")
  endif()
  # GLFW is provided by emscripten
  #target_link_options(simulation PRIVATE --preload-file data)
else()
  find_package(glfw3 REQUIRED)
  include(FindOpenGL)
  find_package(OpenGL REQUIRED)
  target_link_libraries(simulation PRIVATE glfw OpenGL::GL)
endif()

set_target_properties(simulation PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED On)
