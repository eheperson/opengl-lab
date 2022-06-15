#include "headers/common.hpp"

int main(void){

    GLFWwindow *window;

    window = init(WIDTH, HEIGHT, WINDOW_NAME);
    driverInfo();
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Now we have a current OpenGL context, we can use OpenGL normally
    while (!glfwWindowShouldClose(window))
    {
        // Render
        glClear(GL_COLOR_BUFFER_BIT);
        // Swap front and back buffers
        glfwSwapBuffers(window);
        // Poll for and process events
        glfwPollEvents();
    }

    // ... here, the user closed the window
    glfwTerminate();
    return 0;
}