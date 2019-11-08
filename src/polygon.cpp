#include "Polygon.h"
#include <glm/gtx/transform.hpp>

void MPolygon::Triangulate()
{
    //TODO: Populate list of triangles
    // i equals to the index of every triangle.
    for(int i = 0; i < m_verts.size() - 2; i++)
    {
        Triangle temp;
        temp.m_indices[0] = 0;
        temp.m_indices[1] = i + 1;
        temp.m_indices[2] = i + 2;
        m_tris.push_back(temp);
    }
}

// Creates a MPolygon from the input list of PVertex positions and colors
MPolygon::MPolygon(const QString& name, const std::vector<glm::vec4>& pos, const std::vector<glm::vec3>& col)
    : m_tris(), m_verts(), m_name(name), mp_texture(nullptr), mp_normalMap(nullptr)
{
    for(unsigned int i = 0; i < pos.size(); i++)
    {
        m_verts.push_back(PVertex(pos[i], col[i], glm::vec4(), glm::vec2()));
    }
    Triangulate();
}

// Creates a regular MPolygon with a number of sides indicated by the "sides" input integer.
// All of its vertices are of color "color", and the MPolygon is centered at "pos".
// It is rotated about its center by "rot" degrees, and is scaled from its center by "scale" units
MPolygon::MPolygon(const QString& name, int sides, glm::vec3 color, glm::vec4 pos, float rot, glm::vec4 scale)
    : m_tris(), m_verts(), m_name(name), mp_texture(nullptr), mp_normalMap(nullptr)
{
    glm::vec4 v(0.f, 1.f, 0.f, 1.f);
    float angle = 360.f / sides;
    for(int i = 0; i < sides; i++)
    {
        glm::vec4 vert_pos = glm::translate(glm::vec3(pos.x, pos.y, pos.z))
                           * glm::rotate(rot, glm::vec3(0.f, 0.f, 1.f))
                           * glm::scale(glm::vec3(scale.x, scale.y, scale.z))
                           * glm::rotate(i * angle, glm::vec3(0.f, 0.f, 1.f))
                           * v;
        m_verts.push_back(PVertex(vert_pos, color, glm::vec4(), glm::vec2()));
    }

    Triangulate();
}

MPolygon::MPolygon(const QString &name)
    : m_tris(), m_verts(), m_name(name), mp_texture(nullptr), mp_normalMap(nullptr)
{}

MPolygon::MPolygon()
    : m_tris(), m_verts(), m_name("MPolygon"), mp_texture(nullptr), mp_normalMap(nullptr)
{}

MPolygon::MPolygon(const MPolygon& p)
    : m_tris(p.m_tris), m_verts(p.m_verts), m_name(p.m_name), mp_texture(nullptr), mp_normalMap(nullptr)
{
    if(p.mp_texture != nullptr)
    {
        mp_texture = new QImage(*p.mp_texture);
    }
    if(p.mp_normalMap != nullptr)
    {
        mp_normalMap = new QImage(*p.mp_normalMap);
    }
}

MPolygon::~MPolygon()
{
    delete mp_texture;
}

void MPolygon::SetTexture(QImage* i)
{
    mp_texture = i;
}

void MPolygon::SetNormalMap(QImage* i)
{
    mp_normalMap = i;
}

void MPolygon::AddTriangle(const Triangle& t)
{
    m_tris.push_back(t);
}

void MPolygon::AddPVertex(const PVertex& v)
{
    m_verts.push_back(v);
}

void MPolygon::ClearTriangles()
{
    m_tris.clear();
}

Triangle& MPolygon::TriAt(unsigned int i)
{
    return m_tris[i];
}

Triangle MPolygon::TriAt(unsigned int i) const
{
    return m_tris[i];
}

PVertex &MPolygon::VertAt(unsigned int i)
{
    return m_verts[i];
}

PVertex MPolygon::VertAt(unsigned int i) const
{
    return m_verts[i];
}
