#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>


MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
      m_geomSquare(this),
      m_progLambert(this), m_progFlat(this),
      m_glCamera(), m_Cube(this), m_vertDisplay(this),
      m_faceDisplay(this), m_halfEdgeDisplay(this), m_skeleton(this),
      m_progSKeleton(this)
{
    //m_jointDisplayVec.push_back(JointDisplay(this));
}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    m_geomSquare.destroy();
    m_Cube.destroy();
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    //Create the instances of Cylinder and Sphere.
    emit(DrawCow(true));
    emit(DrawSkeleton(true));


    // Create and set up the diffuse shader
    m_progLambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    m_progFlat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    m_progSKeleton.create(":/glsl/skeleton.vert.glsl", ":/glsl/skeleton.frag.glsl");


    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);


    emit SendMesh(m_Cube);
}

void MyGL::resizeGL(int w, int h)
{
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    m_glCamera = Camera(w, h);
    glm::mat4 viewproj = m_glCamera.getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    m_progLambert.setViewProjMatrix(viewproj);
    m_progFlat.setViewProjMatrix(viewproj);
    m_progSKeleton.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function update() is called, paintGL is called implicitly.
// It is called every frame
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 eye = m_glCamera.eye;
    glm::mat4 spherecalToWorld = glm::translate(glm::mat4(1.0f), m_glCamera.ref);
    glm::vec4 eyeWorld = spherecalToWorld * m_glCamera.CameraModelToSpherecal() * glm::vec4(eye.x, eye.y, eye.z, 1);

    m_progFlat.setViewProjMatrix(m_glCamera.getViewProj());
    m_progFlat.setCamPos(glm::vec3(eyeWorld.x, eyeWorld.y, eyeWorld.z));
    m_progLambert.setViewProjMatrix(m_glCamera.getViewProj());
    m_progLambert.setCamPos(glm::vec3(eyeWorld.x, eyeWorld.y, eyeWorld.z));
    m_progSKeleton.setViewProjMatrix(m_glCamera.getViewProj());
    m_progSKeleton.setCamPos(glm::vec3(eyeWorld.x, eyeWorld.y, eyeWorld.z));

    std::vector<glm::mat4> bindMatVec = m_skeleton.getBindMatVec();
    std::vector<glm::mat4> jointToWorldMatVec = m_skeleton.getJointToWorldMatVec();
    m_progSKeleton.setBindMatArray(bindMatVec);
    m_progSKeleton.setJointToWorldMatArray(jointToWorldMatVec);

//#define NOPE
#ifndef NOPE



    //Create a model matrix. This one scales the sphere uniformly by 3, then translates it by <-2,0,0>.
    //Note that we have to transpose the model matrix before passing it to the shader
    //This is because OpenGL expects column-major matrices, but you've
    //implemented row-major matrices.
    glm::mat4 model = glm::mat4(1.f);
    m_Cube.model = model;
    //Send the geometry's transformation matrix to the shader
    //m_progLambert.setModelMatrix(model);
    //Draw the example sphere using our lambert shader
    //m_progLambert.draw(m_Cube);
    m_progSKeleton.setModelMatrix(model);
    m_progSKeleton.jointNum = m_skeleton.mJointsList.size();
    m_progSKeleton.draw(m_Cube);

    glDisable(GL_DEPTH_TEST);
    m_progFlat.setModelMatrix(model);


    if(m_vertDisplay.representedVertex != nullptr)
    {
        m_vertDisplay.create();
        m_progFlat.setModelMatrix(model);
        m_progFlat.draw(m_vertDisplay);
    }

    if(m_faceDisplay.representedFace != nullptr)
    {
        m_faceDisplay.create();
        m_progFlat.setModelMatrix(model);
        m_progFlat.draw(m_faceDisplay);
    }

    if(m_halfEdgeDisplay.representedHalfEdge != nullptr)
    {
        m_halfEdgeDisplay.create();
        m_progFlat.setModelMatrix(model);
        m_progFlat.draw(m_halfEdgeDisplay);
    }
    /*
    m_jointDisplay.create();
    if(m_jointDisplay.representedJoint != nullptr)
    {
        m_progFlat.setModelMatrix(m_jointDisplay.representedJoint->getOverallTransformation());
    }
    m_progFlat.draw(m_jointDisplay);
    */
    if(m_skeleton.SkeletonReady())
    {
        m_skeleton.create();
        m_progFlat.setModelMatrix(m_skeleton.GetModelMat());
        m_progFlat.draw(m_skeleton);
    }


    glEnable(GL_DEPTH_TEST);

#endif
}

