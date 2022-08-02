#pragma once

#include "matrix.hpp"

class Mesh;

class IntDom : public Matrix {
public:
    IntDom(const Mesh &mesh);

    size_t width() const;
    size_t height() const;

    void apply(VectorMut &y, double alpha, const Vector &x) const;

private:
    const Mesh &m_mesh;
};

class IntCtrl : public Matrix {
public:
    IntCtrl(const Mesh &mesh);

    size_t width() const;
    size_t height() const;

    void apply(VectorMut &y, double alpha, const Vector &x) const;

private:
    const Mesh &m_mesh;
};

class IntTimeCtrl : public Matrix {
public:
    IntTimeCtrl(size_t n, const Mesh &mesh);

    size_t width() const;
    size_t height() const;

    void apply(VectorMut &y, double alpha, const Vector &x) const;

private:
    size_t m_n;
    const Mesh &m_mesh;
};
