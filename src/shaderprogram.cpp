#include "shaderprogram.h"
#include <iostream>
#include <QFile>
#include <QStringBuilder>


ShaderProgram::ShaderProgram(OpenGLContext *context)
    : vertShader(), fragShader(), prog(),
      attrPos(-1), attrNor(-1), attrCol(-1), attrInfluJointsID(-1),
      attrInWeights(-1), attrInfluJointsIDArray(-1), attrInWeightsArray(-1),
      hasInfluJointsID(false), hasInWeights(false),
      unifModel(-1), unifModelInvTr(-1), unifViewProj(-1), unifCamPos(-1), unifJointNum(-1),
      jointNum(0), context(context)
{}

void ShaderProgram::create(const char *vertfile, const char *fragfile)
{
    // Allocate space on our GPU for a vertex shader and a fragment shader and a shader program to manage the two
    vertShader = context->glCreateShader(GL_VERTEX_SHADER);
    fragShader = context->glCreateShader(GL_FRAGMENT_SHADER);
    prog = context->glCreateProgram();
    // Get the body of text stored in our two .glsl files
    QString qVertSource = qTextFileRead(vertfile);
    QString qFragSource = qTextFileRead(fragfile);

    char* vertSource = new char[qVertSource.size()+1];
    strcpy(vertSource, qVertSource.toStdString().c_str());
    char* fragSource = new char[qFragSource.size()+1];
    strcpy(fragSource, qFragSource.toStdString().c_str());


    // Send the shader text to OpenGL and store it in the shaders specified by the handles vertShader and fragShader
    context->glShaderSource(vertShader, 1, &vertSource, 0);
    context->glShaderSource(fragShader, 1, &fragSource, 0);
    // Tell OpenGL to compile the shader text stored above
    context->glCompileShader(vertShader);
    context->glCompileShader(fragShader);
    // Check if everything compiled OK
    GLint compiled;
    context->glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        printShaderInfoLog(vertShader);
    }
    context->glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        printShaderInfoLog(fragShader);
    }

    // Tell prog that it manages these particular vertex and fragment shaders
    context->glAttachShader(prog, vertShader);
    context->glAttachShader(prog, fragShader);
    context->glLinkProgram(prog);

    // Check for linking success
    GLint linked;
    context->glGetProgramiv(prog, GL_LINK_STATUS, &linked);
    if (!linked) {
        printLinkInfoLog(prog);
    }

    // Get the handles to the variables stored in our shaders
    // See shaderprogram.h for more information about these variables
    // Get the position for VBOs destination.
    attrPos = context->glGetAttribLocation(prog, "vs_Pos");
    attrNor = context->glGetAttribLocation(prog, "vs_Nor");
    attrCol = context->glGetAttribLocation(prog, "vs_Col");
    attrInfluJointsID = context->glGetAttribLocation(prog, "in_jointIDs");
    attrInWeights = context->glGetAttribLocation(prog, "in_weights");
    attrInfluJointsIDArray = context->glGetAttribLocation(prog, "in_jointIDsArray");
    attrInWeightsArray = context->glGetAttribLocation(prog, "in_weightsArray");


    unifModel      = context->glGetUniformLocation(prog, "u_Model");
    unifModelInvTr = context->glGetUniformLocation(prog, "u_ModelInvTr");
    unifViewProj   = context->glGetUniformLocation(prog, "u_ViewProj");
    unifCamPos      = context->glGetUniformLocation(prog, "u_CamPos");
    unifBindMatArray = context->glGetUniformLocation(prog, "u_BindMatArray");
    unifJointToWorldMatArray = context->glGetUniformLocation(prog, "u_JointToWorldMatArray");
    unifJointNum = context->glGetUniformLocation(prog, "u_JointNum");

    std::cout << "Vertex Shader:" << vertfile << std::endl;
    std::cout << "Shader Source Code:" << std::endl << vertSource << std::endl;
    std::cout << "Fragment Shader:" << fragfile << std::endl;
    std::cout << "Vertex Shader In Variable ID: attrPos:" << attrPos << std::endl;
    std::cout << "Vertex Shader In Variable ID: attrNor:" << attrNor << std::endl;
    std::cout << "Vertex Shader In Variable ID: attrCol:" << attrCol << std::endl;
    std::cout << "Vertex Shader In Variable ID: attrInfluJointsID:" << attrInfluJointsID << std::endl;
    std::cout << "Vertex Shader In Variable ID: attrInWeights:" << attrInWeights << std::endl;
    std::cout << "Vertex Shader In Variable ID: attrInfluJointsIDArray:" << attrInfluJointsIDArray << std::endl;
    std::cout << "Vertex Shader In Variable ID: attrInWeightsArray:" << attrInWeightsArray << std::endl;
    std::cout << "Vertex Shader Uniform Variable ID: unifModel:" << unifModel << std::endl;
    std::cout << "Vertex Shader Uniform Variable ID: unifModelInvTr:" << unifModelInvTr << std::endl;
    std::cout << "Vertex Shader Uniform Variable ID: unifViewProj:" << unifViewProj << std::endl;
    std::cout << "Vertex Shader Uniform Variable ID: unifCamPos:" << unifCamPos << std::endl;
    std::cout << "Vertex Shader Uniform Variable ID: unifBindMatArray:" << unifBindMatArray << std::endl;
    std::cout << "Vertex Shader Uniform Variable ID: unifJointToWorldMatArray:" << unifJointToWorldMatArray << std::endl;
    std::cout << "Vertex Shader Uniform Variable ID: unifJointNum:" << unifJointNum << std::endl;
    std::cout << std::endl << std::endl;
}