void MyGL::keyPressEvent(QKeyEvent *e)
{
    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    // This could all be much more efficient if a switch
    // statement were used
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        m_glCamera.RotateAboutUp(-amount);
    } else if (e->key() == Qt::Key_Left) {
        m_glCamera.RotateAboutUp(amount);
    } else if (e->key() == Qt::Key_Up) {
        m_glCamera.RotateAboutRight(-amount);
    } else if (e->key() == Qt::Key_Down) {
        m_glCamera.RotateAboutRight(amount);
    } else if (e->key() == Qt::Key_1) {
        m_glCamera.fovy += amount;
    } else if (e->key() == Qt::Key_2) {
        m_glCamera.fovy -= amount;
    } else if (e->key() == Qt::Key_W) {
        m_glCamera.TranslateAlongLook(amount);
    } else if (e->key() == Qt::Key_S) {
        m_glCamera.TranslateAlongLook(-amount);
    } else if (e->key() == Qt::Key_D) {
        m_glCamera.TranslateAlongRight(amount);
    } else if (e->key() == Qt::Key_A) {
        m_glCamera.TranslateAlongRight(-amount);
    } else if (e->key() == Qt::Key_Q) {
        m_glCamera.TranslateAlongUp(-amount);
    } else if (e->key() == Qt::Key_E) {
        m_glCamera.TranslateAlongUp(amount);
    } else if (e->key() == Qt::Key_R) {
        m_glCamera = Camera(this->width(), this->height());
    } else if (e->key() == Qt::Key_N) {
        // NEXT half-edge of the currently selected half-edge
        emit SendNextHEOfHE(m_halfEdgeDisplay.representedHalfEdge->mNextEdge_Ptr);
    } else if (e->key() == Qt::Key_M) {
        // SYM half-edge of the currently selected half-edge
        emit SendSYMHEOfHE(m_halfEdgeDisplay.representedHalfEdge->mSymEdge_Ptr);
    } else if (e->key() == Qt::Key_F) {
        // FACE of the currently selected half-edge
        emit SendFaceOfHE(m_halfEdgeDisplay.representedHalfEdge->mFace_Ptr);
    } else if (e->key() == Qt::Key_V) {
        // VERTEX of the currently selected half-edge
        emit SendVertexOfHE(m_halfEdgeDisplay.representedHalfEdge->mVertex_Ptr);
    } else if ((e->modifiers() == Qt::ShiftModifier) && (e->key() == Qt::Key_H)) {
        // HALF-EDGE of the currently selected face
        emit SendHEOfFace(m_faceDisplay.representedFace->mEdge_Ptr);
    } else if (e->key() == Qt::Key_H) {
        // HALF-EDGE of the currently selected vertex
        emit SendHEOfVertex(m_vertDisplay.representedVertex->mEdge_Ptr);
    }
    m_glCamera.RecomputeAttributes();
    update();  // Calls paintGL, among other things
}

void MyGL::DisplayClear()
{
    m_vertDisplay.updateVertex(nullptr);
    m_faceDisplay.updateFace(nullptr);
    m_halfEdgeDisplay.updateHalfEdge(nullptr);
    m_vertDisplay.destroy();
    m_faceDisplay.destroy();
    m_halfEdgeDisplay.destroy();
    m_vertDisplay.create();
    m_faceDisplay.create();
    m_halfEdgeDisplay.create();
}

