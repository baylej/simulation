cmake_minimum_required(VERSION 3.15)

project(simulation VERSION 1.0.0 LANGUAGES C CXX)

if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
  set(EMSCRIPTEN True)
endif()

file(GLOB imgui_source deps/imgui/*.cpp)

add_executable(simulation
  src/main.cpp

  src/contexts/menu.cpp

  src/3D/renderer.cpp
  src/3D/mesh.cpp

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
  target_link_options(simulation PRIVATE "SHELL:-s USE_GLFW=3 -s FULL_ES3=1")
  # GLFW is provided by emscripten
  #target_link_options(simulation PRIVATE --preload-file data)
else()
  find_package(glfw3 REQUIRED)
  include(FindOpenGL)
  find_package(OpenGL REQUIRED)
  target_link_libraries(simulation PRIVATE glfw OpenGL::GL)
endif()

set_target_properties(simulation PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED On)
