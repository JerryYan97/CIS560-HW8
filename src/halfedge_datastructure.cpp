#include "halfedge_datastructure.h"
#include <cstdlib>
#include <cfloat>
#include <cmath>
#include <iostream>

unsigned int Vertex::mID = 0;
unsigned int HalfEdge::mID = 0;
unsigned int Face::mID = 0;


/// General Support ///
glm::vec4 CalculateFaceNorm(Face* f)
{
    glm::vec3 accuNorm = glm::vec3(0, 0, 0);
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
    // Calculate the average of the normals in the normlist
    for(std::vector<glm::vec3>::iterator norPtr = normList.begin(); norPtr != normList.end(); norPtr++)
    {
        accuNorm += *norPtr;
    }
    int counter = normList.size();

    return glm::vec4(accuNorm.x / counter, accuNorm.y / counter, accuNorm.z / counter, 0);
}

HalfEdge* FindPreHE(HalfEdge* const iHE)
{
    HalfEdge* hePtr = iHE;
    do{
        if(hePtr->mNextEdge_Ptr == iHE)
        {
            return hePtr;
        }
        hePtr = hePtr->mNextEdge_Ptr;
    }while(hePtr != iHE);
}

glm::vec4 VertPosSum(const std::vector<Vertex*>& vertVec)
{
    glm::vec4 temp = glm::vec4(0, 0, 0, 0);
    for(unsigned int i = 0; i < vertVec.size(); i++)
    {
        Vertex* tempVPtr = vertVec.at(i);
        temp += tempVPtr->mPos;
    }
    return temp;
}

HalfEdge* FindHEinFacePointsToVert(const Vertex* tarVert, const Face* f)
{
    HalfEdge* hePtr = f->mEdge_Ptr;
    do{
        if(hePtr->mVertex_Ptr == tarVert)
        {
            return hePtr;
        }
        hePtr = hePtr->mNextEdge_Ptr;
    }while(hePtr != f->mEdge_Ptr);
}


bool VertInVec(const Vertex* tarVet, const std::vector<Vertex*>& vetVec)
{
    for(unsigned int i = 0; i < vetVec.size(); i++)
    {
        if(tarVet == vetVec.at(i))
        {
            return true;
        }
    }
    return false;
}

unsigned int GetEdgeNum(Face* f)
{
    HalfEdge* hePtr = f->mEdge_Ptr;
    unsigned int i = 0;
    do{
        i++;
        hePtr = hePtr->mNextEdge_Ptr;
    }while(hePtr != f->mEdge_Ptr);
    return i;
}

bool GLMVec4Same(glm::vec4 a, glm::vec4 b)
{
    for(unsigned int i = 0; i < 4; i++)
    {
        if(std::abs(a[i] - b[i]) > DBL_EPSILON)
        {
            return false;
        }
    }
    return true;
}

// If we cannot find a same one, then we return -1
int Mesh::FindMeshVertIdx(PVertex& iPVertex)
{
    glm::vec4 a = iPVertex.m_pos;
    for(unsigned int i = 0; i < this->mVertexList.size(); i++)
    {
        Vertex* temp = this->mVertexList.at(i).get();
        glm::vec4 b = temp->mPos;
        if(GLMVec4Same(a, b))
        {
            return i;
        }
    }
    return -1;
}



bool VertexInFace(Vertex* v, Face* f)
{
    HalfEdge* hePtr = f->mEdge_Ptr;
    do{
        if(hePtr->mVertex_Ptr == v)
        {
            return true;
        }
        hePtr = hePtr->mNextEdge_Ptr;
    }while(hePtr != f->mEdge_Ptr);
    return false;
}


HalfEdge* NextAdjFaceTarHE(const HalfEdge* currentHE)
{
    HalfEdge* symHEPtr = currentHE->mSymEdge_Ptr;
    HalfEdge* nextHEPtr = FindPreHE(symHEPtr);
    return nextHEPtr;
}

void FindAdjFaces(const Vertex* tarVet, std::vector<Face*>& adjFacesVec)
{
    HalfEdge* temp = tarVet->mEdge_Ptr;
    do{
        adjFacesVec.push_back(temp->mFace_Ptr);
        temp = NextAdjFaceTarHE(temp);
    }while(temp != tarVet->mEdge_Ptr);
}

/// ************************* ///
Vertex::Vertex()
{
    this->setText(QString::number(mID));
    mID++;
}

float Vertex::SphereIntersection(glm::vec3 r0, glm::vec3 rd)
{
    glm::vec4 rc = this->mPos;
    float A = rd.x * rd.x + rd.y * rd.y + rd.z * rd.z;
    float B = 2 * (rd.x * (r0.x - rc.x) + rd.y * (r0.y - rc.y) + rd.z * (r0.z - rc.z));
    float C = (r0.x - rc.x) * (r0.x - rc.x) + (r0.y - rc.y) * (r0.y - rc.y) + (r0.z - rc.z) * (r0.z - rc.z) - 0.5 * 0.5;

    float dis = B * B - 4 * A * C;
    if(dis < 0)
    {
        return -1;
    }
    float t0 = (-B - glm::sqrt(dis)) / (2 * A);
    if(t0 > 0)
    {
        return t0;
    }
    else
    {
        float t1 = (-B + glm::sqrt(B * B - 4 * A * C)) / (2 * A);
        return t1;
    }
}

HalfEdge::HalfEdge()
{
    this->setText(QString::number(mID));
    mID++;
}



Face::Face()
{
    this->setText(QString::number(mID));
    mID++;
}

bool BerycentricTest(glm::vec3 iv1, glm::vec3 iv2, glm::vec3 iv3, glm::vec3 iPos)
{
    glm::vec3 p1 = iv1;
    glm::vec3 p2 = iv2;
    glm::vec3 p3 = iv3;

    glm::vec3 v1_2 = p2 - p1;
    glm::vec3 v1_3 = p3 - p1;
    float s = glm::length(glm::cross(v1_2, v1_3)) / 2;

    glm::vec3 v1_iPos = iPos - p1;
    float s2 = glm::length(glm::cross(v1_iPos, v1_3)) / 2;
    float s3 = glm::length(glm::cross(v1_iPos, v1_2)) / 2;

    glm::vec3 viPos_2 = p2 - iPos;
    glm::vec3 viPos_3 = p3 - iPos;
    float s1 = glm::length(glm::cross(viPos_2, viPos_3)) / 2;

    float r1 = s1 / s;
    float r2 = s2 / s;
    float r3 = s3 / s;

    if((r1 > 0 && r1 < 1) && (r2 > 0 && r2 < 1) && (r3 > 0 && r3 < 1) && ((r1 + r2 + r3) == 1))
    {
        return true;
    }
    else
    {
        return false;
    }
}

