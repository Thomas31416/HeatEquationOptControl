#pragma once

#include "cg.hpp"
#include "matrix.hpp"

class Mesh;

class Heat : public Matrix {
public:
    Heat(size_t n, const Mesh &mesh, double tol);

    size_t width() const;
    size_t height() const;

    void apply(VectorMut &y, double alpha, const Vector &x) const;

private:
    size_t m_n;
    const Mesh &m_mesh;
    double m_tol;
    mutable VectorMut m_y0;
    mutable VectorMut m_y1;
    mutable VectorMut m_b;
    mutable CG m_cg;
};

class HeatAdj : public Matrix {
public:
    HeatAdj(size_t n, const Mesh &mesh, double tol);

    size_t width() const;
    size_t height() const;

    void apply(VectorMut &y, double alpha, const Vector &x) const;

private:
    size_t m_n;
    const Mesh &m_mesh;
    double m_tol;
    mutable VectorMut m_y0;
    mutable VectorMut m_y1;
    mutable VectorMut m_b;
    mutable CG m_cg;
};
