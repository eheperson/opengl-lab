#pragma once

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #define GLFW_INCLUDE_GLCOREARB
    #include <OpenGL/gl.h>
    #include <OpenGL/glext.h>
#else /// your stuff for linux
    #include <GL/glew.h>
#endif

// Include GLFW
#include <GLFW/glfw3.h>

#include "utils.hpp"
#include "data.hpp"

const size_t WIDTH = 1024;
const size_t HEIGHT = 768;
const char* WINDOW_NAME = "Test OpenGL";