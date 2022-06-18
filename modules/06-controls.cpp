// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


#include "headers/common.hpp"
#include "headers/controls.hpp"

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
	
    // Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

    // Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

    /*--------------------------------------------------------------------------------------------------*/

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, WIDTH/2, HEIGHT/2);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    /*--------------------------------------------------------------------------------------------------*/

    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    // read vertex shader 
    std::string const vertexShaderPath = "./resources/shaders/transformVertexShader-texture.vert";
    std::string vertexShaderCode = readShaderFile(vertexShaderPath);
    //compile vertex shader
    compileShader(programId, vertexShaderId, vertexShaderCode, result, infoLogLength);

    /*--------------------------------------------------------------------------------------------------*/

    fragmentShaderId =  glCreateShader(GL_FRAGMENT_SHADER);
    // read fragment shader 
    std::string const fragmentShaderPath = "./resources/shaders/textureFragmentShader.frag";
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

    /*--------------------------------------------------------------------------------------------------*/

    // Load the texture using any two methods
	// GLuint texture = loadBMP("./resources/textures/uvtemplate.bmp");
	GLuint texture = loadDDS("./resources/textures/uvtemplate.DDS");
	
	// Get a handle for our "myTextureSampler" uniform
	GLuint textureId = glGetUniformLocation(programId, "myTextureSampler");

    /*--------------------------------------------------------------------------------------------------*/
    
    // we need three 3D points in order to make a triangle
    // An array of 3 vectors which represents 3 vertices
    static const GLfloat *g_vertex_buffer_data = cubeTextured.vertexBufferData;
    // One color for each vertex. They were generated randomly.
    static const GLfloat *g_color_buffer_data = cubeTextured.uvBufferData;


    /* --- Drawing Triangle --- */

    // vertex buffer object
    GLuint vbo; // identify vertex buffer object
    glGenBuffers(1, &vbo); // create vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // bound the vertex buffer
    glBufferData( // fill the vertex buffer
        GL_ARRAY_BUFFER, 
        cubeTextured.bufferDataSize * sizeof(float), 
        g_vertex_buffer_data, 
        GL_STATIC_DRAW
    );

    // color buffer object or uvbuffer
	GLuint cbo; // identify color buffer object
	glGenBuffers(1, &cbo); //create color buffer
	glBindBuffer(GL_ARRAY_BUFFER, cbo); // bound the color buffer
	glBufferData( // fill the color buffer
        GL_ARRAY_BUFFER, 
        cubeTextured.bufferDataSize * sizeof(float), 
        g_color_buffer_data, 
        GL_STATIC_DRAW
    );

    GLuint vao; // vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);


    GLint vertexPositionAttr = glGetAttribLocation(programId, "vertexPosition_modelspace");
    GLint vertexUV = glGetAttribLocation(programId, "vertexUV");

    /*--------------------------------------------------------------------------------------------------*/

    do{ // Rendering
        
        // Clear the screen. 
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        // use shader program
        glUseProgram(programId);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs(window, WIDTH, HEIGHT);
		glm::mat4 projectionMatrix = getProjectionMatrix();
		glm::mat4 viewMatrix = getViewMatrix();
		glm::mat4 modelMatrix = glm::mat4(1.0);
		glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(textureId, 0);

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
		glEnableVertexAttribArray(vertexUV);
		glBindBuffer(GL_ARRAY_BUFFER, cbo);
		glVertexAttribPointer(
			vertexUV,                         // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 12*3); // Starting from vertex 0; 3 vertices total -> 1 triangle
        
        glDisableVertexAttribArray(vertexPositionAttr);
        glDisableVertexAttribArray(vertexUV);

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
    //delete texture
    glDeleteTextures(1, &texture);
    // delete program
	glDeleteProgram(programId);
    // terminate glfw
    glfwTerminate();

    /*--------------------------------------------------------------------------------------------------*/
    
    return 0;
}