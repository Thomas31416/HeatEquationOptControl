#include "cg.hpp"

#include <cassert>
#include <stdexcept>

using namespace std;

CG::CG(size_t size) :
    m_r(size),
    m_p(size),
    m_ap(size)
{}

size_t CG::solve(const Matrix &a, VectorMut &x, const Vector &b, double tol) {
    assert(x.size() == m_r.size());
    assert(b.size() == m_r.size());

    size_t maxit = 2 * m_r.size();

    x = 0.0;
    m_r = b;
    m_p = b;

    double squared_norm_r = (m_r, m_r);
    double relative_tol = tol * tol * squared_norm_r;

    for (size_t i = 0; i < 2 * maxit; ++i) {
        m_ap = a * m_p;

        double alpha = (m_r, m_r) / (m_p, m_ap);
        x += alpha * m_p;
        m_r -= alpha * m_ap;

        double new_squared_norm_r = (m_r, m_r);

        if (new_squared_norm_r <= relative_tol)
            return i + 1;

        m_p *= (new_squared_norm_r / squared_norm_r);
        m_p += m_r;

        squared_norm_r = new_squared_norm_r;
    }

    throw runtime_error("Conjugate gradient did not converge");
}
