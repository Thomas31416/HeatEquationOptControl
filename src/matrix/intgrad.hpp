#pragma once

#include "matrix.hpp"

class Mesh;

class IntGradDom : public Matrix {
public:
    IntGradDom(const Mesh &mesh);

    size_t width() const;
    size_t height() const;

    void apply(VectorMut &y, double alpha, const Vector &x) const;

private:
    const Mesh &m_mesh;
};
