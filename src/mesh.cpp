#include "mesh.hpp"

#include <cassert>

using namespace std;

Mesh::Mesh(istream &stream,
           const vector<int> &border,
           const vector<int> &control)
{
    size_t n_vertices, n_triangles, n_boundary_edges;
    stream >> n_vertices >> n_triangles >> n_boundary_edges;

    if (!stream.good())
        throw runtime_error("Unexpected end of file");

    m_vertices.reserve(n_vertices);
    m_triangles.reserve(n_triangles);

    for (size_t i = 0; i < n_vertices; ++i) {
        array<double, 2> pos;
        int label;
        stream >> pos[0] >> pos[1] >> label;

        if (!stream.good())
            throw runtime_error("Unexpected end of file");

        m_vertices.push_back(Vertex(pos));
    }

    for (size_t i = 0; i < n_triangles; ++i) {
        array<size_t, 3> vertices;
        int region;
        stream >> vertices[0] >> vertices[1] >> vertices[2] >> region;

        if (!stream.good())
            throw runtime_error("Unexpected end of file");

        vertices[0]--;
        vertices[1]--;
        vertices[2]--;

        if (vertices[0] >= n_vertices ||
            vertices[1] >= n_vertices ||
            vertices[2] >= n_vertices)
        {
            throw runtime_error("Bad triangle vertices");
        }

        m_triangles.push_back(Triangle(*this, vertices));

        for (auto control_region : control) {
            if (region == control_region) {
                m_control_triangles.push_back(i);

                for (auto j : {0, 1, 2}) {
                    if (m_vertices[vertices[j]].control_id() < 0) {
                        m_vertices[vertices[j]].set_control_id(m_control_vertices.size());
                        m_control_vertices.push_back(vertices[j]);
                    }
                }
            }
        }
    }

    for (size_t i = 0; i < n_boundary_edges; ++i) {
        array<size_t, 2> vertices;
        int label;
        stream >> vertices[0] >> vertices[1] >> label;

        if (!stream.good())
            throw runtime_error("Unexpected end of file");

        vertices[0]--;
        vertices[1]--;

        if (vertices[0] >= n_vertices ||
            vertices[1] >= n_vertices)
        {
            throw runtime_error("Bad boundary edge vertices");
        }

        for (auto border_label : border) {
            if (label == border_label) {
                for (auto j : {0, 1}) {
                    if (m_vertices[vertices[j]].border_id() < 0) {
                        m_vertices[vertices[j]].set_border_id(m_border_vertices.size());
                        m_border_vertices.push_back(vertices[j]);
                    }
                }
            }
        }
    }

    m_border_vertices.shrink_to_fit();
    m_control_vertices.shrink_to_fit();
    m_control_triangles.shrink_to_fit();
}

const vector<Vertex> &Mesh::vertices() const {
    return m_vertices;
}

const vector<Triangle> &Mesh::triangles() const {
    return m_triangles;
}

const vector<size_t> &Mesh::border_vertices() const {
    return m_border_vertices;
}

const vector<size_t> &Mesh::control_vertices() const {
    return m_control_vertices;
}

const vector<size_t> &Mesh::control_triangles() const {
    return m_control_triangles;
}

Vertex::Vertex() :
    m_pos({0, 0}),
    m_border_id(-1),
    m_control_id(-1)
{}

Vertex::Vertex(array<double, 2> pos) :
    m_pos(pos),
    m_border_id(-1),
    m_control_id(-1)
{}

const array<double, 2> &Vertex::pos() const {
    return m_pos;
}

ssize_t Vertex::border_id() const {
    return m_border_id;
}

void Vertex::set_border_id(ssize_t id) {
    m_border_id = id;
}

ssize_t Vertex::control_id() const {
    return m_control_id;
}

void Vertex::set_control_id(ssize_t id) {
    m_control_id = id;
}

Vertex Vertex::perp() const {
    return Vertex({-m_pos[1], m_pos[0]});
}

Vertex operator /(const Vertex &lhs, double rhs) {
    return Vertex({
        lhs.pos()[0] / rhs,
        lhs.pos()[1] / rhs
    });
}

Vertex operator -(const Vertex &lhs, const Vertex &rhs) {
    return Vertex({
        lhs.pos()[0] - rhs.pos()[0],
        lhs.pos()[1] - rhs.pos()[1]
    });
}

double operator ,(const Vertex &lhs, const Vertex &rhs) {
    return lhs.pos()[0] * rhs.pos()[0] + lhs.pos()[1] * rhs.pos()[1];
}

ostream &operator <<(ostream &stream, const Vertex &vertex) {
    return stream << vertex.pos()[0] << " " << vertex.pos()[1];
}

Triangle::Triangle(const Mesh &mesh, array<size_t, 3> vertices) :
    m_vertices(vertices)
{
    assert(vertices[0] < mesh.vertices().size());
    assert(vertices[1] < mesh.vertices().size());
    assert(vertices[2] < mesh.vertices().size());

    Vertex v1 = mesh.vertices()[vertices[1]] - mesh.vertices()[vertices[0]];
    Vertex v2 = mesh.vertices()[vertices[2]] - mesh.vertices()[vertices[0]];

    m_area = 0.5 * (v1.pos()[0] * v2.pos()[1] - v1.pos()[1] * v2.pos()[0]);
    assert(m_area >= 0.0);
}

const array<size_t, 3> &Triangle::vertices() const {
    return m_vertices;
}

double Triangle::area() const {
    return m_area;
}
