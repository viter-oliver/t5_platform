   

set(common_HEADERS internal.h
                   "${GLFW_SOURCE_DIR}/include/GLFW/glfw3.h"
                   "${GLFW_SOURCE_DIR}/include/GLFW/glfw3native.h")
set(common_SOURCES context.c init.c input.c monitor.c  window.c)
if (_GLFW_WAYLAND)
    set(glfw_HEADERS ${common_HEADERS} wl_platform.h linux_joystick.h choose_config.h
                    posix_time.h posix_thread.h xkb_unicode.h egl_context.h)
    set(glfw_SOURCES ${common_SOURCES} wl_init.c wl_monitor.c wl_window.c
                     linux_joystick.c posix_time.c posix_thread.c xkb_unicode.c
                     egl_context.c choose_config.c)
    set(target_lib wayland-client  wayland-server wayland-cursor 
    wayland-egl EGL GLESv2 m pthread dl freetype)
elseif(_EGL_ONLY)
    set(glfw_HEADERS ${common_HEADERS} linux_joystick.h choose_config.h
                    posix_time.h posix_thread.h xkb_unicode.h egl_context.h egl_platform.h)
    set(glfw_SOURCES ${common_SOURCES}  posix_time.c posix_thread.c xkb_unicode.c
                     egl_context.c egl_platform.c choose_config.c)
    set(target_lib EGL GLESv2 m pthread dl freetype mali)
elseif(_GLFW_SCREEN)
    set(glfw_HEADERS ${common_HEADERS} screen_platform.h screen_joystick.h choose_config.h
                    posix_time.h posix_thread.h xkb_unicode.h egl_context.h)
    set(glfw_SOURCES ${common_SOURCES} screen_init.c screen_monitor.c screen_window.c
                     screen_joystick.c posix_time.c posix_thread.c xkb_unicode.c
                     egl_context.c  choose_config.c)
endif()

add_library(glfw ${glfw_SOURCES} ${glfw_HEADERS})
target_link_libraries(glfw ${target_lib})

set_target_properties(glfw PROPERTIES
                      OUTPUT_NAME ${GLFW_LIB_NAME}
                      VERSION ${GLFW_VERSION}
                      SOVERSION ${GLFW_VERSION_MAJOR}
                      POSITION_INDEPENDENT_CODE ON)

set(LIB_OUTPUT_PATH ${GLFW_SOURCE_DIR}/../HSG_SOLUTION/hsg_sdk/lib)
set(LIBRARY_OUTPUT_PATH ${LIB_OUTPUT_PATH})