float Face::PlaneIntersection(glm::vec3 r0, glm::vec3 rd)
{
    glm::vec4 nor = CalculateFaceNorm(this);
    glm::vec3 norNormilized = glm::normalize(glm::vec3(nor.x, nor.y, nor.z));
    glm::vec4 norNormilizedVec4 = glm::vec4(norNormilized.x, norNormilized.y, norNormilized.z, 0);
    glm::vec4 s = this->mEdge_Ptr->mVertex_Ptr->mPos;
    float t = glm::dot(norNormilizedVec4, s - glm::vec4(r0.x, r0.y, r0.z, 0)) / glm::dot(norNormilized, rd);
    glm::vec3 p = r0 + t * rd;
    std::cout << "t:" << t << std::endl;
    std::cout << "p:" << p.x << " " << p.y << " " << p.z << std::endl;

    HalfEdge* hePtr = this->mEdge_Ptr->mNextEdge_Ptr;
    do{
        Vertex* v0 = this->mEdge_Ptr->mVertex_Ptr;
        glm::vec3 v0Pos = glm::vec3(v0->mPos.x, v0->mPos.y, v0->mPos.z);
        Vertex* v1 = hePtr->mVertex_Ptr;
        glm::vec3 v1Pos = glm::vec3(v1->mPos.x, v1->mPos.y, v1->mPos.z);
        Vertex* v2 = hePtr->mNextEdge_Ptr->mVertex_Ptr;
        glm::vec3 v2Pos = glm::vec3(v2->mPos.x, v2->mPos.y, v2->mPos.z);
        if(BerycentricTest(v0Pos, v1Pos, v2Pos, p))
        {
            return t;
        }
        hePtr = hePtr->mNextEdge_Ptr;
    }while(hePtr->mNextEdge_Ptr != this->mEdge_Ptr);
    return -1;
}

Mesh::Mesh(OpenGLContext *context)
    : Drawable(context)
{}

GLenum Mesh::drawMode()
{
    return GL_TRIANGLES;
}

// Populates the VBOs of the Drawable
// Organizing our data to make them fulfill the requirement of Opengl Triangle
// Meaning create, put opengl data into VBO.
void Mesh::create()
{
    // Convert half-edge ds to opengl ds
    OpenGLDS mDataArrays = OpenGLDS(*this);

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

    generateInfluJointID();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufInfluJointsID);
    mp_context->glBufferData(GL_ARRAY_BUFFER, mDataArrays.arrInfluJointsIDs.size() * sizeof(int), mDataArrays.arrInfluJointsIDs.data(), GL_STATIC_DRAW);

    generateWeights();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufWeights);
    mp_context->glBufferData(GL_ARRAY_BUFFER, mDataArrays.arrWeights.size() * sizeof(float), mDataArrays.arrWeights.data(), GL_STATIC_DRAW);

    generateInfluJointIDArray();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufInfluJointsIDArray);
    mp_context->glBufferData(GL_ARRAY_BUFFER, mDataArrays.arrInfluJointsIDs.size() * sizeof(int), mDataArrays.arrInfluJointsIDs.data(), GL_STATIC_DRAW);

    generateWeightsArray();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufWeightsArray);
    mp_context->glBufferData(GL_ARRAY_BUFFER, mDataArrays.arrWeights.size() * sizeof(float), mDataArrays.arrWeights.data(), GL_STATIC_DRAW);
}

