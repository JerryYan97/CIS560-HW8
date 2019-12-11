#include "primitivedisplay.h"

VertexDisplay::VertexDisplay(OpenGLContext *context)
    : Drawable(context)
{
    representedVertex = nullptr;
}

void VertexDisplay::create()
{
    OpenGLDS mDataArrays;
    if(representedVertex != nullptr)
    {
        mDataArrays = OpenGLDS(representedVertex->mPos);
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

GLenum VertexDisplay::drawMode()
{
    return GL_POINTS;
}

void VertexDisplay::updateVertex(Vertex *iV){
    representedVertex = iV;
}

FaceDisplay::FaceDisplay(OpenGLContext *context)
    : Drawable(context)
{
    representedFace = nullptr;
}

void FaceDisplay::create()
{
     OpenGLDS mDataArrays;
     if(representedFace != nullptr)
     {
        std::vector<glm::vec4> fVertexVector;
        HalfEdge* hePtr = representedFace->mEdge_Ptr;
        do{
            fVertexVector.push_back(hePtr->mVertex_Ptr->mPos);
            hePtr = hePtr->mNextEdge_Ptr;
        }while(hePtr != representedFace->mEdge_Ptr);

        glm::vec4 oppositeColor = representedFace->mColor - glm::vec4(1, 1, 1, 0);
        oppositeColor = glm::abs(oppositeColor);

        mDataArrays = OpenGLDS(fVertexVector, oppositeColor);
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

GLenum FaceDisplay::drawMode()
{
    return GL_LINES;
}

void FaceDisplay::updateFace(Face *iF)
{
    representedFace = iF;
}

HalfEdgeDisplay::HalfEdgeDisplay(OpenGLContext *context)
    : Drawable(context)
{
    representedHalfEdge = nullptr;
}

void HalfEdgeDisplay::create()
{
    OpenGLDS mDataArrays;
    if(representedHalfEdge != nullptr)
    {
        glm::vec4 vHead = representedHalfEdge->mVertex_Ptr->mPos;
        glm::vec4 vTail = representedHalfEdge->mSymEdge_Ptr->mVertex_Ptr->mPos;
        mDataArrays = OpenGLDS(vHead, vTail);
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

GLenum HalfEdgeDisplay::drawMode()
{
    return GL_LINES;
}

void HalfEdgeDisplay::updateHalfEdge(HalfEdge *iHE)
{
    representedHalfEdge = iHE;
}
/*
JointDisplay::JointDisplay(OpenGLContext *context)
    : Drawable(context)
{
    representedJoint = nullptr;
}

void JointDisplay::create()
{

}

GLenum JointDisplay::drawMode()
{
    return GL_LINES;
}

void JointDisplay::updateJoint(Joint *iJ)
{
    representedJoint = iJ;
}
*/
