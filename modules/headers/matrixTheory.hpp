#include<iostream>


/*

    -------------------------------------------------------------------------------------------
    Remember Futurama : 

        The engines don’t move the ship at all. 
        The ship stays where it is and the engines move the universe around it.
    -------------------------------------------------------------------------------------------

    Homogeneous Coordinates: (x, y, z, w)

        - If w == 1, then the vector (x,y,z,1) is a position in space.
        - If w == 0, then the vector (x,y,z,0) is a direction.

    ! moving a direction does not make sense : try and see (i tried)

    Transformation Matrics : 

        Matrix x Vertex (in this order !!) = TransformedVertex


    - what is the meaning of “scaling a direction” ?
         Well, often, not much, so you usually do not do such a thing, but in some (rare) cases it can be handy.

    - the identity matrix is only a special case of scaling matrices, with (X,Y,Z) = (1,1,1). 
        It is also a special case of translation matrix with (X,Y,Z)=(0,0,0), by the way

    - The Model, View and Projection matrices are a handy tool to separate transformations cleanly.

    --------------------------------------------------------------------------------------------------------------------------------------

    THE MODEL VIEW AND PROJECTION MATRICES

        - This model, just as our beloved red triangle, is defined by a set of vertices. 
        - The X,Y,Z coordinates of these vertices are defined relative to the object's center : 
            that is, if a vertex is at (0,0,0), it is at the center of the object.

        - To move this model, translation*rotation*scale, and done. 
          You apply this matrix to all your vertices at each frame (in GLSL, not in C++!) and everything moves.
          Something that does not move will be at the center of the world.

        - Your vertices are now in World Space : We went from Model Space to World Space.
            
            | Model Coordinates | ---> (Model Matrix) ---> | World Coordinates |

            Being in Model Space : all vertices defined relatively to the center of the model.
            Being in World Space : all vertices defined relatively to the center of the world.
        

        - It you want to view a moutain from another angle, you can either move the camera... or move the mountain. 
            While not practical in real life, this is really simple and handy in Computer Graphics.

        - So initially your camera is at the origin of the World Space. 
            In order to move the world, you simply introduce another matrix. 
            Let's say you want to move your camera of 3 units to the right (+X). 
            This is equivalent to moving your whole world (meshes included) 3 units to the LEFT ! (-X).

                ( glm::mat4 ViewMatrix = glm::translate(glm::mat4(), glm::vec3(-3.0f, 0.0f ,0.0f)); )


        - Your vertices are now in Camera Space : We went from World Space to Camera Space.

            | World Coordinates | ---> (View Matrix) ---> | Camera Coordinates |

            Being in Camera Space : all vertices defined relatively to the camera


        - enjoy GLM's great glm::lookAt function : 

                glm::mat4 CameraMatrix = glm::lookAt(
                    cameraPosition, // the position of your camera, in world space
                    cameraTarget,   // where you want to look at, in world space
                    upVector        // probably glm::vec3(0,1,0), 
                                    // but (0,-1,0) would make you looking upside-down, which can be great too
                );

            | Model Coords | --> (Model Matrix) --> | World Coords | --> (View Matrix) --> | Camera Coords |


        - We are now in Camera Space. 
            This means that after all theses transformations, 
            a vertex that happens to have x==0 and y==0 should be rendered at the center of the screen. 
            
        - But we cannot use only the x and y coordinates 
            to determine where an object should be put on the screen : 
            
            its distance to the camera (z) counts, too ! 
            For two vertices with similar x and y coordinates, 
            the vertex with the biggest z coordinate will 
            be more on the center of the screen than the other.

        - And luckily for us, a 4x4 matrix can represent this projection :

            // Generates a really hard-to-read matrix, but a normal, standard 4x4 matrix nonetheless
            glm::mat4 projectionMatrix = glm::perspective(
                glm::radians(FoV),  // The vertical Field of View, in radians: 
                                    //the amount of "zoom". Think "camera lens". 
                                    // Usually between 90° (extra wide) and 30° (quite zoomed in)
                4.0f / 3.0f,        // Aspect Ratio. Depends on the size of your window. 
                                    // Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
                0.1f,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
                100.0f             // Far clipping plane. Keep as little as possible.
            );

        - We went from Camera Space to Homogeneous Space.

            Being in Homogeneous Space : all vertices defined in a small cube. Everything inside the cube is onscreen

            | Model Coords | --> (Model Matrix) --> | World Coords | --> (View Matrix) --> | Camera Coords | --> (Projection Matrix) --> | Homogeneour Coords |

*/

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
// using namespace glm;

glm::mat4 identityMatrix = glm::mat4(1.0f);

