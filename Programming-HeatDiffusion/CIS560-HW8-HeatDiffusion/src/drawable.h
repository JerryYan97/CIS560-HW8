#pragma once

#include <openglcontext.h>
#include <la.h>

//This defines a class which can be rendered by our shader program.
//Make any geometry a subclass of ShaderProgram::Drawable in order to render it with the ShaderProgram class.
class Drawable
{
protected:
    // Position for VBOs
    int count;     // The number of indices stored in bufIdx.
    GLuint bufIdx; // A Vertex Buffer Object that we will use to store triangle indices (GLuints)
    GLuint bufPos; // A Vertex Buffer Object that we will use to store mesh vertices (vec4s)
    GLuint bufNor; // A Vertex Buffer Object that we will use to store mesh normals (vec4s)
    GLuint bufCol; // Can be used to pass per-vertex color information to the shader, but is currently unused.
                   // Instead, we use a uniform vec4 in the shader to set an overall color for the geometry
    GLuint bufInfluJointsID;
    GLuint bufWeights;

    GLuint bufInfluJointsIDArray;
    GLuint bufWeightsArray;

    bool idxBound; // Set to TRUE by generateIdx(), returned by bindIdx().
    bool posBound;
    bool norBound;
    bool colBound;
    bool influJointsIDBound;
    bool weightsBound;
    bool influJointsIDArrayBound;
    bool weightsArrayBound;

    OpenGLContext* mp_context; // Since Qt's OpenGL support is done through classes like QOpenGLFunctions_3_2_Core,
                          // we need to pass our OpenGL context to the Drawable in order to call GL functions
                          // from within this class.


public:
    Drawable(OpenGLContext* context);
    virtual ~Drawable();

    virtual void create() = 0; // To be implemented by subclasses. Populates the VBOs of the Drawable.
    void destroy(); // Frees the VBOs of the Drawable.

    // Getter functions for various GL data
    virtual GLenum drawMode();
    int elemCount();

    // Call these functions when you want to call glGenBuffers on the buffers stored in the Drawable
    // These will properly set the values of idxBound etc. which need to be checked in ShaderProgram::draw()
    void generateIdx();
    void generatePos();
    void generateNor();
    void generateCol();
    void generateInfluJointID();
    void generateWeights();
    void generateInfluJointIDArray();
    void generateWeightsArray();

    // Used to call glBindBuffer and determine whether there is
    // a VBO representing this chunk of data in this context.
    bool bindIdx();
    bool bindPos();
    bool bindNor();
    bool bindCol();
    bool bindInfluJointID();
    bool bindWeights();
    bool bindInfluJointIDArray();
    bool bindWeightsArray();
};