void MyGL::SplitHE(HalfEdge* he, Mesh& iMesh)
{
    HalfEdge* he1 = he;
    HalfEdge* he2 = he1->mSymEdge_Ptr;

    // Create and Get the pos of middle vertex
    // the middle vertex is at the front of both halfedge
    Vertex* v1 = he1->mVertex_Ptr;
    Vertex* v2 = he2->mVertex_Ptr;
    std::unique_ptr<Vertex> uV3 = std::make_unique<Vertex>(Vertex());
    Vertex* v3 = uV3.get();
    glm::vec4 temp = v1->mPos + v2->mPos;
    v3->mPos = glm::vec4(temp.x / 2, temp.y / 2, temp.z / 2, temp.w / 2);

    // Create the two new half-edges HE1B and HE2B needed
    // to surround the middle vertex
    // HE1B points to V1
    // HE2B points to V2
    std::unique_ptr<HalfEdge> uHe1b = std::make_unique<HalfEdge>(HalfEdge());
    std::unique_ptr<HalfEdge> uHe2b = std::make_unique<HalfEdge>(HalfEdge());
    HalfEdge* he1b = uHe1b.get();
    HalfEdge* he2b = uHe2b.get();
    he1b->mVertex_Ptr = v1;
    he2b->mVertex_Ptr = v2;
    // set face pointers in HE1B and HE2B
    he1b->mFace_Ptr = he1->mFace_Ptr;
    he2b->mFace_Ptr = he2->mFace_Ptr;
    // Just create, do not affect other thing

    // Adjust the sym, next, and vert pointers of HE1, HE2,
    // HE1B, and HE2B
    he1b->mNextEdge_Ptr = he1->mNextEdge_Ptr;
    he2b->mNextEdge_Ptr = he2->mNextEdge_Ptr;
    he1->mNextEdge_Ptr = he1b;
    he2->mNextEdge_Ptr = he2b;
    he1->mVertex_Ptr = he2->mVertex_Ptr = v3;
    he1->mSymEdge_Ptr = he2b;
    he2b->mSymEdge_Ptr = he1;
    he2->mSymEdge_Ptr = he1b;
    he1b->mSymEdge_Ptr = he2;

    // Set the EdgePtr for v1, v2
    v1->mEdge_Ptr = he1b;
    v2->mEdge_Ptr = he2b;

    // Push v3, he1b and he2b into the Mesh Datastructure
    iMesh.mVertexList.push_back(std::move(uV3));
    iMesh.mHalfEdgeList.push_back(std::move(uHe1b));
    iMesh.mHalfEdgeList.push_back(std::move(uHe2b));
}

// For any number of half-edge
Face* MyGL::AddATriangle(Face *f, Mesh &iMesh)
{
    Face* face1 = f;
    HalfEdge* HE_0 = face1->mEdge_Ptr;
    // Create two new half-edges HE_A and HE_B
    std::unique_ptr<HalfEdge> uHE_A = std::make_unique<HalfEdge>(HalfEdge());
    std::unique_ptr<HalfEdge> uHE_B = std::make_unique<HalfEdge>(HalfEdge());
    HalfEdge* HE_A = uHE_A.get();
    HalfEdge* HE_B = uHE_B.get();
    HE_A->mVertex_Ptr = HE_0->mVertex_Ptr;
    // It looks like that it can only work in the condition of square->tri
    HE_B->mVertex_Ptr = HE_0->mNextEdge_Ptr->mNextEdge_Ptr->mVertex_Ptr;
    HE_A->mSymEdge_Ptr = HE_B;
    HE_B->mSymEdge_Ptr = HE_A;

    // Create a second face FACE2
    std::unique_ptr<Face> uFace2 = std::make_unique<Face>(Face());
    Face* face2 = uFace2.get();
    face2->mColor = face1->mColor;
    HE_A->mFace_Ptr = face2;
    HE_0->mNextEdge_Ptr->mFace_Ptr = face2;
    HE_0->mNextEdge_Ptr->mNextEdge_Ptr->mFace_Ptr = face2;
    HE_B->mFace_Ptr = face1;
    face2->mEdge_Ptr = HE_A;

    // Fix up the next pointers for our half-edges
    HE_B->mNextEdge_Ptr = HE_0->mNextEdge_Ptr->mNextEdge_Ptr->mNextEdge_Ptr;
    HE_0->mNextEdge_Ptr->mNextEdge_Ptr->mNextEdge_Ptr = HE_A;
    HE_A->mNextEdge_Ptr = HE_0->mNextEdge_Ptr;
    HE_0->mNextEdge_Ptr = HE_B;

    // Push everything into the mesh
    iMesh.mHalfEdgeList.push_back(std::move(uHE_A));
    iMesh.mHalfEdgeList.push_back(std::move(uHE_B));
    iMesh.mFaceList.push_back(std::move(uFace2));

    return face1;
}

