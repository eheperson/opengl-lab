#include <iostream>

#include <rpppConfig.h>

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <OpenGL/glext.h>
#else /// your stuff for linux
    #include <GL/glew.h>
#endif

// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>

int main(int arc, char ** argv) {

    const std::string inputName = "Mr. Anderson";

    std::cout << " 'ellooooo " << inputName << std::endl;

    return 0;
}
