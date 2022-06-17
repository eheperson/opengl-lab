#include "headers/common.hpp"

int main(int arc, char ** argv){

    GLint result = GL_FALSE;
	int infoLogLength;
    GLuint programId;
    GLuint vertexShaderId;
    GLuint fragmentShaderId;
    GLFWwindow *window;
    
    /*--------------------------------------------------------------------------------------------------*/
    
    window = init(WIDTH, HEIGHT, WINDOW_NAME);
    driverInfo();
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    programId = glCreateProgram();

    /*--------------------------------------------------------------------------------------------------*/

    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    // read vertex shader 
    std::string const vertexShaderPath = "./resources/triangle.vert";
    std::string vertexShaderCode = readShaderFile(vertexShaderPath);
    //compile vertex shader
    compileShader(programId, vertexShaderId, vertexShaderCode, result, infoLogLength);

    /*--------------------------------------------------------------------------------------------------*/

    fragmentShaderId =  glCreateShader(GL_FRAGMENT_SHADER);
    // read fragment shader 
    std::string const fragmentShaderPath = "./resources/triangle.frag";
    std::string fragmentShaderCode = readShaderFile(fragmentShaderPath);
    //compile vertex shader
    compileShader(programId, fragmentShaderId, fragmentShaderCode, result, infoLogLength);

    /*--------------------------------------------------------------------------------------------------*/

    linkProgram(programId, result, infoLogLength);
    cleanShader(programId, vertexShaderId);
    cleanShader(programId, fragmentShaderId);

    /*--------------------------------------------------------------------------------------------------*/
    
    // we need three 3D points in order to make a triangle
    // An array of 3 vectors which represents 3 vertices
    static const GLfloat *g_vertex_buffer_data = triangle.vertexBufferData;
    
    /* --- Drawing Triangle --- */
    // We do this by creating a buffer:
    // This will identify our vertex buffer
    // This needs to be done only once. ----------------------------------------- !!!!!!!
    GLuint vbo;
    // Generate 1 buffer, put the resulting identifier in vbo
    glGenBuffers(1, &vbo);
    // The following commands will talk about our 'vbo' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, triangle.bufferDataSize * sizeof(float), g_vertex_buffer_data, GL_STATIC_DRAW);

    // you need to create a Vertex Array Object and set it as the current one
    // Do this once your window is created 
    // (= after the OpenGL Context creation) and before any other OpenGL call.
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

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
    GLint vertesPositionAttr = glGetAttribLocation(programId, "vertexPosition_modelspace");
    glVertexAttribPointer(
        vertesPositionAttr, // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0           // array buffer offset
    );
    glEnableVertexAttribArray(vertesPositionAttr);

    // 1st attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    /*--------------------------------------------------------------------------------------------------*/

    while (!glfwWindowShouldClose(window)){ // Rendering
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
        // Clear the screen. 
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        // tell OpenGL that you want to use your shader
        glUseProgram(programId);
        // bind vao
        glBindVertexArray(vao);
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
        // Swap front and back buffers
        glfwSwapBuffers(window);
        // Poll for and process events
        glfwPollEvents();
    }

    /*--------------------------------------------------------------------------------------------------*/

    // Cleanup vbp
	glDeleteBuffers(1, &vbo);
    // cleanup vao
	glDeleteVertexArrays(1, &vao);
    // delete program
	glDeleteProgram(programId);
    // terminate glfw
    glfwTerminate();

    /*--------------------------------------------------------------------------------------------------*/
    
    return 0;
}