bool PairMatch(std::pair<HalfEdge*, Vertex*>& a, std::pair<HalfEdge*, Vertex*>& b)
{
    if(a.second == b.first->mVertex_Ptr && b.second == a.first->mVertex_Ptr)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Mesh::ConstructCube()
{
    // Create all Vertices
    std::array<std::unique_ptr<Vertex>, 8> vArray;
    for(int i = 0; i < 8; i++)
    {
        vArray.at(i) = std::make_unique<Vertex>(Vertex());
    }
    // Create all Half-edges
    std::array<std::unique_ptr<HalfEdge>, 24> hArray;
    for(int i = 0; i < 24; i++)
    {
        hArray.at(i) = std::make_unique<HalfEdge>(HalfEdge());
    }
    // Create all Faces
    std::array<std::unique_ptr<Face>, 6> fArray;
    for(int i = 0; i < 6; i++)
    {
        fArray.at(i) = std::make_unique<Face>(Face());
    }

    // Input vertices data
    vArray.at(0)->mPos = glm::vec4( 0.5,  0.5,  0.5, 1);
    vArray.at(1)->mPos = glm::vec4(-0.5,  0.5,  0.5, 1);
    vArray.at(2)->mPos = glm::vec4(-0.5, -0.5,  0.5, 1);
    vArray.at(3)->mPos = glm::vec4( 0.5, -0.5,  0.5, 1);
    vArray.at(4)->mPos = glm::vec4( 0.5,  0.5, -0.5, 1);
    vArray.at(5)->mPos = glm::vec4(-0.5,  0.5, -0.5, 1);
    vArray.at(6)->mPos = glm::vec4(-0.5, -0.5, -0.5, 1);
    vArray.at(7)->mPos = glm::vec4( 0.5, -0.5, -0.5, 1);

    vArray.at(0)->mEdge_Ptr = hArray.at(3).get();
    vArray.at(1)->mEdge_Ptr = hArray.at(0).get();
    vArray.at(2)->mEdge_Ptr = hArray.at(1).get();
    vArray.at(3)->mEdge_Ptr = hArray.at(2).get();
    vArray.at(4)->mEdge_Ptr = hArray.at(10).get();
    vArray.at(5)->mEdge_Ptr = hArray.at(9).get();
    vArray.at(6)->mEdge_Ptr = hArray.at(8).get();
    vArray.at(7)->mEdge_Ptr = hArray.at(11).get();

    // Input faces data
    fArray.at(0)->mColor = glm::vec4(1, 0, 0, 1);
    fArray.at(1)->mColor = glm::vec4(0, 1, 0, 1);
    fArray.at(2)->mColor = glm::vec4(0, 0, 1, 1);
    fArray.at(3)->mColor = glm::vec4(1, 1, 0, 1);
    fArray.at(4)->mColor = glm::vec4(1, 0, 1, 1);
    fArray.at(5)->mColor = glm::vec4(0, 1, 1, 1);

    fArray.at(0)->mEdge_Ptr = hArray.at(3).get();
    fArray.at(1)->mEdge_Ptr = hArray.at(7).get();
    fArray.at(2)->mEdge_Ptr = hArray.at(11).get();
    fArray.at(3)->mEdge_Ptr = hArray.at(15).get();
    fArray.at(4)->mEdge_Ptr = hArray.at(19).get();
    fArray.at(5)->mEdge_Ptr = hArray.at(23).get();

    // Input half-edge data
    // mNextEdge_Ptr
    hArray.at(0)->mNextEdge_Ptr = hArray.at(1).get();
    hArray.at(1)->mNextEdge_Ptr = hArray.at(2).get();
    hArray.at(2)->mNextEdge_Ptr = hArray.at(3).get();
    hArray.at(3)->mNextEdge_Ptr = hArray.at(0).get();

    hArray.at(4)->mNextEdge_Ptr = hArray.at(5).get();
    hArray.at(5)->mNextEdge_Ptr = hArray.at(6).get();
    hArray.at(6)->mNextEdge_Ptr = hArray.at(7).get();
    hArray.at(7)->mNextEdge_Ptr = hArray.at(4).get();

    hArray.at(8)->mNextEdge_Ptr = hArray.at(9).get();
    hArray.at(9)->mNextEdge_Ptr = hArray.at(10).get();
    hArray.at(10)->mNextEdge_Ptr = hArray.at(11).get();
    hArray.at(11)->mNextEdge_Ptr = hArray.at(8).get();

    hArray.at(12)->mNextEdge_Ptr = hArray.at(13).get();
    hArray.at(13)->mNextEdge_Ptr = hArray.at(14).get();
    hArray.at(14)->mNextEdge_Ptr = hArray.at(15).get();
    hArray.at(15)->mNextEdge_Ptr = hArray.at(12).get();

    hArray.at(16)->mNextEdge_Ptr = hArray.at(17).get();
    hArray.at(17)->mNextEdge_Ptr = hArray.at(18).get();
    hArray.at(18)->mNextEdge_Ptr = hArray.at(19).get();
    hArray.at(19)->mNextEdge_Ptr = hArray.at(16).get();

    hArray.at(20)->mNextEdge_Ptr = hArray.at(21).get();
    hArray.at(21)->mNextEdge_Ptr = hArray.at(22).get();
    hArray.at(22)->mNextEdge_Ptr = hArray.at(23).get();
    hArray.at(23)->mNextEdge_Ptr = hArray.at(20).get();

    // mSymEdge_Ptr
    hArray.at(0)->mSymEdge_Ptr = hArray.at(6).get();
    hArray.at(1)->mSymEdge_Ptr = hArray.at(20).get();
    hArray.at(2)->mSymEdge_Ptr = hArray.at(12).get();
    hArray.at(3)->mSymEdge_Ptr = hArray.at(16).get();

    hArray.at(4)->mSymEdge_Ptr = hArray.at(10).get();
    hArray.at(5)->mSymEdge_Ptr = hArray.at(21).get();
    hArray.at(6)->mSymEdge_Ptr = hArray.at(0).get();
    hArray.at(7)->mSymEdge_Ptr = hArray.at(19).get();

    hArray.at(8)->mSymEdge_Ptr = hArray.at(14).get();
    hArray.at(9)->mSymEdge_Ptr = hArray.at(22).get();
    hArray.at(10)->mSymEdge_Ptr = hArray.at(4).get();
    hArray.at(11)->mSymEdge_Ptr = hArray.at(18).get();

    hArray.at(12)->mSymEdge_Ptr = hArray.at(2).get();
    hArray.at(13)->mSymEdge_Ptr = hArray.at(23).get();
    hArray.at(14)->mSymEdge_Ptr = hArray.at(8).get();
    hArray.at(15)->mSymEdge_Ptr = hArray.at(17).get();

    hArray.at(16)->mSymEdge_Ptr = hArray.at(3).get();
    hArray.at(17)->mSymEdge_Ptr = hArray.at(15).get();
    hArray.at(18)->mSymEdge_Ptr = hArray.at(11).get();
    hArray.at(19)->mSymEdge_Ptr = hArray.at(7).get();

    hArray.at(20)->mSymEdge_Ptr = hArray.at(1).get();
    hArray.at(21)->mSymEdge_Ptr = hArray.at(5).get();
    hArray.at(22)->mSymEdge_Ptr = hArray.at(9).get();
    hArray.at(23)->mSymEdge_Ptr = hArray.at(13).get();

    // mFace_Ptr
    hArray.at(0)->mFace_Ptr = fArray.at(0).get();
    hArray.at(1)->mFace_Ptr = fArray.at(0).get();
    hArray.at(2)->mFace_Ptr = fArray.at(0).get();
    hArray.at(3)->mFace_Ptr = fArray.at(0).get();

    hArray.at(4)->mFace_Ptr = fArray.at(1).get();
    hArray.at(5)->mFace_Ptr = fArray.at(1).get();
    hArray.at(6)->mFace_Ptr = fArray.at(1).get();
    hArray.at(7)->mFace_Ptr = fArray.at(1).get();

    hArray.at(8)->mFace_Ptr = fArray.at(2).get();
    hArray.at(9)->mFace_Ptr = fArray.at(2).get();
    hArray.at(10)->mFace_Ptr = fArray.at(2).get();
    hArray.at(11)->mFace_Ptr = fArray.at(2).get();

    hArray.at(12)->mFace_Ptr = fArray.at(3).get();
    hArray.at(13)->mFace_Ptr = fArray.at(3).get();
    hArray.at(14)->mFace_Ptr = fArray.at(3).get();
    hArray.at(15)->mFace_Ptr = fArray.at(3).get();

    hArray.at(16)->mFace_Ptr = fArray.at(4).get();
    hArray.at(17)->mFace_Ptr = fArray.at(4).get();
    hArray.at(18)->mFace_Ptr = fArray.at(4).get();
    hArray.at(19)->mFace_Ptr = fArray.at(4).get();

    hArray.at(20)->mFace_Ptr = fArray.at(5).get();
    hArray.at(21)->mFace_Ptr = fArray.at(5).get();
    hArray.at(22)->mFace_Ptr = fArray.at(5).get();
    hArray.at(23)->mFace_Ptr = fArray.at(5).get();

    // mVertex_Ptr
    hArray.at(0)->mVertex_Ptr = vArray.at(1).get();
    hArray.at(1)->mVertex_Ptr = vArray.at(2).get();
    hArray.at(2)->mVertex_Ptr = vArray.at(3).get();
    hArray.at(3)->mVertex_Ptr = vArray.at(0).get();

    hArray.at(4)->mVertex_Ptr = vArray.at(5).get();
    hArray.at(5)->mVertex_Ptr = vArray.at(1).get();
    hArray.at(6)->mVertex_Ptr = vArray.at(0).get();
    hArray.at(7)->mVertex_Ptr = vArray.at(4).get();

    hArray.at(8)->mVertex_Ptr = vArray.at(6).get();
    hArray.at(9)->mVertex_Ptr = vArray.at(5).get();
    hArray.at(10)->mVertex_Ptr = vArray.at(4).get();
    hArray.at(11)->mVertex_Ptr = vArray.at(7).get();

    hArray.at(12)->mVertex_Ptr = vArray.at(2).get();
    hArray.at(13)->mVertex_Ptr = vArray.at(6).get();
    hArray.at(14)->mVertex_Ptr = vArray.at(7).get();
    hArray.at(15)->mVertex_Ptr = vArray.at(3).get();

    hArray.at(16)->mVertex_Ptr = vArray.at(3).get();
    hArray.at(17)->mVertex_Ptr = vArray.at(7).get();
    hArray.at(18)->mVertex_Ptr = vArray.at(4).get();
    hArray.at(19)->mVertex_Ptr = vArray.at(0).get();

    hArray.at(20)->mVertex_Ptr = vArray.at(1).get();
    hArray.at(21)->mVertex_Ptr = vArray.at(5).get();
    hArray.at(22)->mVertex_Ptr = vArray.at(6).get();
    hArray.at(23)->mVertex_Ptr = vArray.at(2).get();

    // Push vertices, half-edges and faces unique pointers to their own vectors
    for(int i = 0; i < 8; i++)
    {
        mVertexList.push_back(std::move(vArray.at(i)));
    }
    for(int i = 0; i < 24; i++)
    {
        mHalfEdgeList.push_back(std::move(hArray.at(i)));
    }
    for(int i = 0; i < 6; i++)
    {
        mFaceList.push_back(std::move(fArray.at(i)));
    }
}

void IndexInput(std::vector<GLuint>& idxVec, unsigned int firstVertIdx, unsigned int vertNum)
{
    for(unsigned int i = 0; i < (vertNum - 2); i++)
    {
        idxVec.push_back(firstVertIdx);
        idxVec.push_back(firstVertIdx + i + 1);
        idxVec.push_back(firstVertIdx + i + 2);
    }
}

// Calculate the normal at the vertex pointed by this halfedge
glm::vec3 CalculateVertexNorm(HalfEdge* hePtr)
{
    // current point
    glm::vec3 v1 = glm::vec3(
                hePtr->mVertex_Ptr->mPos.x,
                hePtr->mVertex_Ptr->mPos.y,
                hePtr->mVertex_Ptr->mPos.z);
    // back point
    glm::vec3 v2 = glm::vec3(
                hePtr->mSymEdge_Ptr->mVertex_Ptr->mPos.x,
                hePtr->mSymEdge_Ptr->mVertex_Ptr->mPos.y,
                hePtr->mSymEdge_Ptr->mVertex_Ptr->mPos.z);
    // front point
    glm::vec3 v3 = glm::vec3(
                hePtr->mNextEdge_Ptr->mVertex_Ptr->mPos.x,
                hePtr->mNextEdge_Ptr->mVertex_Ptr->mPos.y,
                hePtr->mNextEdge_Ptr->mVertex_Ptr->mPos.z);

    glm::vec3 vec1 = v3 - v1;
    glm::vec3 vec2 = v2 - v1;
    return glm::normalize(glm::cross(vec1, vec2));
}


OpenGLDS::OpenGLDS(Mesh &iMesh)
{
    // Construct arrPos, arrNor, arrCol according to faces sequence
    // Construct indexArray for each face
    for(std::vector<std::unique_ptr<Face>>::iterator fVecPtr = iMesh.mFaceList.begin();
        fVecPtr != iMesh.mFaceList.end();
        fVecPtr++)
    {
        // auxiliary variables
        unsigned int arrInitSize = arrPos.size();
        unsigned int counter = 0;

        // Push every vertices' pos, col in this ring into arrPos
        Face& temp = **fVecPtr;
        HalfEdge* ePtr = temp.mEdge_Ptr;
        do{
            arrPos.push_back(glm::vec4(ePtr->mVertex_Ptr->mPos.x,
                                       ePtr->mVertex_Ptr->mPos.y,
                                       ePtr->mVertex_Ptr->mPos.z,
                                       1.f));

            arrCol.push_back(temp.mColor);

            for(unsigned int influFlag = 0; influFlag < ePtr->mVertex_Ptr->mJointInfluVec.size(); influFlag++)
            {
                JointInfluence jI = ePtr->mVertex_Ptr->mJointInfluVec.at(influFlag);
                arrInfluJointsIDs.push_back(jI.tarJoint->mID);
                arrWeights.push_back(jI.influenceNum);
            }

            ePtr = ePtr->mNextEdge_Ptr;
            counter++;
        }while(ePtr != temp.mEdge_Ptr);



        // Calculate this face's normal
        Face* fPtr = fVecPtr->get();
        glm::vec4 nor = CalculateFaceNorm(fPtr);

        /*for(unsigned int i = 0; i < counter; i++)
        {
            arrInfluJointsIDs.push_back();
            arrInfluJointsIDs.push_back();
        }

        for(unsigned int i = 0; i < counter; i++)
        {
            arrWeights.push_back();
            arrWeights.push_back();
        }
        */

        // Push the normal into arrNor
        // for 'counter' times
        // there are counter number points in a ring
        for(unsigned int i = 0; i < counter; i++)
        {
            arrNor.push_back(nor);
        }


        // Construct the index array for this face
        // triangluate this face
        IndexInput(arrIdx, arrInitSize, counter);
    }
}

OpenGLDS::OpenGLDS(glm::vec4 iVertex)
{
    arrPos.push_back(iVertex);
    arrCol.push_back(glm::vec4(1, 1, 1, 1));
    arrNor.push_back(glm::vec4(1, 0, 0, 1));
    arrIdx.push_back(0);
}

OpenGLDS::OpenGLDS(glm::vec4 vHead, glm::vec4 vTail)
{
    arrPos.push_back(vHead);
    arrPos.push_back(vTail);
    arrCol.push_back(glm::vec4(1, 1, 0, 1)); // head -- yellow
    arrCol.push_back(glm::vec4(1, 0, 0, 1)); // tail -- red
    arrNor.push_back(glm::vec4(1, 0, 0, 1));
    arrNor.push_back(glm::vec4(1, 0, 0, 1));
    arrIdx.push_back(0);
    arrIdx.push_back(1);
}

OpenGLDS::OpenGLDS(std::vector<glm::vec4>& ringVerticesList, glm::vec4 iColor)
{
    int vSize = ringVerticesList.size();
    // Construct the Idx arr
    for(int i = 0; i < vSize; i++)
    {
        arrIdx.push_back(i);
        if(i + 1 >= vSize)
        {
            arrIdx.push_back(0);
        }
        else
        {
            arrIdx.push_back(i + 1);
        }
    }
    // Construct the pos, nor, col arr
    for(std::vector<glm::vec4>::iterator vPtr = ringVerticesList.begin(); vPtr != ringVerticesList.end(); vPtr++)
    {
        arrPos.push_back(*vPtr);
        arrCol.push_back(iColor);
        arrNor.push_back(glm::vec4(1, 0, 0, 1));
    }
}


void OpenGLDS::ConstructAFrameWireSphere(glm::mat4 transformMat, FrameWireSphere &iSphere, bool selected)
{
    // Start idx is the vertex idx that we are going to use
    std::vector<glm::vec4>& verticalZVec = iSphere.verticalZVec;
    std::vector<glm::vec4>& verticalXVec = iSphere.verticalXVec;
    std::vector<glm::vec4>& horizontalVec = iSphere.horizontalVec;
    GLuint idx = arrPos.size();
    // verticalZVec
    GLuint startIdx = idx;
    GLuint endIdx = idx + GLuint(verticalZVec.size()) - 1;
    for(std::vector<glm::vec4>::iterator i = verticalZVec.begin(); i != verticalZVec.end(); i++, idx++)
    {
        arrPos.push_back(transformMat * (*i));
        arrNor.push_back(glm::vec4(1, 0, 0, 1));
        if(selected)
        {
            arrCol.push_back(glm::vec4(1, 1, 1, 1));
        }
        else
        {
            arrCol.push_back(glm::vec4(1, 0, 0, 1));
        }

        arrIdx.push_back(idx);
        if(idx == endIdx)
        {
            arrIdx.push_back(startIdx);
        }
        else
        {
            arrIdx.push_back(idx + 1);
        }
    }

    // verticalXVec
    startIdx = idx;
    endIdx = idx + GLuint(verticalXVec.size()) - 1;
    for(std::vector<glm::vec4>::iterator i = verticalXVec.begin(); i != verticalXVec.end(); i++, idx++)
    {
        arrPos.push_back(transformMat * (*i));
        arrNor.push_back(glm::vec4(1, 0, 0, 1));
        if(selected)
        {
            arrCol.push_back(glm::vec4(1, 1, 1, 1));
        }
        else
        {
            arrCol.push_back(glm::vec4(0, 1, 0, 1));
        }
        arrIdx.push_back(idx);
        if(idx == endIdx)
        {
            arrIdx.push_back(startIdx);
        }
        else
        {
            arrIdx.push_back(idx + 1);
        }
    }

    // horizontalVec
    startIdx = idx;
    endIdx = idx + GLuint(horizontalVec.size()) - 1;
    for(std::vector<glm::vec4>::iterator i = horizontalVec.begin(); i != horizontalVec.end(); i++, idx++)
    {
        arrPos.push_back(transformMat * (*i));
        arrNor.push_back(glm::vec4(1, 0, 0, 1));
        if(selected)
        {
            arrCol.push_back(glm::vec4(1, 1, 1, 1));
        }
        else
        {
            arrCol.push_back(glm::vec4(0, 0, 1, 1));
        }
        arrIdx.push_back(idx);
        if(idx == endIdx)
        {
            arrIdx.push_back(startIdx);
        }
        else
        {
            arrIdx.push_back(idx + 1);
        }
    }
}

void OpenGLDS::ConstructSphereConnection(Joint *parent)
{
    for(std::vector<Joint*>::iterator i = parent->mChildJointsVec.begin(); i != parent->mChildJointsVec.end(); i++)
    {
        GLuint startIdx = arrPos.size();
        Joint* child = *i;
        arrPos.push_back(parent->getOverallTransformation() * glm::vec4(0, 0, 0, 1));
        arrPos.push_back(child->getOverallTransformation() * glm::vec4(0, 0, 0, 1));
        arrCol.push_back(glm::vec4(1, 0, 0, 1)); // head -- red
        arrCol.push_back(glm::vec4(1, 1, 0, 1)); // tail -- yellow
        arrNor.push_back(glm::vec4(1, 0, 0, 1));
        arrNor.push_back(glm::vec4(1, 0, 0, 1));
        arrIdx.push_back(startIdx);
        arrIdx.push_back(startIdx + 1);
        ConstructSphereConnection(child);
    }

}

OpenGLDS::OpenGLDS(Skeleton& iSkeleton)
{
    // Draw Framewire Sphere
    FrameWireSphere mSphere;
    for(unsigned int i = 0; i < iSkeleton.mJointsList.size(); i++)
    {
        Joint* currJ = iSkeleton.mJointsList.at(i).get();
        // World Space Position
        ConstructAFrameWireSphere(currJ->getOverallTransformation(), mSphere, currJ->mSelected);
    }
    ConstructSphereConnection(iSkeleton.mRoot);
}

///***************///
// Subdivide
///**************///
// Add Centroid
void Mesh::AddACentroidToAFace(std::vector<Vertex*>& centroidsVec, Face* iFace){
    // Add every vertices in a Face together
    // Count the number of vertices in a Face
    unsigned int vNum = 0;
    glm::vec4 totalPos = glm::vec4(0, 0, 0, 0);
    HalfEdge* hePtr = iFace->mEdge_Ptr;
    do{
        totalPos += hePtr->mVertex_Ptr->mPos;
        hePtr = hePtr->mNextEdge_Ptr;
        vNum++;
    }while(hePtr != iFace->mEdge_Ptr);

    // Get the average value of these vertices' pos
    glm::vec4 avgPos = glm::vec4(totalPos.x / vNum, totalPos.y / vNum, totalPos.z / vNum, totalPos.w / vNum);
    // Construct a Vertex(Centroid) that has this pos
    std::unique_ptr<Vertex> centroidUPtr = std::make_unique<Vertex>(Vertex());
    centroidUPtr->mPos = avgPos;
    // Push it into the vec
    centroidsVec.push_back(centroidUPtr.get());
    this->mVertexList.push_back(std::move(centroidUPtr));
}

void Mesh::AddCentroids(std::vector<Vertex*>& centroidsVec){
    for(std::vector<std::unique_ptr<Face>>::iterator fPtr = this->mFaceList.begin();
        fPtr != this->mFaceList.end(); fPtr++)
    {
        AddACentroidToAFace(centroidsVec, fPtr->get());
    }
}

///*************///
// Add midpoints
void Mesh::SplitHE_HW7(glm::vec4 iPos, HalfEdge* he, std::vector<Vertex*>& midPointsVec)
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
    v3->mPos = iPos;
    midPointsVec.push_back(v3);

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
    this->mVertexList.push_back(std::move(uV3));
    this->mHalfEdgeList.push_back(std::move(uHe1b));
    this->mHalfEdgeList.push_back(std::move(uHe2b));
}

