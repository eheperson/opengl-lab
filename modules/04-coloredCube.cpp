#include "headers/common.hpp"

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

    // Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

    /*--------------------------------------------------------------------------------------------------*/

    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    // read vertex shader 
    std::string const vertexShaderPath = "./resources/shaders/transformVertexShader.vert";
    std::string vertexShaderCode = readShaderFile(vertexShaderPath);
    //compile vertex shader
    compileShader(programId, vertexShaderId, vertexShaderCode, result, infoLogLength);

    /*--------------------------------------------------------------------------------------------------*/

    fragmentShaderId =  glCreateShader(GL_FRAGMENT_SHADER);
    // read fragment shader 
    std::string const fragmentShaderPath = "./resources/shaders/colorFragmentShader.frag";
    std::string fragmentShaderCode = readShaderFile(fragmentShaderPath);
    //compile vertex shader
    compileShader(programId, fragmentShaderId, fragmentShaderCode, result, infoLogLength);

    /*--------------------------------------------------------------------------------------------------*/

    linkProgram(programId, result, infoLogLength);
    cleanShader(programId, vertexShaderId);
    cleanShader(programId, fragmentShaderId);

    /*--------------------------------------------------------------------------------------------------*/
    

	// Get a handle for our "MVP" uniform
	GLuint matrixId = glGetUniformLocation(programId, "MVP");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	
    // Camera matrix
	glm::mat4 view       = glm::lookAt(
								glm::vec3(4,3,-3), // Camera is at (4,3,-3), in World Space
								glm::vec3(0,0,0), // and looks at the origin
								glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 model      = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 mvp        = projection * view * model; // Remember, matrix multiplication is the other way around

    
    /*--------------------------------------------------------------------------------------------------*/
    
    // we need three 3D points in order to make a triangle
    // An array of 3 vectors which represents 3 vertices
    static const GLfloat *g_vertex_buffer_data = cube.vertexBufferData;
    // One color for each vertex. They were generated randomly.
    static const GLfloat *g_color_buffer_data = cube.colorBufferData;


    /* --- Drawing Triangle --- */

    // vertex buffer object
    GLuint vbo; // identify vertex buffer object
    glGenBuffers(1, &vbo); // create vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // bound the vertex buffer
    glBufferData( // fill the vertex buffer
        GL_ARRAY_BUFFER, 
        cube.bufferDataSize * sizeof(float), 
        g_vertex_buffer_data, 
        GL_STATIC_DRAW
    );

    // color buffer object
	GLuint cbo; // identify color buffer object
	glGenBuffers(1, &cbo); //create color buffer
	glBindBuffer(GL_ARRAY_BUFFER, cbo); // bound the color buffer
	glBufferData( // fill the color buffer
        GL_ARRAY_BUFFER, 
        cube.bufferDataSize * sizeof(float), 
        g_color_buffer_data, 
        GL_STATIC_DRAW
    );

    GLuint vao; // vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);


    GLint vertexPositionAttr = glGetAttribLocation(programId, "vertexPosition_modelspace");
    GLint vertexColor = glGetAttribLocation(programId, "vertexColor");

    /*--------------------------------------------------------------------------------------------------*/

    do{ // Rendering
        
        // fill the screen
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
        
        // Clear the screen. 
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        // use shader program
        glUseProgram(programId);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(vertexPositionAttr);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(
            vertexPositionAttr, // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0           // array buffer offset
        );

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(vertexColor);
		glBindBuffer(GL_ARRAY_BUFFER, cbo);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 12*3); // Starting from vertex 0; 3 vertices total -> 1 triangle
        
        glDisableVertexAttribArray(vertexPositionAttr);
        glDisableVertexAttribArray(vertexColor);

        // Swap front and back buffers
        glfwSwapBuffers(window);
        // Poll for and process events
        glfwPollEvents();

    } while (!glfwWindowShouldClose(window));
    // // Check if the ESC key was pressed or the window was closed
    // } while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );

    /*--------------------------------------------------------------------------------------------------*/

    // Cleanup vbo
	glDeleteBuffers(1, &vbo);
    // Cleanup vco
	glDeleteBuffers(1, &cbo);
    // cleanup vao
	glDeleteVertexArrays(1, &vao);
    // delete program
	glDeleteProgram(programId);
    // terminate glfw
    glfwTerminate();

    /*--------------------------------------------------------------------------------------------------*/
    
    return 0;
}