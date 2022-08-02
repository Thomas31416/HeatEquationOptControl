#pragma once

#include "vector.hpp"

class CG {
public:
    CG(size_t size);

    size_t solve(const Matrix &a, VectorMut &x, const Vector &b, double tol);

private:
    VectorMut m_r;
    VectorMut m_p;
    VectorMut m_ap;
};
