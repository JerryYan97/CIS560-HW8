#ifndef PRIMITIVEDISPLAY_H
#define PRIMITIVEDISPLAY_H
#include "halfedge_datastructure.h"
#include "joint.h"

class VertexDisplay : public Drawable{
protected:

public:
    Vertex *representedVertex;
    void create() override;
    void updateVertex(Vertex* iV);
    GLenum drawMode();
    VertexDisplay(OpenGLContext* context);
    ~VertexDisplay(){}
};

class FaceDisplay : public Drawable{
protected:

public:
    Face *representedFace;
    void create() override;
    void updateFace(Face* iF);
    GLenum drawMode();
    FaceDisplay(OpenGLContext* context);
    ~FaceDisplay(){}
};

class HalfEdgeDisplay : public Drawable{
protected:

public:
    HalfEdge *representedHalfEdge;
    void create() override;
    void updateHalfEdge(HalfEdge* iHE);
    GLenum drawMode();
    HalfEdgeDisplay(OpenGLContext* context);
    ~HalfEdgeDisplay(){}
};

/*
class JointDisplay : public Drawable{
public:
    Joint* representedJoint;
    void create() override;
    void updateJoint(Joint* iJ);
    GLenum drawMode();
    JointDisplay(OpenGLContext* context);
};

class JointConnectionDisplay : public Drawable{
public:
    Joint* startJoint;
    Joint* endJoint;
    void create() override;
    void updateJointConnection(Joint* iJStart, Joint* iJEnd);
    GLenum drawMode();
    JointConnectionDisplay(OpenGLContext* context);
    ~JointConnectionDisplay(){}
};
*/
#endif // PRIMITIVEDISPLAY_H