bool ShouldSplit(std::vector<HalfEdge*>& splitedList, HalfEdge* heCheckedPtr)
{
    for(std::vector<HalfEdge*>::iterator heSplittedPtr = splitedList.begin();
        heSplittedPtr != splitedList.end(); heSplittedPtr++)
    {
        if(*heSplittedPtr == heCheckedPtr)
        {
            return false;
        }
    }
    return true;
}

glm::vec4 MidpointPos(std::vector<glm::vec4>& oriPos, std::vector<glm::vec4>& centroPos)
{
    unsigned int posNum = 0;
    glm::vec4 totalVal = glm::vec4(0, 0, 0, 0);
    for(std::vector<glm::vec4>::iterator oriPtr = oriPos.begin();
        oriPtr != oriPos.end(); oriPtr++)
    {
        totalVal += *oriPtr;
        posNum++;
    }

    for(std::vector<glm::vec4>::iterator cenPtr = centroPos.begin();
        cenPtr != centroPos.end(); cenPtr++)
    {
        totalVal += *cenPtr;
        posNum++;
    }

    return glm::vec4(totalVal.x / posNum, totalVal.y / posNum, totalVal.z / posNum, totalVal.w / posNum);
}

unsigned int GetCentroOffset(Face* fPtr)
{
    unsigned int id = fPtr->text().toInt();
    return id;
}

