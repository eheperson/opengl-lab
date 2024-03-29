#include<iostream>

// #define GLEW_BUILD

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <OpenGL/glext.h>
#else
    #include <GL/glew.h>
#endif

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>

int main(int arc, char ** argv){

    
    /* Initialise GLFW*/    
    #ifndef __APPLE__
        glewExperimental = true; // Needed for core profile
    #endif
    if( !glfwInit() ){
        std::cout << stderr <<"Failed to initialize GLFW\n";
        return -1;
    }

    /*--------------------------------------------------------------------------------------------------*/

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing : we have 4 samples in each pixel
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

    // Open a window and create its OpenGL context
    GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)

    window = glfwCreateWindow( 1024, 768, "Tutorial 01", NULL, NULL);

    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    };

    glfwMakeContextCurrent(window); // Initialize GLEW


    #ifndef __APPLE__
        glewExperimental=true; // Needed in core profile
        if (glewInit() != GLEW_OK) {
            fprintf(stderr, "Failed to initialize GLEW\n");
            return -1;
        };
    #endif

    /*--------------------------------------------------------------------------------------------------*/

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    do{
        // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
        glClear( GL_COLOR_BUFFER_BIT );

        // Draw nothing, see you in tutorial 2 !

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0 );

    return 0;
}