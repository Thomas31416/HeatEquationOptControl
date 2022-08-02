#pragma once

#include "matrix.hpp"

class Mesh;

class Id : public Matrix {
public:
    Id(size_t size);

    size_t width() const;
    size_t height() const;

    void apply(VectorMut &y, double alpha, const Vector &x) const;

private:
    size_t m_size;
};

class BorderId : public Matrix {
public:
    BorderId(const Mesh &mesh);

    size_t width() const;
    size_t height() const;

    void apply(VectorMut &y, double alpha, const Vector &x) const;

private:
    const Mesh &m_mesh;
};
