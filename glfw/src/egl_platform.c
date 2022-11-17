#include "internal.h"

int _glfwPlatformInit(void)
{

    _glfwInitTimerPOSIX();
    return GLFW_TRUE;
}

void _glfwPlatformTerminate(void)
{
    _glfwTerminateEGL();
}

const char* _glfwPlatformGetVersionString(void)
{
    return _GLFW_VERSION_NUMBER " EGL ONLY";
}

int _glfwPlatformCreateWindow(_GLFWwindow* window,
                              const _GLFWwndconfig* wndconfig,
                              const _GLFWctxconfig* ctxconfig,
                              const _GLFWfbconfig* fbconfig)
{
    window->fb_window.width=wndconfig->width;
    window->fb_window.height=wndconfig->height;
    if (!_glfwInitEGL())
        return GLFW_FALSE;
    if (!_glfwCreateContextEGL(window, ctxconfig, fbconfig))
        return GLFW_FALSE;

    return GLFW_TRUE;
}
void _glfwPlatformGetGammaRamp(_GLFWmonitor* monitor, GLFWgammaramp* ramp)
{
    // TODO
    _glfwInputError(GLFW_PLATFORM_ERROR,
                    "egl only: Gamma ramp getting not supported yet");
}

void _glfwPlatformGetCursorPos(_GLFWwindow* window, double* xpos, double* ypos)
{
    _glfwInputError(GLFW_PLATFORM_ERROR,
                    "egl only: CursorPos getting not supported yet");
}

int _glfwPlatformWindowFocused(_GLFWwindow* window)
{
    return GLFW_FALSE;
}
void _glfwPlatformSetCursorMode(_GLFWwindow* window, int mode)
{
    _glfwInputError(GLFW_PLATFORM_ERROR,
                    "egl only: set Cursor mode getting not supported yet");
}
int _glfwPlatformGetKeyScancode(int key)
{
    return 0;
}
const char* _glfwPlatformGetScancodeName(int scancode)
{
    // TODO
    return NULL;
}
void _glfwPlatformSetCursor(_GLFWwindow* window, _GLFWcursor* cursor)
{
    _glfwInputError(GLFW_PLATFORM_ERROR,
                    "egl only: set Cursor getting not supported yet");
}
void _glfwPlatformDestroyCursor(_GLFWcursor* cursor)
{
     _glfwInputError(GLFW_PLATFORM_ERROR,
                    "egl only: destroy Cursor getting not supported yet");
}
int _glfwPlatformCreateCursor(_GLFWcursor* cursor,
                              const GLFWimage* image,
                              int xhot, int yhot)
{
    return 0;
}

int _glfwPlatformCreateStandardCursor(_GLFWcursor* cursor, int shape)
{
    return GLFW_TRUE;
}


int _glfwPlatformPollJoystick(_GLFWjoystick* js, int mode)
{
    return GLFW_FALSE;
}
void _glfwPlatformUpdateGamepadGUID(char* guid)
{
}

const char* _glfwPlatformGetClipboardString(void)
{
    // TODO
    _glfwInputError(GLFW_PLATFORM_ERROR,
                    "egl only: Clipboard getting not implemented yet");
    return NULL;
}
GLFWvidmode* _glfwPlatformGetVideoModes(_GLFWmonitor* monitor, int* found)
{
    *found = monitor->modeCount;
    return NULL;
}
void _glfwPlatformGetWindowSize(_GLFWwindow* window, int* width, int* height)
{
    if (width)
        *width = window->fb_window.width;
    if (height)
        *height = window->fb_window.height;

}
void _glfwPlatformSetWindowMonitor(_GLFWwindow* window,
                                   _GLFWmonitor* monitor,
                                   int xpos, int ypos,
                                   int width, int height,
                                   int refreshRate)
{
}

void _glfwPlatformGetWindowFrameSize(_GLFWwindow* window,
                                     int* left, int* top,
                                     int* right, int* bottom)
{
}
void _glfwPlatformGetWindowPos(_GLFWwindow* window, int* xpos, int* ypos)
{
}

