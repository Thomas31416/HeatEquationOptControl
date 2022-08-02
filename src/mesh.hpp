#pragma once

#include <array>
#include <iostream>
#include <set>
#include <vector>

class Vertex;
class Triangle;

class Mesh {
public:
    Mesh(std::istream &stream,
         const std::vector<int> &border,
         const std::vector<int> &control);

    const std::vector<Vertex> &vertices() const;
    const std::vector<Triangle> &triangles() const;

    const std::vector<size_t> &border_vertices() const;
    const std::vector<size_t> &control_vertices() const;
    const std::vector<size_t> &control_triangles() const;

private:
    std::vector<Vertex> m_vertices;
    std::vector<Triangle> m_triangles;
    std::vector<size_t> m_border_vertices;
    std::vector<size_t> m_control_vertices;
    std::vector<size_t> m_control_triangles;
};

class Vertex {
public:
    Vertex();
    Vertex(std::array<double, 2> pos);

    const std::array<double, 2> &pos() const;

    ssize_t border_id() const;
    void set_border_id(ssize_t id);

    ssize_t control_id() const;
    void set_control_id(ssize_t id);

    Vertex perp() const;

private:
    std::array<double, 2> m_pos;
    ssize_t m_border_id;
    ssize_t m_control_id;
};

Vertex operator /(const Vertex &lhs, double rhs);
Vertex operator -(const Vertex &lhs, const Vertex &rhs);
double operator ,(const Vertex &lhs, const Vertex &rhs);

std::ostream &operator <<(std::ostream &stream, const Vertex &vertex);

class Triangle {
public:
    Triangle(const Mesh &mesh, std::array<size_t, 3> vertices);

    const std::array<size_t, 3> &vertices() const;
    double area() const;

private:
    std::array<size_t, 3> m_vertices;
    double m_area;
};