int FaceEdgeNum(Face* f)
{
    int counter = 0;
    HalfEdge* hePtr = f->mEdge_Ptr;
    do{
        counter++;
        hePtr = hePtr->mNextEdge_Ptr;
    }while(hePtr != f->mEdge_Ptr);
    return counter;
}

void MyGL::TriangulateFace(Face *f, Mesh &iMesh)
{
    Face* oldFace;
    for(;;)
    {
        oldFace = AddATriangle(f, iMesh);
        int faceVNum = FaceEdgeNum(oldFace);
        if(faceVNum <= 3)
        {
            break;
        }
    }
}

bool MyGL::CheckPlanar(Face *f)
{
    int count = FaceEdgeNum(f);
    if(count <= 3)
    {
        return true;
    }else{
        // Get every normal for every vertices in this face
        HalfEdge* hePtr = f->mEdge_Ptr;
        std::vector<glm::vec3> normList;
        do{
            glm::vec3 nor = CalculateVertexNorm(hePtr);
            // Ignore the vertex that is align with front and back vertex
            if((glm::abs(nor.x) > DBL_EPSILON) || (glm::abs(nor.y) > DBL_EPSILON) || (glm::abs(nor.z) > DBL_EPSILON))
            {
                normList.push_back(nor);
            }
            hePtr = hePtr->mNextEdge_Ptr;
        }while(hePtr != f->mEdge_Ptr);
        // Check whether these normal are same
        for(std::vector<glm::vec3>::iterator norPtr = normList.begin(); norPtr != normList.end(); norPtr++)
        {
            std::vector<glm::vec3>::iterator nextPtr = norPtr + 1;
            for(;nextPtr != normList.end(); nextPtr++)
            {
                if(*norPtr != *nextPtr)
                {
                    TriangulateFace(f, m_Cube);
                    return false;
                }
            }
        }
        return true;
    }
}

void MyGL::AdjustMeshPlanar(Mesh &iMesh)
{
    for(unsigned int i = 0; i < iMesh.mFaceList.size(); i++)
    {
        Face* fPtr = iMesh.mFaceList.at(i).get();
        CheckPlanar(fPtr);
    }
}

glm::vec2 MyGL::GetNDCPosition(int ix, int iy)
{
    glm::vec2 sPos;
    sPos.x = (2 * ix / float(this->width())) - 1;
    sPos.y = 1 - (2 * iy / float(this->height()));
    return sPos;
}

