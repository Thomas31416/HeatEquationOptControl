#include "id.hpp"

#include <cassert>

#include "mesh.hpp"

using namespace std;

Id::Id(size_t size) :
    m_size(size)
{}

size_t Id::width() const {
    return m_size;
}

size_t Id::height() const {
    return m_size;
}

void Id::apply(VectorMut &y, double alpha, const Vector &x) const {
    assert(y.size() == height());
    assert(x.size() == width());

    y += alpha * x;
}

BorderId::BorderId(const Mesh &mesh) :
    m_mesh(mesh)
{}

size_t BorderId::width() const {
    return m_mesh.vertices().size();
}

size_t BorderId::height() const {
    return m_mesh.vertices().size();
}

void BorderId::apply(VectorMut &y, double alpha, const Vector &x) const {
    assert(y.size() == height());
    assert(x.size() == width());

    for (auto i : m_mesh.border_vertices())
        y[i] += alpha * x[i];
}
