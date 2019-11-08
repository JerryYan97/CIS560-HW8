#ifndef HALFEDGE_DATASTRUCTURE_H
#define HALFEDGE_DATASTRUCTURE_H
#include "joint.h"
#include "la.h"
#include <memory>
#include "drawable.h"
#include "polygon.h"
#include <QListWidget>


class HalfEdge;
class Face;
class OpenGLDS;

class JointInfluence
{
public:
    Joint* tarJoint;
    float influenceNum;
};

class Vertex : public QListWidgetItem
{
public:
    glm::vec4 mPos; // A vec3 for storing its position
    HalfEdge* mEdge_Ptr; // A pointer to one of the HalfEdges that points to this Vertex
    std::vector<JointInfluence> mJointInfluVec;
    static unsigned int mID; // A unique integer to identify the Vertex in menus and while debugging
    Vertex();
    ~Vertex(){}
    float SphereIntersection(glm::vec3 r0, glm::vec3 rd);
};

class HalfEdge : public QListWidgetItem
{
public:
    HalfEdge* mNextEdge_Ptr;
    HalfEdge* mSymEdge_Ptr;
    Face* mFace_Ptr;
    Vertex* mVertex_Ptr;
    static unsigned int mID;
    HalfEdge();
    ~HalfEdge(){}

};

class Face : public QListWidgetItem
{
public:
    HalfEdge* mEdge_Ptr;
    glm::vec4 mColor;
    static unsigned int mID;
    Face();
    ~Face(){}
    float PlaneIntersection(glm::vec3 r0, glm::vec3 rd);
};

class Mesh : public Drawable
{
public:
    glm::mat4 model;
    std::vector<std::unique_ptr<Vertex>> mVertexList;
    std::vector<std::unique_ptr<HalfEdge>> mHalfEdgeList;
    std::vector<std::unique_ptr<Face>> mFaceList;
    friend class OpenGLDS;
    Mesh(OpenGLContext* context);
    ~Mesh(){}
    void create();
    GLenum drawMode();
    void ConstructCube();
    void OpenGLDataConversion(OpenGLDS& iGLDS);
    void ConstructStandardCube();
    void Catmull_Clark_Subdivide();
    void AddMidPoints(std::vector<Vertex*>& mCentroidsVec, std::vector<Vertex*>& midPointsVec);
    void ExtrudeFace(Face* f, float dis);
    void Clear();
    void PolygonToMesh(MPolygon& iPolygon);
    Vertex* SphereTest(glm::vec3 eyePos, glm::vec4 modelSpaceRay, float& resT);
    Face* FaceTest(glm::vec3 eyePos, glm::vec4 modelSpaceRay, float& resT);
private:
    void SetHEConnection(HalfEdge* hePtr, int i);
    void SetFaceConnection(Face* f);
    void FindRelatedFaces(Face* centerFace, std::vector<Face*>& relatedFaces);
    void SetSymmetryInConversion();
    int FindMeshVertIdx(PVertex& iPVertex);
    void ConstructAndCreateAMeshLoopFace(Triangle& iTri);
    void ConstructAndCreateMeshVecs(MPolygon& iPolygon,
                                    std::vector<Triangle>& meshTriIndex);
    void ConstructAndCreateMeshLoopFaces(std::vector<Triangle>& meshTriIndex);

    void UpraiseTheOldPlane(Face* f, float dis);
    void ExtrudeAnEdge(HalfEdge* HE1);
    void ConstructOldFaceList(std::vector<Face*>& oldFaceList);
    void ConstructAndCreateHEInSubFace(Face *assignedFace,
                                       HalfEdge *midHE,
                                       HalfEdge *preTempHE,
                                       Vertex* centroid);
    void ConstructAndCreateFaces(Face* oriF, std::vector<Face*>& faceVec);
    void QuadrangleMesh(const std::vector<Vertex *> centroidsVec,
                        const std::vector<Vertex *> midPointsVec,
                        const std::vector<Vertex *> oriVertVec);
    void QuadrangleFace(Face* f, Vertex* centroid, std::vector<HalfEdge*> midHEVec);
    void ConstructOriVertVec(std::vector<Vertex*>& oriVertVec);
    void MoveOriVertices(const std::vector<Vertex*> centroidsVec,
                         const std::vector<Vertex*> midPointsVec,
                         std::vector<Vertex*> oriVertVec);
    void AddCentroids(std::vector<Vertex*>& centroidsVec);
    void AddACentroidToAFace(std::vector<Vertex*>& centroidsVec, Face* iFace);
    void SplitHE_HW7(glm::vec4 iPos, HalfEdge* he, std::vector<Vertex*>& midPointsVec);
};

//OpenGL Datastructure
class OpenGLDS{
public:
    std::vector<glm::vec4> arrPos;
    std::vector<glm::vec4> arrNor;
    std::vector<glm::vec4> arrCol;
    std::vector<GLuint> arrIdx;
    std::vector<int> arrInfluJointsIDs;
    std::vector<float> arrWeights;
    //Skeleton iSkeleton;
    OpenGLDS(){}
    OpenGLDS(Mesh& iMesh);
    OpenGLDS(Skeleton& iSkeleton);
    OpenGLDS(glm::vec4 iVertex); // used to draw a point
    OpenGLDS(glm::vec4 vHead, glm::vec4 vTail); // used to draw a line
    OpenGLDS(std::vector<glm::vec4>& ringVerticesList, glm::vec4 iColor); // used to draw a ring
    ~OpenGLDS(){}

protected:
    void ConstructAFrameWireSphere(glm::mat4 transformMat, FrameWireSphere& iSphere, bool selected);
    void ConstructSphereConnection(Joint* parent);
};

glm::vec3 CalculateVertexNorm(HalfEdge* hePtr);

#endif // HALFEDGE_DATASTRUCTURE_H