glm::vec4 MyGL::GetRayDirction(QMouseEvent *e)
{
     glm::vec2 mouseNDCPos = GetNDCPosition(e->x(), e->y());
     glm::mat4 viewProjMat = this->m_glCamera.getViewProj();

     glm::vec3 eye = m_glCamera.eye;

     glm::mat4 spherecalToWorld = glm::translate(glm::mat4(1.0f), m_glCamera.ref);
     glm::vec4 eyeWorld = spherecalToWorld * m_glCamera.CameraModelToSpherecal() * glm::vec4(eye.x, eye.y, eye.z, 1);
     glm::vec4 lookWorldVec4 = glm::inverse(viewProjMat) *
             glm::vec4(m_glCamera.look.x, m_glCamera.look.y, m_glCamera.look.z, 0);
     glm::vec3 lookWorldNormal = glm::normalize(glm::vec3(lookWorldVec4.x, lookWorldVec4.y, lookWorldVec4.z));


     glm::vec4 ref = glm::vec4(m_glCamera.ref.x, m_glCamera.ref.y, m_glCamera.ref.z, 1);

     glm::vec3 tempH = m_glCamera.H;
     glm::vec3 tempV = m_glCamera.V;
     glm::vec4 p = ref + mouseNDCPos.x * glm::vec4(m_glCamera.H.x, m_glCamera.H.y, m_glCamera.H.z, 0) +
             mouseNDCPos.y * glm::vec4(m_glCamera.V.x, m_glCamera.V.y, m_glCamera.V.z, 0);
     glm::vec3 ray = glm::vec3(p.x, p.y, p.z) - glm::vec3(eyeWorld.x, eyeWorld.y, eyeWorld.z);
     ray = glm::normalize(ray);
     glm::vec4 modelRay = glm::inverse(m_Cube.model) * glm::vec4(ray.x, ray.y, ray.z, 0);
     return glm::vec4(ray.x, ray.y, ray.z, 1);
}


void MyGL::IntersectionTest(glm::vec4 modelSpaceRay)
{
    glm::vec3 eye = m_glCamera.eye;
    glm::mat4 spherecalToWorld = glm::translate(glm::mat4(1.0f), m_glCamera.ref);
    glm::vec4 eyeWorld = spherecalToWorld * m_glCamera.CameraModelToSpherecal() * glm::vec4(eye.x, eye.y, eye.z, 1);
    glm::vec4 eyeModel = eyeWorld;

    float vertexnearestT;
    Vertex* nearestVert = this->m_Cube.SphereTest(glm::vec3(eyeModel.x, eyeModel.y, eyeModel.z), modelSpaceRay, vertexnearestT);

    float facenearestT;
    Face* nearestFace = this->m_Cube.FaceTest(glm::vec3(eyeModel.x, eyeModel.y, eyeModel.z), modelSpaceRay, facenearestT);

    if(nearestVert != nullptr && nearestFace != nullptr)
    {
        if(vertexnearestT < facenearestT)
        {
            std::cout << "pos:" << nearestVert->mPos.x << "," << nearestVert->mPos.y << ","
                      << nearestVert->mPos.z << std::endl;
            emit HighlightVert(nearestVert);
        }
        else
        {
            std::cout << "Activitied Face:" << nearestFace->text().toStdString() << std::endl;
            emit HighlightFace(nearestFace);
        }
    }
    else
    {
        if(nearestVert != nullptr)
        {
            std::cout << "pos:" << nearestVert->mPos.x << "," << nearestVert->mPos.y << ","
                      << nearestVert->mPos.z << std::endl;
            emit HighlightVert(nearestVert);
        }
        if(nearestFace != nullptr)
        {
            std::cout << "Activitied Face:" << nearestFace->text().toStdString() << std::endl;
            emit HighlightFace(nearestFace);
        }
    }

}

double CalDistance(glm::vec4 v1, glm::vec4 v2)
{
    glm::vec4 lenVec = v1 - v2;
    lenVec.w = 0;
    return glm::length(lenVec);
}

int FindFarestVert(const std::vector<Vertex*>& vertVec, glm::vec4 tarPos)
{
    Vertex* farestVert = vertVec.at(0);
    double farestDis = CalDistance(farestVert->mPos, tarPos);
    int farestPos = 0;
    for(unsigned int i = 1; i < vertVec.size(); i++)
    {
        Vertex* currVert = vertVec.at(i);
        double currDis = CalDistance(currVert->mPos, tarPos);
        if(currDis > farestDis)
        {
            farestDis = currDis;
            farestVert = currVert;
            farestPos = i;
        }
    }
    return farestPos;
}

