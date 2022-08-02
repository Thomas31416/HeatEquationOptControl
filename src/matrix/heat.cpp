#include "heat.hpp"

#include <cassert>
#include <iostream>

#include "cg.hpp"
#include "matrix/id.hpp"
#include "matrix/int.hpp"
#include "matrix/intgrad.hpp"
#include "mesh.hpp"

#define LARGE_VALUE 1.0e154

using namespace std;

Heat::Heat(size_t n, const Mesh &mesh, double tol) :
    m_n(n),
    m_mesh(mesh),
    m_tol(tol),
    m_y0(mesh.vertices().size()),
    m_y1(mesh.vertices().size()),
    m_b(mesh.vertices().size()),
    m_cg(mesh.vertices().size())
{}

size_t Heat::width() const {
    return (m_n + 1) * m_mesh.control_vertices().size();
}

size_t Heat::height() const {
    return (m_n + 1) * m_mesh.vertices().size();
}

void Heat::apply(VectorMut &y, double alpha, const Vector &x) const {
    cout << "* Solving heat equation…" << endl;

    assert(y.size() == height());
    assert(x.size() == width());

    double dt = 1.0 / m_n;

    m_y0 = 0.0;

    for (size_t k = 0;; ++k) {
        y.slice(m_mesh.vertices().size(), k) += alpha * m_y0;

        if (k >= m_n)
            break;

        m_b = 0.0;
        m_b += (1.0 / dt) * IntDom(m_mesh) * m_y0;
        m_b += IntCtrl(m_mesh) * x.slice(m_mesh.control_vertices().size(), k);

        for (auto i : m_mesh.border_vertices())
            m_b[i] = 0.0;

        m_cg.solve((1.0/ dt) * IntDom(m_mesh)
                   + IntGradDom(m_mesh)
                   + LARGE_VALUE * BorderId(m_mesh),
                   m_y1, m_b, m_tol);

        m_y0.swap(m_y1);
    }
}

HeatAdj::HeatAdj(size_t n, const Mesh &mesh, double tol) :
    m_n(n),
    m_mesh(mesh),
    m_tol(tol),
    m_y0(mesh.vertices().size()),
    m_y1(mesh.vertices().size()),
    m_b(mesh.vertices().size()),
    m_cg(mesh.vertices().size())
{}

size_t HeatAdj::width() const {
    return (m_n + 1) * m_mesh.vertices().size();
}

size_t HeatAdj::height() const {
    return (m_n + 1) * m_mesh.control_vertices().size();
}

void HeatAdj::apply(VectorMut &y, double alpha, const Vector &x) const {
    cout << "* Solving adjoint heat equation…" << endl;

    assert(y.size() == height());
    assert(x.size() == width());

    double dt = 1.0 / m_n;

    m_y0 = 0.0;

    for (size_t k = m_n;; --k) {
        for (size_t i = 0; i < m_mesh.control_vertices().size(); ++i)
            y.slice(m_mesh.control_vertices().size(), k)[i] += alpha * m_y0[m_mesh.control_vertices()[i]];

        if (k == 0)
            break;

        m_b = 0.0;
        m_b += (1.0 / dt) * IntDom(m_mesh) * m_y0;
        m_b += IntDom(m_mesh) * x.slice(m_mesh.vertices().size(), k);

        for (auto i : m_mesh.border_vertices())
            m_b[i] = 0.0;

        m_cg.solve((1.0/ dt) * IntDom(m_mesh)
                   + IntGradDom(m_mesh)
                   + LARGE_VALUE * BorderId(m_mesh),
                   m_y1, m_b, m_tol);

        m_y0.swap(m_y1);
    }
}