void ShaderProgram::useMe()
{
    context->glUseProgram(prog);
}

void ShaderProgram::setModelMatrix(const glm::mat4 &model)
{
    useMe();

    if (unifModel != -1) {
        // Pass a 4x4 matrix into a uniform variable in our shader
                        // Handle to the matrix variable on the GPU
        context->glUniformMatrix4fv(unifModel,
                        // How many matrices to pass
                           1,
                        // Transpose the matrix? OpenGL uses column-major, so no.
                           GL_FALSE,
                        // Pointer to the first element of the matrix
                           &model[0][0]);
    }

    if (unifModelInvTr != -1) {
        glm::mat4 modelinvtr = glm::inverse(glm::transpose(model));
        // Pass a 4x4 matrix into a uniform variable in our shader
                        // Handle to the matrix variable on the GPU
        context->glUniformMatrix4fv(unifModelInvTr,
                        // How many matrices to pass
                           1,
                        // Transpose the matrix? OpenGL uses column-major, so no.
                           GL_FALSE,
                        // Pointer to the first element of the matrix
                           &modelinvtr[0][0]);
    }
}

void ShaderProgram::setViewProjMatrix(const glm::mat4 &vp)
{
    // Tell OpenGL to use this shader program for subsequent function calls
    useMe();

    if(unifViewProj != -1) {
    // Pass a 4x4 matrix into a uniform variable in our shader
                    // Handle to the matrix variable on the GPU
    context->glUniformMatrix4fv(unifViewProj,
                    // How many matrices to pass
                       1,
                    // Transpose the matrix? OpenGL uses column-major, so no.
                       GL_FALSE,
                    // Pointer to the first element of the matrix
                       &vp[0][0]);
    }
}

void ShaderProgram::setCamPos(glm::vec3 pos)
{
    useMe();

    if(unifCamPos != -1)
    {
        context->glUniform3fv(unifCamPos, 1, &pos[0]);
    }
}

void ShaderProgram::setBindMatArray(std::vector<glm::mat4> &bindMatVec)
{
    useMe();
    glm::mat4& temp = bindMatVec.at(0);

    if(unifBindMatArray != -1)
    {
        context->glUniformMatrix4fv(unifBindMatArray, bindMatVec.size(), false, &temp[0][0]);
    }
}

void ShaderProgram::setJointToWorldMatArray(std::vector<glm::mat4> &jointToWorldMatVec)
{
    useMe();
    glm::mat4& temp = jointToWorldMatVec.at(0);

    if(unifJointToWorldMatArray != -1)
    {
        context->glUniformMatrix4fv(unifJointToWorldMatArray,
                                    jointToWorldMatVec.size(),
                                    false, &temp[0][0]);
    }
}

void ShaderProgram::setJointNum(int iNum)
{
    useMe();
    jointNum = iNum;
    if(unifJointNum != -1)
    {
        //int temp = 2;
        context->glUniform1iv(unifJointNum, 1, &iNum);
    }
}