void ConstructCenVec(HalfEdge* hePtr, std::vector<Vertex*>& mCentroidsVec, std::vector<glm::vec4>& centroPos)
{
    Face* fAPtr = hePtr->mFace_Ptr;
    Face* fBPtr = hePtr->mSymEdge_Ptr->mFace_Ptr;
    if(fBPtr == nullptr)
    {
        unsigned int offset = GetCentroOffset(fAPtr);
        centroPos.push_back(mCentroidsVec.at(offset)->mPos);
    }
    else
    {
        unsigned int off1 = GetCentroOffset(fAPtr);
        unsigned int off2 = GetCentroOffset(fBPtr);
        centroPos.push_back(mCentroidsVec.at(off1)->mPos);
        centroPos.push_back(mCentroidsVec.at(off2)->mPos);
    }
}

void ConstructOriVec(HalfEdge* hePtr, std::vector<glm::vec4>& oriPosVec)
{
    oriPosVec.push_back(hePtr->mVertex_Ptr->mPos);
    oriPosVec.push_back(hePtr->mSymEdge_Ptr->mVertex_Ptr->mPos);
}

void Mesh::AddMidPoints(std::vector<Vertex*>& mCentroidsVec, std::vector<Vertex*>& midPointsVec)
{
    std::vector<HalfEdge*> oldHEList;
    std::vector<HalfEdge*> splitedList;
    // Construct the old Half Edges list
    for(std::vector<std::unique_ptr<HalfEdge>>::iterator uHePtr = this->mHalfEdgeList.begin();
        uHePtr != this->mHalfEdgeList.end(); uHePtr++)
    {
        oldHEList.push_back(uHePtr->get());
    }
    // Check every old HE and split the HE that should be splitted
    for(std::vector<HalfEdge*>::iterator hePtr = oldHEList.begin();
        hePtr != oldHEList.end(); hePtr++)
    {
        if(ShouldSplit(splitedList, *hePtr))
        {
            // Determine how to calculate the pos of this midpoint
            // And get the value of this pos
            // push hePtr and it's sym ptr into splitedList
            splitedList.push_back(*hePtr);
            HalfEdge* temp = *hePtr;
            splitedList.push_back(temp->mSymEdge_Ptr);

            std::vector<glm::vec4> oriPosVec;
            std::vector<glm::vec4> centroPosVec;
            ConstructCenVec(*hePtr, mCentroidsVec, centroPosVec);
            ConstructOriVec(*hePtr, oriPosVec);
            glm::vec4 midpos = MidpointPos(oriPosVec, centroPosVec);
            // split the edge related to hePtr and add this midpoint into mesh
            // Note: it would change the sym pointers
            SplitHE_HW7(midpos, *hePtr, midPointsVec);
        }
    }
}