/* - - - - - Transformation Matrix - - - - - - - - - - - - - - - - - - - Begin - - - - - - - - -*/

glm::mat4 matrix_1; // fill myMatrix somehow
glm::vec4 vector_1; // fill myVector somehow
glm::vec4 transformedVector = matrix_1 * vector_1; // Again, in this order ! this is important.
/* GLSL
    mat4 myMatrix;
    vec4 myVector;
    vec4 transformedVector = myMatrix * myVector;
*/

/* - - - - - Transformation Matrix - - - - - - - - - - - - - - - - - - - End - - - - - - - - -*/


/* - - - - - Translation - - - - - - - - - - - - - - - - - - - Begin - - - - - - - - - - - - - */

// translate the vector (10,10,10,1) of 10 units in the X direction
glm::mat4 translationMatrix = glm::translate(glm::mat4(), glm::vec3(10.0f, 0.0f, 0.0f)); // producing translation matrix
glm::vec4 vector_2(10.0f, 10.0f, 10.0f, 0.0f);
glm::vec4 translatedVector = translationMatrix * vector_2;

/* - - - - - Translation - - - - - - - - - - - - - - - - - - - End - - - - - - - - - - - - - - */


/* - - - - - Scaling - - - - - - - - - - - - - - - - - - - - - Begin - - - - - - - - - - - - - */
// scale a vector (position or direction, it doesn’t matter) by 2.0 in all directions
glm::mat4 scaleMatrix = glm::scale(2.0f, 2.0f ,2.0f); // producing scaling matrix
glm::vec4 vector_3(10.0f, 10.0f, 10.0f, 0.0f);
glm::vec4 scaledVector = scaleMatrix * vector_3;

/* - - - - - Scaling - - - - - - - - - - - - - - - - - - - - - End - - - - - - - - - - - - - - */


/* - - - - - Rotation - - - - - - - - - - - - - - - - - - - - - Begin - - - - - - - - - - - - - */

// Use #include <glm/gtc/matrix_transform.hpp> and #include <glm/gtx/transform.hpp>
glm::vec3 rotationAxis( 0.0f, 0.0f, 0.0f);
float rotationAngleInDegrees = 90.0f;
glm::mat4 rotationMatrix = glm::rotate(rotationAngleInDegrees, rotationAxis );

/* - - - - - Rotation - - - - - - - - - - - - - - - - - - - - - End - - - - - - - - - - - - - - */


/* - - - - - Cumulating Transformations - - - - - - - - - - - - Begin - - - - - - - - - - - - - */

// TransformedVector = TranslationMatrix * RotationMatrix * ScaleMatrix * OriginalVector;
glm::vec4 originalVector;
glm::mat4 myModelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
glm::vec4 transformedVector = myModelMatrix * originalVector;

/* GLSL
    mat4 transform = mat2 * mat1;
    vec4 out_vec = transform * in_vec;
*/

/* - - - - - Cumulating Transformations - - - - - - - - - - - - End - - - - - - - - - - - - - - */


/* - - - - - Putting It All Together - - - - - - - - - - - - Begin - - - - - - - - - - - - - - */

// Cumulating transformations : the ModelViewProjection matrix
glm::vec4 orgVect;
glm::mat4 modelMat;
glm::mat4 viewMat;
glm::mat4 projectionMat;
glm::mat4 MVPmatrix = projectionMat * viewMat * modelMat; // Remember : inverted !
glm::vec4 transformedVect = MVPmatrix * orgVect;

// GLSL : apply it
// transformed_vertex = MVP * in_vertex;



/* First Step */
#include <glm/gtc/matrix_transform.hpp>
const int width = 1024;
const int height = 720;


/* Second Step */
// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 100.0f);

// Or, for an ortho camera :
//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

// Camera matrix
glm::mat4 view = glm::lookAt(
    glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
    glm::vec3(0,0,0), // and looks at the origin
    glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
  
// Model matrix : an identity matrix (model will be at the origin)
glm::mat4 model = glm::mat4(1.0f);
// Our ModelViewProjection : multiplication of our 3 matrices
glm::mat4 mvp = projection * view * model; // Remember, matrix multiplication is the other way around

/* Third Step */
/*
    // Get a handle for our "MVP" uniform
    // Only during the initialisation
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    
    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
*/


/* Fourth Step */
/*
    // Input vertex data, different for all executions of this shader.
    layout(location = 0) in vec3 vertexPosition_modelspace;
    
    // Values that stay constant for the whole mesh.
    uniform mat4 MVP;
    
    void main(){
    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
    }
*/

/* - - - - - Putting It All Together - - - - - - - - - - - - End - - - - - - - - - - - - - - - */