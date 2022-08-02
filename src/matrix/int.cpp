#include "int.hpp"

#include <cassert>

#include "mesh.hpp"

using namespace std;

template<typename S, typename D>
static void integrate(double alpha, const Triangle &triangle, S src, D dest) {
    for (auto i : triangle.vertices())
        for (auto j : triangle.vertices())
            dest(i) += alpha * triangle.area() * (1.0 + (i == j)) / 12.0 * src(j);
}

IntDom::IntDom(const Mesh &mesh) :
    m_mesh(mesh)
{}

size_t IntDom::width() const {
    return m_mesh.vertices().size();
}

size_t IntDom::height() const {
    return m_mesh.vertices().size();
}

void IntDom::apply(VectorMut &y, double alpha, const Vector &x) const {
    assert(y.size() == height());
    assert(x.size() == width());

    auto src = [&](size_t i) {
        return x[i];
    };

    auto dest = [&](size_t i) -> double & {
        return y[i];
    };

    for (auto triangle : m_mesh.triangles())
        integrate(alpha, triangle, src, dest);
}

IntCtrl::IntCtrl(const Mesh &mesh) :
    m_mesh(mesh)
{}

size_t IntCtrl::width() const {
    return m_mesh.control_vertices().size();
}

size_t IntCtrl::height() const {
    return m_mesh.vertices().size();
}

void IntCtrl::apply(VectorMut &y, double alpha, const Vector &x) const {
    assert(y.size() == height());
    assert(x.size() == width());

    auto src = [&](size_t i) {
        return x[m_mesh.vertices()[i].control_id()];
    };

    auto dest = [&](size_t i) -> double & {
        return y[i];
    };

    for (auto t : m_mesh.control_triangles())
        integrate(alpha, m_mesh.triangles()[t], src, dest);
}

IntTimeCtrl::IntTimeCtrl(size_t n, const Mesh &mesh) :
    m_n(n),
    m_mesh(mesh)
{}

size_t IntTimeCtrl::width() const {
    return (m_n + 1) * m_mesh.control_vertices().size();
}

size_t IntTimeCtrl::height() const {
    return (m_n + 1) * m_mesh.control_vertices().size();
}

void IntTimeCtrl::apply(VectorMut &y, double alpha, const Vector &x) const {
    assert(y.size() == height());
    assert(x.size() == width());

    for (size_t k = 0; k < m_n + 1; ++k) {
        Vector x_slice = x.slice(m_mesh.control_vertices().size(), k);
        VectorMut y_slice = y.slice(m_mesh.control_vertices().size(), k);

        auto src = [&](size_t i) {
            return x_slice[m_mesh.vertices()[i].control_id()];
        };

        auto dest = [&](size_t i) -> double & {
            return y_slice[m_mesh.vertices()[i].control_id()];
        };

        for (auto t : m_mesh.control_triangles())
            integrate(alpha / (m_n + 1), m_mesh.triangles()[t], src, dest);
    }
}