///*********************///
// For smooth step below
///General Support///

///*********************///
void FindAdjMidpoints(const Vertex* tarVet, std::vector<Vertex*>& adjMidpointsVec, const std::vector<Face*>& adjFacesVec)
{
    for(unsigned int i = 0; i < adjFacesVec.size(); i++)
    {
        Face* f = adjFacesVec.at(i);
        HalfEdge* currHE = FindHEinFacePointsToVert(tarVet, f);
        HalfEdge* preHE = FindPreHE(currHE);
        adjMidpointsVec.push_back(preHE->mVertex_Ptr);
    }
}

void FindAdjCentro(const Vertex* tarVet, const std::vector<Vertex*>& CentrodsVec, std::vector<Vertex*>& adjCentroVec, std::vector<Face*>& adjFacesVec)
{
    FindAdjFaces(tarVet, adjFacesVec);
    for(unsigned int i = 0; i < adjFacesVec.size(); i++)
    {
        unsigned int FaceID = GetCentroOffset(adjFacesVec.at(i));
        adjCentroVec.push_back(CentrodsVec.at(FaceID));
    }
}

void Mesh::MoveOriVertices(const std::vector<Vertex*> centroidsVec,
                           const std::vector<Vertex*> midPointsVec,
                           std::vector<Vertex*> oriVertVec)
{
    for(unsigned int i = 0; i < oriVertVec.size(); i++)
    {
        std::vector<Vertex*> adjMidpointsVec;
        std::vector<Vertex*> adjCentroVec;
        std::vector<Face*> adjFacesVec;

        Vertex* tarVet = oriVertVec.at(i);
        // Construct the adj Centro vec
        FindAdjCentro(tarVet, centroidsVec, adjCentroVec, adjFacesVec);
        // Construct the adj midpoints vec
        FindAdjMidpoints(tarVet, adjMidpointsVec, adjFacesVec);
        unsigned int adjMidNum = adjMidpointsVec.size();
        glm::vec4 sumE = VertPosSum(adjMidpointsVec);
        glm::vec4 sumF = VertPosSum(adjCentroVec);

        glm::vec4 smoothedPos = glm::vec4(
                    (adjMidNum - 2) * tarVet->mPos.x / adjMidNum +
                     sumE.x / (adjMidNum * adjMidNum) +
                     sumF.x / (adjMidNum * adjMidNum),
                    (adjMidNum - 2) * tarVet->mPos.y / adjMidNum +
                     sumE.y / (adjMidNum * adjMidNum) +
                     sumF.y / (adjMidNum * adjMidNum),
                    (adjMidNum - 2) * tarVet->mPos.z / adjMidNum +
                     sumE.z / (adjMidNum * adjMidNum) +
                     sumF.z / (adjMidNum * adjMidNum),
                    (adjMidNum - 2) * tarVet->mPos.w / adjMidNum +
                     sumE.w / (adjMidNum * adjMidNum) +
                     sumF.w / (adjMidNum * adjMidNum));
        tarVet->mPos = smoothedPos;
    }
}

///*********************///
// Quadrangulate
///General Support///
void ConstructTempHE(std::vector<HalfEdge*>& tempHE, const std::vector<HalfEdge*>& midHEVec)
{
    for(unsigned int i = 0; i < midHEVec.size(); i++)
    {
        HalfEdge* temp = midHEVec.at(i);
        tempHE.push_back(temp->mNextEdge_Ptr);
    }
}

int GetOriVertNum(Face* oriF)
{
    int i = 0;
    HalfEdge* hePtr = oriF->mEdge_Ptr;
    do{
        i++;
        hePtr = hePtr->mNextEdge_Ptr;
    }while(hePtr != oriF->mEdge_Ptr);
    return (i / 2);
}



///*********************///
void Mesh::ConstructAndCreateFaces(Face* oriF, std::vector<Face*>& faceVec)
{
    int oriNum = GetOriVertNum(oriF);
    int newFaceNum = oriNum - 1;
    faceVec.push_back(oriF);
    for(unsigned int i = 0; i < newFaceNum; i++)
    {
        std::unique_ptr<Face> uPtrF = std::make_unique<Face>(Face());
        uPtrF->mColor = oriF->mColor;
        faceVec.push_back(uPtrF.get());
        this->mFaceList.push_back(std::move(uPtrF));
    }
}

void Mesh::ConstructAndCreateHEInSubFace(Face *assignedFace,
                                   HalfEdge *midHE,
                                   HalfEdge *preTempHE,
                                   Vertex* centroid)
{
    std::unique_ptr<HalfEdge> uHEA = std::make_unique<HalfEdge>(HalfEdge());
    std::unique_ptr<HalfEdge> uHEB = std::make_unique<HalfEdge>(HalfEdge());
    HalfEdge* HEA = uHEA.get();
    HalfEdge* HEB = uHEB.get();
    Vertex* vM1 = midHE->mVertex_Ptr;
    Vertex* vM2 = preTempHE->mSymEdge_Ptr->mVertex_Ptr;

    midHE->mNextEdge_Ptr = HEA;
    HEA->mNextEdge_Ptr = HEB;
    HEB->mNextEdge_Ptr = preTempHE;

    HEA->mVertex_Ptr =centroid;
    HEB->mVertex_Ptr = vM2;

    centroid->mEdge_Ptr = HEA;

    HEA->mFace_Ptr = assignedFace;
    HEB->mFace_Ptr = assignedFace;
    midHE->mFace_Ptr = assignedFace;
    preTempHE->mFace_Ptr = assignedFace;

    assignedFace->mEdge_Ptr = midHE;

    this->mHalfEdgeList.push_back(std::move(uHEA));
    this->mHalfEdgeList.push_back(std::move(uHEB));
}

void SetSymmetry(HalfEdge* tempHE, HalfEdge* midHE)
{
    HalfEdge* tempPre = FindPreHE(tempHE);
    HalfEdge* midNext = midHE->mNextEdge_Ptr;
    midNext->mSymEdge_Ptr = tempPre;
    tempPre->mSymEdge_Ptr = midNext;
}

void Mesh::QuadrangleFace(Face *f, Vertex *centroid, std::vector<HalfEdge *> midHEVec)
{
    // Create Faces and HEs, Set everything except for symmetry
    std::vector<HalfEdge*> tempHEVec;
    std::vector<Face*> faceVec;
    ConstructTempHE(tempHEVec, midHEVec);
    ConstructAndCreateFaces(f, faceVec);
    for(unsigned int i = 0; i < midHEVec.size(); i++)
    {
        HalfEdge* preTempHE;
        if(i == 0)
        {
            preTempHE = tempHEVec.back();
        }
        else
        {
            preTempHE = tempHEVec.at(i - 1);
        }
        ConstructAndCreateHEInSubFace(faceVec.at(i), midHEVec.at(i), preTempHE, centroid);
    }
    // Set symmetry
    for(unsigned int i = 0; i < midHEVec.size(); i++)
    {
        SetSymmetry(tempHEVec.at(i), midHEVec.at(i));
    }
}


void ConstructMidHEVec(const Face* f,
                       const std::vector<Vertex *> midPointsVec,
                       std::vector<HalfEdge*>& midHEVec)
{
    HalfEdge* hePtr = f->mEdge_Ptr;
    do{
        // if this HE points to a vert in the midPoints vector
        if(VertInVec(hePtr->mVertex_Ptr, midPointsVec))
        {
            midHEVec.push_back(hePtr);
        }
        hePtr = hePtr->mNextEdge_Ptr;
    }while(hePtr != f->mEdge_Ptr);
}