// When you need to implement the heat diffusion algorthim
// you can just make the vertNum become 1.
void FindNearestVertices(unsigned int vertNum, Mesh &iMesh, Joint* tarJoint, std::vector<Vertex*>& nearestVertVec)
{
    for(unsigned int i = 0; i < iMesh.mVertexList.size(); i++)
    {
        Vertex* vPtr = iMesh.mVertexList.at(i).get();
        if(nearestVertVec.size() < vertNum)
        {
            nearestVertVec.push_back(vPtr);
        }
        else
        {
            // Find the farest vert in the nearestVertVec
            // If it is nearer than the current vPtr, then do nothing.
            // If it is farer than the current vPtr, then replace it.
            glm::vec4 jointDis = tarJoint->getOverallTransformation() * glm::vec4(0, 0, 0, 1);
            int nearestFarPos = FindFarestVert(nearestVertVec, jointDis);
            Vertex* nearestFarVPtr = nearestVertVec.at(nearestFarPos);
            double nearestFarDis = CalDistance(nearestFarVPtr->mPos, jointDis);
            double currDis = CalDistance(vPtr->mPos, jointDis);
            if(nearestFarDis > currDis)
            {
                //nearestFarVPtr = vPtr;
                nearestVertVec.at(nearestFarPos) = vPtr;
            }
        }
    }
}

// Heat Diffusion
void SetInfluenceForThisSkeletonBrunch(Mesh &iMesh, Joint *mJoint)
{
    std::vector<Vertex*> nearestVertVec;
    unsigned int nearestVetNum = 2;
    unsigned int influPerVertNum = 2;
    glm::vec4 jointPos = mJoint->getOverallTransformation() * glm::vec4(0, 0, 0, 1);
    double sumDis = 0;

    FindNearestVertices(nearestVetNum, iMesh, mJoint, nearestVertVec);

    // Get sumDis.
    for(unsigned int i = 0; i < nearestVertVec.size(); i++)
    {
        Vertex* currVertPtr = nearestVertVec.at(i);
        sumDis += CalDistance(jointPos, currVertPtr->mPos);
    }

    // For each nearest vertice for this joint.
    for(unsigned int i = 0; i < nearestVertVec.size(); i++)
    {
        Vertex* currVertPtr = nearestVertVec.at(i);
        // Set the influence for this vertex.
        JointInfluence jInflu;
        double currDis = CalDistance(jointPos, currVertPtr->mPos);
        jInflu.influenceNum = 1 - (currDis / sumDis);
        jInflu.tarJoint = mJoint;
        currVertPtr->mJointInfluVec.push_back(jInflu);
    }

    // Set influence for children joints.
    for(std::vector<Joint*>::iterator i = mJoint->mChildJointsVec.begin(); i != mJoint->mChildJointsVec.end(); i++)
    {
        SetInfluenceForThisSkeletonBrunch(iMesh, *i);
    }
}
/***/


// Find the Farest joint to target vertex
// return the id in nearestJointVec
int FindFarestJoint(std::vector<Joint*>& nearestJointVec, glm::vec4 tarVertPos)
{
    Joint* farestJoint = nearestJointVec.at(0);
    glm::vec4 farestJointPos = farestJoint->getOverallTransformation() * glm::vec4(0, 0, 0, 1);
    double farestDis = CalDistance(farestJointPos, tarVertPos);
    int farestPos = 0;

    for(unsigned int i = 1; i < nearestJointVec.size(); i++)
    {
        Joint* currJoint = nearestJointVec.at(i);
        glm::vec4 currJointPos = currJoint->getOverallTransformation() * glm::vec4(0, 0, 0, 1);
        double currDis = CalDistance(currJointPos, tarVertPos);
        if(currDis > farestDis)
        {
            farestPos = i;
        }
    }

    return farestPos;
}

