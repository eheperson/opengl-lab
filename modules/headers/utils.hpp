
static void quit_callback(GLFWwindow *window, int key, int scancode, int action, int _mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
};


GLFWwindow* init(const size_t width, const size_t height, const char* windowName){

    // Initialize GLEW
    #ifndef __APPLE__
        glewExperimental=true; // Needed in core profile
        if (glewInit() != GLEW_OK) {
            std::cerr <<  "Failed to initialize GLEW" << std::endl;
        };
    #endif

    GLFWwindow *window;

    if (!glfwInit()) {
        std::cerr << "ERROR: could not start GLFW3" << std::endl;
        return window;
    }

    // Minimum target is OpenGL 4.1
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing : we have 4 samples in each pixel
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

    window = glfwCreateWindow(height, width, windowName, NULL, NULL);
    if (!window){
        std::cerr << "ERROR: could not open window with GLFW3" << std::endl;
        glfwTerminate();
        return window;
    }
    
    // Close the window as soon as the Escape key has been pressed
    glfwSetKeyCallback(window, quit_callback);
    // Makes the window context current
    glfwMakeContextCurrent(window);

    return window;
}

void driverInfo(){
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;
}

std::string readShaderFile(std::string const shaderFilePath){
    std::string shaderCode;
	std::ifstream shaderStream(shaderFilePath, std::ios::in);
	if(shaderStream.is_open()){
		std::stringstream shaderSstr;
		shaderSstr << shaderStream.rdbuf();
		shaderCode = shaderSstr.str();
		shaderStream.close();
	}else{
		std::cout << "Impossible to open ! ";
		std::cout <<  "Are you in the right directory?" << std::endl;
		getchar();
	};
    return shaderCode;
}

uint compileShader(GLuint& programId, GLuint& shaderId, std::string& shaderCode, GLint& result, int& infoLogLength){
    char const *  shaderSourcePointer = shaderCode.c_str();
    // compile shader
    glShaderSource(shaderId, 1, &shaderSourcePointer , NULL);
	glCompileShader(shaderId);
    
    // Check Vertex Shader
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 ){
		std::vector<char> vertexShaderErrorMessage(infoLogLength+1);
		glGetShaderInfoLog(shaderId, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
		printf("%s\n", &vertexShaderErrorMessage[0]);
        return -1;
	}

    // link shader to program
	glAttachShader(programId, shaderId);

    return 1;
}

uint linkProgram(GLuint& programId, GLint& result, int& infoLogLength){
    glLinkProgram(programId);

	// Check the program
	glGetProgramiv(programId, GL_LINK_STATUS, &result);
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
	
	if ( infoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(infoLogLength+1);
		glGetProgramInfoLog(programId, infoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
        return -1;
	}

    return 1;
}

void cleanShader(GLuint& programId, GLuint& shaderId){
    glDetachShader(programId, shaderId);
	glDeleteShader(shaderId);
}


GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){
	GLint Result = GL_FALSE;
	int InfoLogLength;

	GLuint ProgramID = glCreateProgram();

	/*--------------------------------------------------------------------------*/

	// Create the shaders
	// Notice that just as buffers, shaders are not directly accessible : we just have an ID
	// The actual implementation is hidden inside the driver.
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();

	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// linking vertex shader to program
	glAttachShader(ProgramID, VertexShaderID);
	/*--------------------------------------------------------------------------*/

	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();

	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	glAttachShader(ProgramID, FragmentShaderID);

	/*--------------------------------------------------------------------------*/

	// Link the program
	printf("Linking program\n");
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	/*--------------------------------------------------------------------------*/
	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}
