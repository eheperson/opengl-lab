// tell the compiler that we will use OpenGL 3’s syntax.
#version 330 core

// “out” means that this is some output data
out vec3 color;
void main(){

  // set the color of each fragment to red. 
  // (Remember, there are 4 fragment in a pixel because we use 4x AntiAliasing)
  
  color = vec3(1,0,0); // vec3(Red, Green, Blue)
}