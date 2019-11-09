#version 150
// ^ Change this to version 130 if you have compatibility issues

//This is a vertex shader. While it is called a "shader" due to outdated conventions, this file
//is used to apply matrix transformations to the arrays of vertex data passed to it.
//Since this code is run on your GPU, each vertex is transformed simultaneously.
//If it were run on your CPU, each vertex would have to be processed in a FOR loop, one at a time.
//This simultaneous transformation allows your program to run much faster, especially when rendering
//geometry with millions of vertices.

uniform mat4 u_Model;       // The matrix that defines the transformation of the
                            // object we're rendering. In this assignment,
                            // this will be the result of traversing your scene graph.

uniform mat4 u_ModelInvTr;  // The inverse transpose of the model matrix.
                            // This allows us to transform the object's normals properly
                            // if the object has been non-uniformly scaled.

uniform mat4 u_ViewProj;    // The matrix that defines the camera's transformation.
                            // We've written a static matrix for you to use for HW2,
                            // but in HW3 you'll have to generate one yourself

uniform mat4 u_BindMatArray[100];

uniform mat4 u_JointToWorldMatArray[100];

uniform int u_JointNum;

//in int in_InfluJointsID[2]; // A joint's ID should correspond to the index at which its bind and
                              // transformation matrices are stored in your arrays.

//in float in_weights[2];    // For example, if a vertex had joint IDs [0, 4] and weights [0.45, 0.55],
                             // then joint 0 would have a weight of 0.45 and joint 4 would have a weight of 0.55 on this vertex.
                             // We recommend using a regular vec or an array of floats to store this data.
in ivec2 in_jointIDs;
in vec2 in_weights;


in int in_jointIDsArray[7];
in float in_weightsArray[7];


in vec4 vs_Pos;             // The array of vertex positions passed to the shader

in vec4 vs_Nor;             // The array of vertex normals passed to the shader

in vec4 vs_Col;             // The array of vertex colors passed to the shader.

out vec3 fs_Pos;
out vec4 fs_Nor;            // The array of normals that has been transformed by u_ModelInvTr. This is implicitly passed to the fragment shader.
out vec4 fs_Col;            // The color of each vertex. This is implicitly passed to the fragment shader.

void main()
{
    // Pass the vertex colors to the fragment shader for interpolation
    // Note the Default Int Calculation
    /*fs_Col = vec4(1.0 / (in_jointIDs.y + 1.0),
                  1.0 / (in_jointIDs.y + 1.0),
                  1.0 / (in_jointIDs.y + 1.0), 0);*/
    /*fs_Col = vec4(1.0 / (in_jointIDsArray[0] + 1.0),
                  1.0 / (in_jointIDsArray[0] + 1.0),
                  1.0 / (in_jointIDsArray[0] + 1.0),
                  0);*/
    float sumWeight = 0;
    for(int i = 0; i < 7; i++)
    {
        sumWeight += in_weightsArray[i];
    }
    fs_Col = vec4(sumWeight, sumWeight, sumWeight, 0);
    //fs_Col = vec4(1.0 / u_JointNum, 1.0 / u_JointNum, 1.0 / u_JointNum, 0);

    mat3 invTranspose = mat3(u_ModelInvTr);
    fs_Nor = vec4(invTranspose * vec3(vs_Nor), 0);          // Pass the vertex normals to the fragment shader for interpolation.
                                                            // Transform the geometry's normals by the inverse transpose of the
                                                            // model matrix. This is necessary to ensure the normals remain
                                                            // perpendicular to the surface after the surface is transformed by
                                                            // the model matrix.
    vec4 posSum = vec4(0, 0, 0, 0);
    for(int i = 0; i < 7; i++)
    {
        mat4 bindMat = u_BindMatArray[in_jointIDsArray[i]];
        mat4 overAllT = u_JointToWorldMatArray[in_jointIDsArray[i]];
        posSum += overAllT * bindMat * in_weightsArray[i] * u_Model * vs_Pos;
    }

    fs_Pos = posSum.xyz;
    gl_Position = u_ViewProj * posSum;
    // Temporarily store the transformed vertex positions for use below


    //gl_Position = u_ViewProj * modelposition;// gl_Position is a built-in variable of OpenGL which is
                                             // used to render the final positions of the geometry's vertices
}