// jointNum = 2
void Find2NearestJoint(Vertex* tarVet, Skeleton& iSkeleton, std::vector<Joint*>& nearestJointVec)
{
    for(unsigned int i = 0; i < iSkeleton.mJointsList.size(); i++)
    {
        Joint* currJoint = iSkeleton.mJointsList.at(i).get();
        // For each joint.
        if(nearestJointVec.size() < 2)
        {
            // If the nearestJointVec is not full ( < 2 ), then just push into it.
            nearestJointVec.push_back(currJoint);
        }
        else
        {
            // If the nearestJointVec is full ( = 2 ), then Find the Farest joint and determine
            // whether this joint's should be replaced.
            glm::vec4 jointPos = currJoint->getOverallTransformation() * glm::vec4(0, 0, 0, 1);
            double currDis = CalDistance(jointPos, tarVet->mPos);
            int jointID = FindFarestJoint(nearestJointVec, tarVet->mPos);

            Joint* farestJoint = nearestJointVec.at(jointID);
            glm::vec4 farestJointPos = farestJoint->getOverallTransformation() * glm::vec4(0, 0, 0, 1);
            double nearestFarDis = CalDistance(farestJointPos, tarVet->mPos);

            if(currDis < nearestFarDis)
            {
                nearestJointVec.at(jointID) = currJoint;
            }
        }
    }
}

// Closest Joint Function
void SetInfluenceForThisVertex(Vertex* tarVet, Skeleton& iSkeleton)
{
    /*
    for(unsigned int i = 0; i < tarVet)
    {
        Vertex* currVertPtr = nearestVertVec.at(i);
        // Set the influence for this vertex.
        JointInfluence jInflu;
        double currDis = CalDistance(jointPos, currVertPtr->mPos);
        jInflu.influenceNum = 1 - (currDis / sumDis);
        jInflu.tarJoint = mJoint;
        currVertPtr->mJointInfluVec.push_back(jInflu);
    }
    */
    std::vector<Joint*> nearestJointVec;
    Find2NearestJoint(tarVet, iSkeleton, nearestJointVec);
    // Set the influence for this vertex.
    Joint* j1 = nearestJointVec.at(0);
    Joint* j2 = nearestJointVec.at(1);
    JointInfluence j1Influ;
    JointInfluence j2Influ;

    glm::vec4 j1Pos = j1->getOverallTransformation() * glm::vec4(0, 0, 0, 1);
    glm::vec4 j2Pos = j2->getOverallTransformation() * glm::vec4(0, 0, 0, 1);

    double j1Dis = CalDistance(j1Pos, tarVet->mPos);
    double j2Dis = CalDistance(j2Pos, tarVet->mPos);
    double sumDis = j1Dis + j2Dis;

    j1Influ.influenceNum = 1 - (j1Dis / sumDis);
    j1Influ.tarJoint = j1;
    j2Influ.influenceNum = 1 - (j2Dis / sumDis);
    j2Influ.tarJoint = j2;

    tarVet->mJointInfluVec.push_back(j1Influ);
    tarVet->mJointInfluVec.push_back(j2Influ);
}

void SetBindMatForEachJoint(Skeleton& iSkeleton)
{
    for(unsigned int i = 0; i < iSkeleton.mJointsList.size(); i++)
    {
        Joint* jPtr = iSkeleton.mJointsList.at(i).get();
        jPtr->mBindMatrix = glm::inverse(jPtr->getOverallTransformation());
    }
}

void MyGL::Skinning(Mesh& iMesh, Skeleton& iSkeleton)
{
    for(unsigned int i = 0; i < iMesh.mVertexList.size(); i++)
    {
        SetInfluenceForThisVertex(iMesh.mVertexList.at(i).get(), iSkeleton);
    }
    SetBindMatForEachJoint(iSkeleton);
}

