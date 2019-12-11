#include "joint.h"
#include "halfedge_datastructure.h"
Joint::Joint()
{
    mName = QString("Hello");
    mParentJoint = nullptr;
    mChildJointsVec.clear();
    mRelativePos = glm::vec3(0, 0, 0);
    mOrientation = glm::quat_cast(glm::mat4(1.f));
    mBindMatrix = glm::mat4(1.f);
    mSelected = false;
    mXAxisDegree = 0;
    mYAxisDegree = 0;
    mZAxisDegree = 0;
    mID = 0;
}

Joint::Joint(glm::vec3 iRelativePos)
{
    mName = QString("Hello");
    mParentJoint = nullptr;
    mChildJointsVec.clear();
    mRelativePos = iRelativePos;
    mOrientation = glm::quat_cast(glm::mat4(1.f));
    mBindMatrix = glm::mat4(1.f);
    mSelected = false;
    mXAxisDegree = 0;
    mYAxisDegree = 0;
    mZAxisDegree = 0;
    mID = 0;
}

glm::mat4 Joint::getLocalTransformation()
{
    // Local2Parent

    glm::mat4 localTransformation;
    glm::mat4 tempRotation = glm::mat4_cast(mOrientation);
    localTransformation = glm::translate(glm::mat4(1.0f), mRelativePos) * glm::mat4_cast(mOrientation);
    return localTransformation;
}

glm::mat4 Joint::getOverallTransformation()
{
    glm::mat4 overallTransformation = getLocalTransformation();
    // Local2Global
    if(this->mParentJoint != nullptr)
    {
        overallTransformation = this->mParentJoint->getOverallTransformation() * overallTransformation;
    }
    return overallTransformation;
}

void Joint::UpdateOrientation()
{
    // Euler Angles to Rotation Matrix. X - Y - Z.
    glm::mat4 rMat = glm::rotate(glm::mat4(1.f), float(glm::radians(mXAxisDegree)), glm::vec3(1, 0, 0)) *
                     glm::rotate(glm::mat4(1.f), float(glm::radians(mYAxisDegree)), glm::vec3(0, 1, 0)) *
                     glm::rotate(glm::mat4(1.f), float(glm::radians(mZAxisDegree)), glm::vec3(0, 0, 1));
    mOrientation = glm::quat_cast(rMat);
}

Skeleton::Skeleton(OpenGLContext* context)
    : Drawable(context)
{

}

bool Skeleton::SkeletonReady()
{
    if(mJointsList.size() == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

// Init Everything.
void Skeleton::create()
{
    OpenGLDS mDataArrays;
    if(SkeletonReady())
    {
        mDataArrays = OpenGLDS(*this);
    }

    count = mDataArrays.arrIdx.size();
    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, mDataArrays.arrIdx.size() * sizeof(GLuint), mDataArrays.arrIdx.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, mDataArrays.arrPos.size() * sizeof(glm::vec4), mDataArrays.arrPos.data(), GL_STATIC_DRAW);

    generateNor();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    mp_context->glBufferData(GL_ARRAY_BUFFER, mDataArrays.arrNor.size() * sizeof(glm::vec4), mDataArrays.arrNor.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, mDataArrays.arrCol.size() * sizeof(glm::vec4), mDataArrays.arrCol.data(), GL_STATIC_DRAW);

}

GLenum Skeleton::drawMode()
{
    return GL_LINES;
}

glm::mat4 Skeleton::GetModelMat()
{   /*
    if(SkeletonReady())
    {
        return mRoot->getLocalTransformation();
    }
    else
    {
        return glm::mat4(1.f);
    }
    */
    return glm::mat4(1.f);
}

void Skeleton::ClearSelected()
{
    for(unsigned int i = 0; i < mJointsList.size(); i++)
    {
        Joint* jPtr = mJointsList.at(i).get();
        jPtr->mSelected = false;
    }
}

std::vector<glm::mat4> Skeleton::getBindMatVec()
{
    std::vector<glm::mat4> tempVec;
    for(unsigned int i = 0; i < mJointsList.size(); i++)
    {
        Joint* jPtr = mJointsList.at(i).get();
        tempVec.push_back(jPtr->mBindMatrix);
    }
    return tempVec;
}

std::vector<glm::mat4> Skeleton::getJointToWorldMatVec()
{
    std::vector<glm::mat4> tempVec;
    for(unsigned int i = 0; i < mJointsList.size(); i++)
    {
        Joint* jPtr = mJointsList.at(i).get();
        tempVec.push_back(jPtr->getOverallTransformation());
    }
    return tempVec;
}

FrameWireSphere::FrameWireSphere()
{
    // Vertex positions
    unsigned int pNumInScircle = 10;
    float deg = glm::radians(360.f / pNumInScircle);
    glm::vec4 p1(0.f, 0.5f, 0.f, 1.f);
    glm::vec4 p2(0.f, 0.f, 0.5f, 1.f);

    for (int i = 0; i < pNumInScircle; i++)
    {
        glm::mat4 MVZ = glm::rotate(glm::mat4(), i * deg, glm::vec3(1.f, 0.f, 0.f));
        glm::mat4 MVX = glm::rotate(glm::mat4(), i * deg, glm::vec3(0.f, 0.f, 1.f));
        glm::mat4 MH = glm::rotate(glm::mat4(), i * deg, glm::vec3(0.f, 1.f, 0.f));
        verticalZVec.push_back(MVZ * p1);
        verticalXVec.push_back(MVX * p1);
        horizontalVec.push_back(MH * p2);
    }
}
