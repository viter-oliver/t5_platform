# Specify the minimum version for CMake

cmake_minimum_required(VERSION 2.8)

# Project's name

project(GLFW C CXX)
set(GLFW_LIB_NAME glfw)
set(GLFW_VERSION_MAJOR "3")
set(GLFW_VERSION_MINOR "2")
set(GLFW_VERSION "${GLFW_VERSION_MAJOR}.${GLFW_VERSION_MINOR}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -fpermissive")
option(_GLFW_WAYLAND "Use Wayland for window creation" OFF)
option(_EGL_ONLY "Use egl only for window creation" ON)
option(_GLFW_SCREEN "Use Screen for window creation" OFF)
message(sysroot=${CMAKE_SYSROOT})
message(toolchainfile=${CMAKE_TOOLCHAIN_FILE})
message(find_path=${CMAKE_FIND_ROOT_PATH})
message(framework=${CMAKE_FRAMEWORK_PATH})
add_definitions( -DEGL_API_FB  -D_GLFW_USE_CONFIG_H -DIMGUI_PLATFORM)
if (_GLFW_WAYLAND)
    add_definitions(-DLINUX -DWL_EGL_PLATFORM  -D_GLFW_WAYLAND)
   
elseif(_EGL_ONLY)
    include_directories(
    /opt/t5/aarch64-buildroot-linux-gnu/sysroot/usr/include
    /opt/t5/aarch64-buildroot-linux-gnu/sysroot/usr/sdk_lib/include/opengl/include
    /opt/t5/aarch64-buildroot-linux-gnu/sysroot/usr/sdk_lib/include/opengl/include/khronos/original)
    add_definitions(-DLINUX -D_GLFW_EGL -DGLFW_EXPOSE_NATIVE_EGL -D_GLFW_USE_EGLPLATFORM_H)
    
elseif(_GLFW_SCREEN)
    add_definitions(-D_GLFW_SCREEN)
    
endif()
add_subdirectory(json_lib)
add_subdirectory(src)
#add_subdirectory(freetype)
add_subdirectory(imgui)
#add_subdirectory(msg_host)
#add_subdirectory(graphics_app)