//This function, as its name implies, uses the passed in GL widget
void ShaderProgram::draw(Drawable &d)
{
        useMe();

    // Each of the following blocks checks that:
    //   * This shader has this attribute, and
    //   * This Drawable has a vertex buffer for this attribute.
    // If so, it binds the appropriate buffers to each attribute.

        // Remember, by calling bindPos(), we call
        // glBindBuffer on the Drawable's VBO for vertex position,
        // meaning that glVertexAttribPointer associates vs_Pos
        // (referred to by attrPos) with that VBO
    if (attrPos != -1 && d.bindPos()) {
        context->glEnableVertexAttribArray(attrPos);
        context->glVertexAttribPointer(attrPos, 4, GL_FLOAT, false, 0, nullptr);
    }

    if (attrNor != -1 && d.bindNor()) {
        context->glEnableVertexAttribArray(attrNor);
        context->glVertexAttribPointer(attrNor, 4, GL_FLOAT, false, 0, nullptr);
    }

    if (attrCol != -1 && d.bindCol()) {
        context->glEnableVertexAttribArray(attrCol);
        context->glVertexAttribPointer(attrCol, 4, GL_FLOAT, false, 0, nullptr);
    }

    if (attrInfluJointsID != -1 && d.bindInfluJointID()) {
        context->glEnableVertexAttribArray(attrInfluJointsID);
        context->glVertexAttribIPointer(attrInfluJointsID, 2, GL_INT, 0, nullptr);
    }

    if (attrInWeights != -1 && d.bindWeights()) {
        context->glEnableVertexAttribArray(attrInWeights);
        context->glVertexAttribPointer(attrInWeights, 2, GL_FLOAT, false, 0, nullptr);
    }

    if (attrInfluJointsIDArray != -1 && d.bindInfluJointIDArray())
    {
        for(unsigned int i = 0; i < jointNum; i++)
        {
            context->glEnableVertexAttribArray(attrInfluJointsIDArray + i);
            context->glVertexAttribIPointer(attrInfluJointsIDArray + i,
                                            1,
                                            GL_INT,
                                            sizeof(int) * jointNum,
                                            reinterpret_cast<void*>(i * 1 * sizeof(int)));
        }
    }

    if(attrInWeightsArray != -1 && d.bindWeightsArray())
    {
        for(unsigned int i = 0; i < jointNum; i++)
        {
            context->glEnableVertexAttribArray(attrInWeightsArray + i);
            context->glVertexAttribPointer(attrInWeightsArray + i,
                                           1,
                                           GL_FLOAT,
                                           false,
                                           sizeof(float) * jointNum,
                                           reinterpret_cast<void*>(i * 1 * sizeof(float)));
        }
    }

    // Bind the index buffer and then draw shapes from it.
    // This invokes the shader program, which accesses the vertex buffers.
    d.bindIdx();
    context->glDrawElements(d.drawMode(), d.elemCount(), GL_UNSIGNED_INT, 0);

    if (attrPos != -1) context->glDisableVertexAttribArray(attrPos);
    if (attrNor != -1) context->glDisableVertexAttribArray(attrNor);
    if (attrCol != -1) context->glDisableVertexAttribArray(attrCol);
    if (attrInfluJointsID != -1) context->glDisableVertexAttribArray(attrInfluJointsID);
    if (attrInWeights != -1) context->glDisableVertexAttribArray(attrInWeights);
    if (attrInfluJointsIDArray != -1)
    {
        for(unsigned int i = 0; i < 2; i++)
        {
            context->glDisableVertexAttribArray(attrInfluJointsIDArray + i);
        }
    }
    if (attrInWeightsArray != -1)
    {
        for(unsigned int i = 0; i < 2; i++)
        {
            context->glDisableVertexAttribArray(attrInWeightsArray + i);
        }
    }

    context->printGLErrorLog();
}

char* ShaderProgram::textFileRead(const char* fileName) {
    char* text;

    if (fileName != NULL) {
        FILE *file = fopen(fileName, "rt");

        if (file != NULL) {
            fseek(file, 0, SEEK_END);
            int count = ftell(file);
            rewind(file);

            if (count > 0) {
                text = (char*)malloc(sizeof(char) * (count + 1));
                count = fread(text, sizeof(char), count, file);
                text[count] = '\0';	//cap off the string with a terminal symbol, fixed by Cory
            }
            fclose(file);
        }
    }
    return text;
}

QString ShaderProgram::qTextFileRead(const char *fileName)
{
    QString text;
    QFile file(fileName);
    if(file.open(QFile::ReadOnly))
    {
        QTextStream in(&file);
        text = in.readAll();
        text.append('\0');
    }
    return text;
}

void ShaderProgram::printShaderInfoLog(int shader)
{
    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar *infoLog;

    context->glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

    // should additionally check for OpenGL errors here

    if (infoLogLen > 0)
    {
        infoLog = new GLchar[infoLogLen];
        // error check for fail to allocate memory omitted
        context->glGetShaderInfoLog(shader,infoLogLen, &charsWritten, infoLog);
        qDebug() << "ShaderInfoLog:" << endl << infoLog << endl;
        delete [] infoLog;
    }

    // should additionally check for OpenGL errors here
}

void ShaderProgram::printLinkInfoLog(int prog)
{
    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar *infoLog;

    context->glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &infoLogLen);

    // should additionally check for OpenGL errors here

    if (infoLogLen > 0) {
        infoLog = new GLchar[infoLogLen];
        // error check for fail to allocate memory omitted
        context->glGetProgramInfoLog(prog, infoLogLen, &charsWritten, infoLog);
        qDebug() << "LinkInfoLog:" << endl << infoLog << endl;
        delete [] infoLog;
    }
}
