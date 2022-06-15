// tell the compiler that we will use OpenGL 3’s syntax.
#version 410 core

// declares the input data 
layout(location = 0) in vec3 vertexPosition_modelspace;

/*
    “vec3” is a vector of 3 components in GLSL.
    if we use 3 components in C++, we use 3 components in GLSL too.
    
     - - -

    “layout(location = 0)” refers to the buffer we use to feed the vertexPosition_modelspace attribute. 
    Each vertex can have numerous attributes : 
        
        - A position
        - one or several colours
        - one or several texture coordinates
        - lots of other things. 
    
    The value “0” is not important, it could be 12 
    (but no more than glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &v) ), 
    the important thing is that it’s the same number on both sides.

     - - -

    “vertexPosition_modelspace” could have any other name. 
    It will contain the position of the vertex for each run of the vertex shader.
    
     - - -
     
    “in” means that this is some input data
*/

void main(){
    // Our main function will merely set the vertex’ position to whatever was in the buffer.

    // gl_Position is one of the few built-in variables : you *have *to assign some value to it
    // Everything else is optional;

    // x, y, z : space coordinates
    gl_Position.xyz = vertexPosition_modelspace;

    // w :  homogeneous vertex coordinate.
    gl_Position.w = 10.0;
}