void Mesh::ConstructOldFaceList(std::vector<Face*>& oldFaceList)
{
    for(unsigned int i = 0; i < this->mFaceList.size(); i++)
    {
        oldFaceList.push_back(this->mFaceList.at(i).get());
    }
}

void Mesh::QuadrangleMesh(const std::vector<Vertex *> centroidsVec,
                          const std::vector<Vertex *> midPointsVec,
                          const std::vector<Vertex *> oriVertVec)
{
    std::vector<Face*> oldFaceList;
    ConstructOldFaceList(oldFaceList);
    for(unsigned int i = 0; i < oldFaceList.size(); i++)
    {
        Face* f = oldFaceList.at(i);
        std::vector<HalfEdge*> midHEVec;
        ConstructMidHEVec(f, midPointsVec, midHEVec);
        unsigned int faceID = GetCentroOffset(f);
        Vertex* centroid = centroidsVec.at(faceID);
        QuadrangleFace(f, centroid, midHEVec);
    }
}



///*********************///
/// General Support
void Mesh::ConstructOriVertVec(std::vector<Vertex*>& oriVertVec)
{
    for(unsigned int i = 0; i < this->mVertexList.size(); i++)
    {
        Vertex* temp = mVertexList.at(i).get();
        oriVertVec.push_back(temp);
    }
}
///*********************///
void Mesh::Catmull_Clark_Subdivide()
{
    std::vector<Vertex*> centroidsVec;
    std::vector<Vertex*> midPointsVec;
    std::vector<Vertex*> oriVertVec;
    ConstructOriVertVec(oriVertVec);
    AddCentroids(centroidsVec);
    AddMidPoints(centroidsVec, midPointsVec);
    MoveOriVertices(centroidsVec, midPointsVec, oriVertVec);
    QuadrangleMesh(centroidsVec, midPointsVec, oriVertVec);
}

///*********************///
// Edge Extrustion
///***General Support***///

///*********************///
void SetExtrudeSym(HalfEdge* oldHE1, HalfEdge* oldHE2)
{
    HalfEdge* oldHE1Sym = oldHE1->mSymEdge_Ptr;
    HalfEdge* oldHE2Sym = oldHE2->mSymEdge_Ptr;
    HalfEdge* oHE1SymPre = FindPreHE(oldHE1Sym);
    HalfEdge* oHE2SymNext = oldHE2Sym->mNextEdge_Ptr;

    oHE1SymPre->mSymEdge_Ptr = oHE2SymNext;
    oHE2SymNext->mSymEdge_Ptr = oHE1SymPre;
}

void Mesh::ExtrudeAnEdge(HalfEdge* HE1)
{
    ///***********///
    // Init required info
    std::unique_ptr<HalfEdge> uHE1B = std::make_unique<HalfEdge>(HalfEdge());
    std::unique_ptr<HalfEdge> uHE2B = std::make_unique<HalfEdge>(HalfEdge());
    std::unique_ptr<HalfEdge> uHE3 = std::make_unique<HalfEdge>(HalfEdge());
    std::unique_ptr<HalfEdge> uHE4 = std::make_unique<HalfEdge>(HalfEdge());
    HalfEdge* HE1B = uHE1B.get();
    HalfEdge* HE2B = uHE2B.get();
    HalfEdge* HE3 = uHE3.get();
    HalfEdge* HE4 = uHE4.get();
    HalfEdge* HE2 = HE1->mSymEdge_Ptr;
    HalfEdge* preHE1 = FindPreHE(HE1);
    HalfEdge* preHE2 = FindPreHE(HE2);

    Vertex* v1 = preHE2->mVertex_Ptr;
    Vertex* v2 = HE2->mVertex_Ptr;
    Vertex* v3 = HE1->mVertex_Ptr;
    Vertex* v4 = preHE1->mVertex_Ptr;

    std::unique_ptr<Face> uNewF = std::make_unique<Face>(Face());
    Face* newF = uNewF.get();
    ///***********///

    ///***********///
    // link every conponents
    // face
    newF->mEdge_Ptr = HE1B;
    newF->mColor = HE1->mFace_Ptr->mColor;

    // halfedge
    HE1->mVertex_Ptr = v3;
    preHE1->mVertex_Ptr = v4;
    HE1B->mVertex_Ptr = v4;
    HE2B->mVertex_Ptr = v1;
    HE3->mVertex_Ptr = v3;
    HE4->mVertex_Ptr = v2;

    HE1->mSymEdge_Ptr = HE1B;
    HE2->mSymEdge_Ptr = HE2B;
    HE1B->mSymEdge_Ptr = HE1;
    HE2B->mSymEdge_Ptr = HE2;

    HE1B->mNextEdge_Ptr = HE4;
    HE4->mNextEdge_Ptr = HE2B;
    HE2B->mNextEdge_Ptr = HE3;
    HE3->mNextEdge_Ptr = HE1B;

    HE1B->mFace_Ptr = newF;
    HE2B->mFace_Ptr = newF;
    HE3->mFace_Ptr = newF;
    HE4->mFace_Ptr = newF;
    ///***********///

    ///***********///
    // push uPtrs into Mesh
    this->mFaceList.push_back(std::move(uNewF));
    this->mHalfEdgeList.push_back(std::move(uHE1B));
    this->mHalfEdgeList.push_back(std::move(uHE2B));
    this->mHalfEdgeList.push_back(std::move(uHE3));
    this->mHalfEdgeList.push_back(std::move(uHE4));
    ///***********///
}

void Mesh::UpraiseTheOldPlane(Face* f, float dis)
{
    // Upraise The Old Plane using the new Vertices
    // Leave the old vertices for previous faces
    // Create new Vertices
    glm::vec4 fNormal = CalculateFaceNorm(f);
    HalfEdge* hePtr = f->mEdge_Ptr;
    do{
        // Calculate the pos value
        glm::vec4 translation = fNormal * dis;
        std::unique_ptr<Vertex> uVert = std::make_unique<Vertex>(Vertex());
        Vertex* v = uVert.get();
        v->mPos = hePtr->mVertex_Ptr->mPos + translation;
        hePtr->mVertex_Ptr = v;
        v->mEdge_Ptr = hePtr;
        this->mVertexList.push_back(std::move(uVert));
        hePtr = hePtr->mNextEdge_Ptr;
    }while(hePtr != f->mEdge_Ptr);
}

void Mesh::ExtrudeFace(Face* f, float dis)
{
    UpraiseTheOldPlane(f, dis);
    HalfEdge* hePtr = f->mEdge_Ptr;
    do{
        ExtrudeAnEdge(hePtr);
        hePtr = hePtr->mNextEdge_Ptr;
    }while(hePtr != f->mEdge_Ptr);

    do{
        SetExtrudeSym(hePtr, hePtr->mNextEdge_Ptr);
        hePtr = hePtr->mNextEdge_Ptr;
    }while(hePtr != f->mEdge_Ptr);
}

///*********///
// Convert from Polygon to Mesh
///*********///
//



