

glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;

glm::mat4 getViewMatrix(){
    /* returns the computed View matrix. */
	return viewMatrix;
}
glm::mat4 getProjectionMatrix(){
    /* returns the computed Projection matrix.*/
	return projectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 0, 5 ); 
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;



void computeMatricesFromInputs(GLFWwindow *window, int width, int height){
    /*  reads the keyboard and mouse and computes the Projection and View matrices. This is where all the magic happens. */

    /*--------------------------------------------------------------------------------------------------*/

	/*
		timedelta : You don’t want to move from 1 unit each frame for a simple reason :

			- If you have a fast computer, 
				and you run at 60 fps, you’d move of 60*speed units in 1 second

			- If you have a slow computer, 
				and you run at 20 fps, you’d move of 20*speed units in 1 second
		
		Since having a better computer is not an excuse for going faster, 
		you have to scale the distance by the “time since the last frame”, or “deltaTime”.

			- If you have a fast computer, and you run at 60 fps, 
			you’d move of 1/60 * speed units in 1 frame, so 1*speed in 1 second.

			- If you have a slow computer, and you run at 20 fps, 
			you’d move of 1/20 * speed units in 1 second, so 1*speed in 1 second.
	*/
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();
	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

    /*--------------------------------------------------------------------------------------------------*/

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, width/2, height/2);

    /*--------------------------------------------------------------------------------------------------*/

	// Compute new orientation
	horizontalAngle += deltaTime * mouseSpeed * float(width/2 - xpos );
	verticalAngle   += deltaTime * mouseSpeed * float(height/2 - ypos );
	/*
		"width/2 - xpos" : how far is the mouse from the center of the window ? 
							The bigger this value, the more we want to turn.

		float(…) converts it to a floating-point number so that the multiplication goes well.

		"mouseSpeed" : just there to speed up or slow down the rotations. 
						Fine-tune this at will, or let the user choose it.

		"+=" : If you didn’t move the mouse, "width/2-xpos" will be 0, 
				and "horizontalAngle+=0" doesn’t change horizontalAngle. 
				
				If you had a “=” instead, you would be forced back to your original orientation each frame, 
				which isn’t good.
	*/

    /*--------------------------------------------------------------------------------------------------*/

	// compute a vector that represents, in World Space, the direction in which we’re looking
	// Direction : Spherical coordinates to Cartesian coordinates conversion
	
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	); 

    /*--------------------------------------------------------------------------------------------------*/

	/*
		in our case, the only constant is that the vector 
		goes to the right of the camera is always horizontal. 
		
		You can check this by putting your arm horizontal, 
		and looking up, down, in any direction. 
		
		So let’s define the “right” vector : 
			- its Y coordinate is 0 since it’s horizontal, 
			and its X and Z coordinates are just like in the figure above, 
			but with the angles rotated by 90°, or Pi/2 radians.
	*/
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	
    /*--------------------------------------------------------------------------------------------------*/

	/* 
		We have a “right” vector and a “direction”, or “front” vector. 
		The “up” vector is a vector that is perpendicular to these two. 
		A useful mathematical tool makes this very easy : the cross product.
	*/
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

    /*--------------------------------------------------------------------------------------------------*/

	// Move forward
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}

    /*--------------------------------------------------------------------------------------------------*/

	/*
		For fun, we can also bind the wheel of the mouse to the Field Of View, 
		so that we can have a cheap zoom : 
	*/
	float FoV = initialFoV;
	// float FoV = initialFoV - 5 * glfwGetMouseWheel(); 

	// Now GLFW 3 requires setting up a callback for this. 
	// It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

    /*--------------------------------------------------------------------------------------------------*/

	/* 
		computing matricies 

		Computing the matrices is now straightforward. 
		We use the exact same functions than before, but with our new parameters.
	*/
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	projectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	viewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

    /*--------------------------------------------------------------------------------------------------*/

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}