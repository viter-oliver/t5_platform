cmake_minimum_required(VERSION 2.8)
project(HSG_SL C CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -fpermissive")
option(_GLFW_WAYLAND "Use Wayland for window creation" OFF)
option(_EGL_ONLY "Use egl only for window creation" ON)
option(_GLFW_SCREEN "Use Screen for window creation" OFF)
if (_GLFW_WAYLAND)
    add_definitions(-DLINUX 
    -DWL_EGL_PLATFORM  
    -D_GLFW_WAYLAND)
   
elseif(_EGL_ONLY)
    include_directories(
    /opt/t5/aarch64-buildroot-linux-gnu/sysroot/usr/include
    /opt/t5/aarch64-buildroot-linux-gnu/sysroot/usr/sdk_lib/include/opengl/include
    /opt/t5/aarch64-buildroot-linux-gnu/sysroot/usr/sdk_lib/include/opengl/include/khronos/original)
    add_definitions(-DLINUX -D_GLFW_EGL -DGLFW_EXPOSE_NATIVE_EGL -D_GLFW_USE_EGLPLATFORM_H)
    
elseif(_GLFW_SCREEN)
    add_definitions(-D_GLFW_SCREEN)  
endif()
add_subdirectory(hst_ar_hud)
