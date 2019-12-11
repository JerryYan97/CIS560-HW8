#pragma once
#include "la.h"
#include <vector>
#include <QString>
#include <QImage>
#include <QColor>

// A PVertex is a point in space that defines one corner of a polygon.
// Each PVertex has several attributes that determine how they contribute to the
// appearance of their Polygon, such as coloration.
struct PVertex
{
    glm::vec4 m_pos;    // The position of the PVertex. In hw02, this is in pixel space.
    glm::vec3 m_color;  // The color of the PVertex. X corresponds to Red, Y corresponds to Green, and Z corresponds to Blue.
    glm::vec4 m_normal; // The surface normal of the PVertex (not yet used)
    glm::vec2 m_uv;     // The texture coordinates of the PVertex (not yet used)

    PVertex(glm::vec4 p, glm::vec3 c, glm::vec4 n, glm::vec2 u)
        : m_pos(p), m_color(c), m_normal(n), m_uv(u)
    {}
};

// Each Polygon can be decomposed into triangles that fill its area.
struct Triangle
{
    // The indices of the Vertices that make up this triangle.
    // The indices correspond to the std::vector of Vertices stored in the Polygon
    // which stores this Triangle
    unsigned int m_indices[3];
};

class MPolygon
{
public:
    // TODO: Populate this list of triangles in Triangulate()
    std::vector<Triangle> m_tris;
    // The list of Vertices that define this polygon. This is already filled by the Polygon constructor.
    std::vector<PVertex> m_verts;
    // The name of this polygon, primarily to help you debug
    QString m_name;
    // The image that can be read to determine pixel color when used in conjunction with UV coordinates
    // Not used until homework 3.
    QImage* mp_texture;
    // The image that can be read to determine surface normal offset when used in conjunction with UV coordinates
    // Not used until homework 3
    QImage* mp_normalMap;

    // Polygon class constructors
    MPolygon(const QString& name, const std::vector<glm::vec4>& pos, const std::vector<glm::vec3> &col);
    MPolygon(const QString& name, int sides, glm::vec3 color, glm::vec4 pos, float rot, glm::vec4 scale);
    MPolygon(const QString& name);
    MPolygon();
    MPolygon(const MPolygon& p);
    ~MPolygon();

    // TODO: Complete the body of Triangulate() in polygon.cpp
    // Creates a set of triangles that, when combined, fill the area of this convex polygon.
    void Triangulate();

    // Copies the input QImage into this Polygon's texture
    void SetTexture(QImage*);

    // Copies the input QImage into this Polygon's normal map
    void SetNormalMap(QImage*);

    // Various getter, setter, and adder functions
    void AddPVertex(const PVertex&);
    void AddTriangle(const Triangle&);
    void ClearTriangles();

    Triangle& TriAt(unsigned int);
    Triangle TriAt(unsigned int) const;

    PVertex& VertAt(unsigned int);
    PVertex VertAt(unsigned int) const;
};



