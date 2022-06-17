#include "headers/common.hpp"

static void quitCallback(GLFWwindow *window, int key, int scancode, int action, int _mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
};


int main(int arc, char ** argv){

    GLFWwindow *window;

    if (!glfwInit()) {
        std::cerr << "ERROR: could not start GLFW3" << std::endl;
        return -1; // Initialize the lib
    }

    // Minimum target is OpenGL 4.1
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing : we have 4 samples in each pixel
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

    window = glfwCreateWindow(HEIGHT, WIDTH, WINDOW_NAME, NULL, NULL);
    if (!window)
    {
        std::cerr << "ERROR: could not open window with GLFW3" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Close the window as soon as the Escape key has been pressed
    glfwSetKeyCallback(window, quitCallback);
    // Makes the window context current
    glfwMakeContextCurrent(window);
    // Enable the viewport
    glViewport(0, 0, HEIGHT, WIDTH);

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    /*--------------------------------------------------------------------------------------------------*/

    GLint result = GL_FALSE;
	int infoLogLength;

    GLuint programId;
    programId = glCreateProgram();

    /*--------------------------------------------------------------------------------------------------*/

    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);

    // read vertex shader 
    std::string vertexShaderPath = "./resources/triangle.vert";
    std::string vertexShaderCode;
	std::ifstream vertexShaderStream(vertexShaderPath, std::ios::in);
	if(vertexShaderStream.is_open()){
		std::stringstream vertexShaderSstr;
		vertexShaderSstr << vertexShaderStream.rdbuf();
		vertexShaderCode = vertexShaderSstr.str();
		vertexShaderStream.close();
	}else{
		std::cout << "Impossible to open ! ";
		std::cout <<  "Are you in the right directory?" << std::endl;
		getchar();
	};
    char const *  vertexShaderSourcePointer = vertexShaderCode.c_str();
    
    // compile vertex shader
    glShaderSource(vertexShaderId, 1, &vertexShaderSourcePointer , NULL);
	glCompileShader(vertexShaderId);
    
    // Check Vertex Shader
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 ){
		std::vector<char> vertexShaderErrorMessage(infoLogLength+1);
		glGetShaderInfoLog(vertexShaderId, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
		printf("%s\n", &vertexShaderErrorMessage[0]);
	}

    // link vertex shader program
	glAttachShader(programId, vertexShaderId);

    /*--------------------------------------------------------------------------------------------------*/

    GLuint fragmentShaderId =  glCreateShader(GL_FRAGMENT_SHADER);

    // read fragmentshader 
    std::string fragmentShaderPath = "./resources/triangle.frag";
    std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream(fragmentShaderPath, std::ios::in);
	if(fragmentShaderStream.is_open()){
		std::stringstream fragmentShaderSstr;
		fragmentShaderSstr << fragmentShaderStream.rdbuf();
		fragmentShaderCode = fragmentShaderSstr.str();
		fragmentShaderStream.close();
	}else{
		std::cout << "Impossible to open ! ";
		std::cout <<  "Are you in the right directory?" << std::endl;
		getchar();
	};
    char const *  fragmentShaderSourcePointer = fragmentShaderCode.c_str();

	glShaderSource(fragmentShaderId, 1, &fragmentShaderSourcePointer , NULL);
	glCompileShader(fragmentShaderId);

	// Check Fragment Shader
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

	if ( infoLogLength > 0 ){
		std::vector<char> fragmentShaderErrorMessage(infoLogLength+1);
		glGetShaderInfoLog(fragmentShaderId, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
		printf("%s\n", &fragmentShaderErrorMessage[0]);
	}

    // link fragment shader program
	glAttachShader(programId, fragmentShaderId);


    /*--------------------------------------------------------------------------------------------------*/

	// Link the program
	printf("Linking program\n");
	glLinkProgram(programId);

	// Check the program
	glGetProgramiv(programId, GL_LINK_STATUS, &result);
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
	
	if ( infoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(infoLogLength+1);
		glGetProgramInfoLog(programId, infoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

    glDetachShader(programId, vertexShaderId);
	glDetachShader(programId, fragmentShaderId);
	
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

    /*--------------------------------------------------------------------------------------------------*/
    
    // we need three 3D points in order to make a triangle
    // An array of 3 vectors which represents 3 vertices
    static const GLfloat *g_vertex_buffer_data = triangle.vertexBufferData;

    
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
    glBufferData(GL_ARRAY_BUFFER, triangle.bufferDataSize * sizeof(float), g_vertex_buffer_data, GL_STATIC_DRAW);

    // you need to create a Vertex Array Object and set it as the current one
    // Do this once your window is created 
    // (= after the OpenGL Context creation) and before any other OpenGL call.
    GLuint VertexArrayID;

    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    /*--------------------------------------------------------------------------------------------------*/

    // Now we have a current OpenGL context, we can use OpenGL normally
    while (!glfwWindowShouldClose(window)){
        
        // Clear the screen. 
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        // tell OpenGL that you want to use your shader
        glUseProgram(programId);

        GLint vertesPositionAttr = glGetAttribLocation(programId, "vertexPosition_modelspace");

        glEnableVertexAttribArray(vertesPositionAttr);

        // 1st attribute buffer : vertices
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
            vertesPositionAttr, // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0           // array buffer offset
        );

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(vertesPositionAttr);

        // Draw triangle here ---------------------------------------------------------------------------

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    /*--------------------------------------------------------------------------------------------------*/

    // Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);

	glDeleteVertexArrays(1, &VertexArrayID);

	glDeleteProgram(programId);
    // ... here, the user closed the window
    glfwTerminate();
    return 0;
    /*--------------------------------------------------------------------------------------------------*/

}