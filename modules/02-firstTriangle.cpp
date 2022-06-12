#include<string>
#include<stdio.h>
#include<stdlib.h>
#include <iostream>
#include<vector>
#include<sstream>
#include<fstream>

// #define GLEW_BUILD

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

#include "headers/utils.hpp"

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

    // you need to create a Vertex Array Object and set it as the current one
    // Do this once your window is created 
    // (= after the OpenGL Context creation) and before any other OpenGL call.
    GLuint VertexArrayID;
    #ifdef __APPLE__
        glGenVertexArraysAPPLE(1, &VertexArrayID);
        glBindVertexArrayAPPLE(VertexArrayID);
    #else
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);
    #endif


    // we need three 3D points in order to make a triangle
    // An array of 3 vectors which represents 3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            0.0f,  1.0f, 0.0f,
    };


    static const GLfloat g_vertex_buffer_data2[] = {
            -10.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            0.0f,  1.0f, 0.0f,
            0.0f,  10.0f, -1.0f,
            -4.0f,  10.0f, -1.0f,

	-0.9f, -0.9f, 0.0f,
    };
    /* --- Drawing Triangle --- */
    // We do this by creating a buffer:
    // This will identify our vertex buffer
    // This needs to be done only once. ----------------------------------------- !!!!!!!
    GLuint vertexbuffer;

    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);

    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

    // Give our vertices to OpenGL.
    // glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


    /*--------------------------------------------------------------------------------------------------*/

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    /*--------------------------------------------------------------------------------------------------*/

    // get shaders 
    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders("./resources/triangle.vert", "./resources/triangle.frag" );

    /*--------------------------------------------------------------------------------------------------*/

    int i= 0;
    do{
        i ++ ;
        // Clear the screen. 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        if(i<100){

            glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
        }
        else{

            glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data2), g_vertex_buffer_data2, GL_STATIC_DRAW);
        }

        // tell OpenGL that you want to use your shader
        glUseProgram(programID);

        // Draw triangle here

        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        
        /*
            OpenGL doesn’t know what a colour is : it just sees a vec3. 
            So we have to tell him which buffer corresponds to which input. 
            
            We do that by setting the layout to the same value as the first parameter to 
                glVertexAttribPointer. 

            The value “0” is not important, 
            it could be 12 
            (but no more than glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &v))

            the important thing is that it’s the same number on both sides. :

                    layout(location = 0) in vec3 vertexPosition_modelspace;
        */

        glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(0);

        // Draw triangle here ---------------------------------------------------------------------------

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0 );

    return 0;
}