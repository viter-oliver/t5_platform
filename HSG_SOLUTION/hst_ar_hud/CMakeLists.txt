include_directories(${HSG_SL_SOURCE_DIR}/hsg_sdk/include
${HSG_SL_SOURCE_DIR}/hsg_sdk/include/imgui
${HSG_SL_SOURCE_DIR}/hsg_sdk/include/imgui/fab
${HSG_SL_SOURCE_DIR}/hsg_sdk/include/imgui/af_include
${HSG_SL_SOURCE_DIR}/hsg_sdk/include/imgui/glm
${HSG_SL_SOURCE_DIR}/../boost_1_65_1
$ENV{SDKTARGETSYSROOT}/usr/include/freetype2
)
message("hsg_sl_dir:${HSG_SL_SOURCE_DIR}")
link_directories(${HSG_SL_SOURCE_DIR}/hsg_sdk/lib)
if(_GLFW_SCREEN AND QNX_VERSION<=660)
add_definitions(-DGLFW_INCLUDE_ES2)
else()
add_definitions(-DGLFW_INCLUDE_ES3)
endif()
add_definitions(-DIMGUI_DISABLE_DEMO_WINDOWS)
file(GLOB GP_CPP ./*.cpp)

if (_GLFW_WAYLAND)
    set(target_lib wayland-client  wayland-server wayland-cursor 
    wayland-egl EGL GLESv2 m pthread dl freetype)
elseif(_EGL_ONLY)
    link_directories(/opt/T5/aarch64-linux/usr/lib)
    set(target_lib EGL GLESv2 m pthread dl freetype mali)
elseif(_GLFW_SCREEN)
    set(target_lib EGL GLESv2 m pthread dl freetype screen)
endif()
add_executable(hst_rehearsal ${GP_CPP})
set_target_properties(hst_rehearsal PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(hst_rehearsal imgui_lib glfw json_lib ${target_lib})