// We are going to Construct the whole vertics and meshTriIndex that we need.
// The meshTriIndex should contain the index of this->mVerticeList, and we are going to
// construct it.
void Mesh::ConstructAndCreateMeshVecs(MPolygon& iPolygon,
                                         std::vector<Triangle>& meshTriIndex)
{
    // Visit every PVertex in the Polygon
    for(unsigned int i = 0; i < iPolygon.m_verts.size(); i++)
    {
        // Get the current PVertex
        PVertex& currPVert = iPolygon.m_verts.at(i);
        // Check whether this PVert has already been converted to meshVert
        // or whether this PVert has same meshVert with other PVert
        int mIdx = FindMeshVertIdx(currPVert);
        if(mIdx == -1)
        {
            // mIdx == -1 -> we cannot find a corresponding one, so we need to create one.
            std::unique_ptr<Vertex> uV = std::make_unique<Vertex>(Vertex());
            Vertex* v = uV.get();
            v->mPos = currPVert.m_pos;
            this->mVertexList.push_back(std::move(uV));
        }
    }

    // Construct mesh tri index corresponding to iPolygon's tri
    // Visit every tri in the Polygon
    for(unsigned int i = 0; i < iPolygon.m_tris.size(); i++)
    {
        Triangle& currTri = iPolygon.m_tris.at(i);
        Triangle meshTri;
        for(unsigned int i = 0; i < 3; i++)
        {
            PVertex& temp = iPolygon.m_verts.at(currTri.m_indices[i]);
            int mIdx = FindMeshVertIdx(temp);
            meshTri.m_indices[i] = mIdx;
        }
        meshTriIndex.push_back(meshTri);
    }
}

// The iTri should contain the index of meshVertVec
void Mesh::ConstructAndCreateAMeshLoopFace(Triangle& iTri)
{
    // Init all needed uPtr
    std::unique_ptr<Face> uf = std::make_unique<Face>(Face());
    std::unique_ptr<HalfEdge> uHE1 = std::make_unique<HalfEdge>(HalfEdge());
    std::unique_ptr<HalfEdge> uHE2 = std::make_unique<HalfEdge>(HalfEdge());
    std::unique_ptr<HalfEdge> uHE3 = std::make_unique<HalfEdge>(HalfEdge());
    Face* f = uf.get();
    HalfEdge* HE1 = uHE1.get();
    HalfEdge* HE2 = uHE2.get();
    HalfEdge* HE3 = uHE3.get();
    Vertex* v1 = this->mVertexList.at(iTri.m_indices[0]).get();
    Vertex* v2 = this->mVertexList.at(iTri.m_indices[1]).get();
    Vertex* v3 = this->mVertexList.at(iTri.m_indices[2]).get();

    // Set links
    f->mEdge_Ptr = HE1;
    float r1 = rand() % 100;
    float r2 = rand() % 100;
    float r3 = rand() % 100;
    r1 /= 100;
    r2 /= 100;
    r3 /= 100;
    f->mColor = glm::vec4(r1, r2, r3, 0);

    HE1->mFace_Ptr = f;
    HE2->mFace_Ptr = f;
    HE3->mFace_Ptr = f;

    HE1->mNextEdge_Ptr = HE2;
    HE2->mNextEdge_Ptr = HE3;
    HE3->mNextEdge_Ptr = HE1;

    HE1->mVertex_Ptr = v1;
    HE2->mVertex_Ptr = v2;
    HE3->mVertex_Ptr = v3;

    v1->mEdge_Ptr = HE1;
    v2->mEdge_Ptr = HE2;
    v3->mEdge_Ptr = HE3;

    // Push every uPtr into Mesh
    this->mFaceList.push_back(std::move(uf));
    this->mHalfEdgeList.push_back(std::move(uHE1));
    this->mHalfEdgeList.push_back(std::move(uHE2));
    this->mHalfEdgeList.push_back(std::move(uHE3));
}

void Mesh::ConstructAndCreateMeshLoopFaces(std::vector<Triangle>& meshTriIndex)
{
    for(unsigned int i = 0; i < meshTriIndex.size(); i++)
    {
        ConstructAndCreateAMeshLoopFace(meshTriIndex.at(i));
    }
}

void Mesh::SetHEConnection(HalfEdge* hePtr, int i)
{
    HalfEdge* preHE = FindPreHE(hePtr);
    Vertex* v0 = hePtr->mVertex_Ptr;
    Vertex* v1 = preHE->mVertex_Ptr;
    for(int j = i + 1; j < this->mFaceList.size(); j++)
    {
        Face* currFace = this->mFaceList.at(j).get();
        if(VertexInFace(v0, currFace) && VertexInFace(v1, currFace))
        {
            HalfEdge* symHE = FindHEinFacePointsToVert(v1, currFace);
            hePtr->mSymEdge_Ptr = symHE;
            symHE->mSymEdge_Ptr = hePtr;
        }
    }
}

void Mesh::SetSymmetryInConversion()
{
    // Visit every face
    for(unsigned int i = 0; i < this->mFaceList.size(); i++)
    {
        Face* centerFace = this->mFaceList.at(i).get();
        // For every HE in the face
        HalfEdge* hePtr = centerFace->mEdge_Ptr;
        do{
            // Set HE Connection by checking every faces after i
            SetHEConnection(hePtr, i);
            hePtr = hePtr->mNextEdge_Ptr;
        }while(hePtr != centerFace->mEdge_Ptr);
    }
}

void Mesh::PolygonToMesh(MPolygon &iPolygon)
{
    // The name of it is a little bit awkward
    // It should be meshTriVec
    // But it is too late to change it...
    std::vector<Triangle> meshTriIndex;
    ConstructAndCreateMeshVecs(iPolygon, meshTriIndex);
    ConstructAndCreateMeshLoopFaces(meshTriIndex);
    SetSymmetryInConversion();
}

void Mesh::Clear()
{
    this->mFaceList.clear();
    this->mVertexList.clear();
    this->mHalfEdgeList.clear();
    Face::mID = 0;
    Vertex::mID = 0;
    HalfEdge::mID = 0;
}

Vertex* Mesh::SphereTest(glm::vec3 eyePos, glm::vec4 modelSpaceRay, float& resT)
{
    float nearestT = -1;
    Vertex* nearestVert = nullptr;
    float distance = 20;
    for(unsigned int i = 0; i < this->mVertexList.size(); i++)
    {
        Vertex* temp = this->mVertexList.at(i).get();

        glm::vec3 rd = eyePos + glm::vec3(modelSpaceRay.x * distance,
                                          modelSpaceRay.y * distance,
                                          modelSpaceRay.z * distance);

        float t = temp->SphereIntersection(eyePos, rd);
        if(t > 0 && t < nearestT)
        {
            nearestT = t;
            nearestVert = temp;
        }
        else if(nearestT < 0 && t > 0)
        {
            nearestT = t;
            nearestVert = temp;
        }
    }
    resT = nearestT;
    return nearestVert;
}

Face* Mesh::FaceTest(glm::vec3 eyePos, glm::vec4 modelSpaceRay, float& resT)
{
    float nearestT = -1;
    Face* nearestVert = nullptr;
    float distance = 20;
    for(unsigned int i = 0; i < this->mFaceList.size(); i++)
    {
        Face* temp = this->mFaceList.at(i).get();

        glm::vec3 rd = eyePos + glm::vec3(modelSpaceRay.x, modelSpaceRay.y, modelSpaceRay.z) * distance;

        std::cout << "face:" << temp->text().toStdString() << std::endl;
        float t = temp->PlaneIntersection(eyePos, rd);
        if(t > 0 && t < nearestT)
        {
            nearestT = t;
            nearestVert = temp;
        }
        else if(nearestT < 0 && t > 0)
        {
            nearestT = t;
            nearestVert = temp;
        }
    }
    resT = nearestT;
    return nearestVert;
}