void _glfwPlatformSetWindowPos(_GLFWwindow* window, int xpos, int ypos)
{
}

void _glfwPlatformFreeMonitor(_GLFWmonitor* monitor)
{
}

void _glfwPlatformGetMonitorPos(_GLFWmonitor* monitor, int* xpos, int* ypos)
{
}
void _glfwPlatformFocusWindow(_GLFWwindow* window)
{
}
void _glfwPlatformSetWindowTitle(_GLFWwindow* window, const char* title)
{
}

void _glfwPlatformSetWindowIcon(_GLFWwindow* window, int count,
                                const GLFWimage* images)
{
}

void _glfwPlatformSetWindowSizeLimits(_GLFWwindow* window,
                                      int minwidth, int minheight,
                                      int maxwidth, int maxheight)
{

}

void _glfwPlatformSetWindowAspectRatio(_GLFWwindow* window, int n, int d)
{
}

void _glfwPlatformGetFramebufferSize(_GLFWwindow* window, int* width, int* height)
{
    *width=window->fb_window.width;
    *height=window->fb_window.height;
}
void _glfwPlatformGetWindowContentScale(_GLFWwindow* window,
                                        float* xscale, float* yscale)
{
    if (xscale)
        *xscale = 1.f;
    if (yscale)
        *yscale = 1.f;
}
float _glfwPlatformGetWindowOpacity(_GLFWwindow* window)
{
    return 1.f;
}
void _glfwPlatformSetWindowOpacity(_GLFWwindow* window, float opacity)
{
}
void _glfwPlatformIconifyWindow(_GLFWwindow* window)
{
}

void _glfwPlatformRestoreWindow(_GLFWwindow* window)
{
}

void _glfwPlatformMaximizeWindow(_GLFWwindow* window)
{
}
void _glfwPlatformShowWindow(_GLFWwindow* window)
{
    
}
int _glfwPlatformFramebufferTransparent(_GLFWwindow* window)
{
    return GLFW_FALSE;
}
int _glfwPlatformWindowVisible(_GLFWwindow* window)
{
    return GLFW_TRUE;
}
int _glfwPlatformWindowMaximized(_GLFWwindow* window)
{
    return GLFW_TRUE;
}
int _glfwPlatformWindowIconified(_GLFWwindow* window)
{
    return GLFW_FALSE;
}

void _glfwPlatformSetWindowResizable(_GLFWwindow* window, GLFWbool enabled)
{
}

void _glfwPlatformSetWindowDecorated(_GLFWwindow* window, GLFWbool enabled)
{
}

void _glfwPlatformSetWindowFloating(_GLFWwindow* window, GLFWbool enabled)
{
}
void _glfwPlatformPollEvents(void)
{
}

void _glfwPlatformWaitEvents(void)
{
}

void _glfwPlatformWaitEventsTimeout(double timeout)
{
}

void _glfwPlatformPostEmptyEvent(void)
{
}
void _glfwPlatformSetGammaRamp(_GLFWmonitor* monitor, const GLFWgammaramp* ramp)
{
}
void _glfwPlatformSetCursorPos(_GLFWwindow* window, double x, double y)
{
}
void _glfwPlatformSetClipboardString(const char* string)
{
}
void _glfwPlatformGetMonitorContentScale(_GLFWmonitor* monitor,
                                         float* xscale, float* yscale)
{
    if (xscale)
        *xscale = 1.f;
    if (yscale)
        *yscale = 1.f;
}
void _glfwPlatformGetVideoMode(_GLFWmonitor* monitor, GLFWvidmode* mode)
{
}
void _glfwPlatformSetWindowSize(_GLFWwindow* window, int width, int height)
{
}
void _glfwPlatformRequestWindowAttention(_GLFWwindow* window)
{
}
void _glfwPlatformHideWindow(_GLFWwindow* window)
{   
}
void _glfwPlatformDestroyWindow(_GLFWwindow* window)
{
    if (window->context.destroy)
        window->context.destroy(window);
}
