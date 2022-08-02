#include "intgrad.hpp"

#include <cassert>

#include "mesh.hpp"

using namespace std;

IntGradDom::IntGradDom(const Mesh &mesh) :
    m_mesh(mesh)
{}

size_t IntGradDom::width() const {
    return m_mesh.vertices().size();
}

size_t IntGradDom::height() const {
    return m_mesh.vertices().size();
}

void IntGradDom::apply(VectorMut &y, double alpha, const Vector &x) const {
    assert(y.size() == height());
    assert(x.size() == width());

    for (auto triangle : m_mesh.triangles()) {
        Vertex grad[3];

        for (auto i : {0, 1, 2}) {
            const Vertex &v1 = m_mesh.vertices()[triangle.vertices()[(i + 1) % 3]];
            const Vertex &v2 = m_mesh.vertices()[triangle.vertices()[(i + 2) % 3]];

            grad[i] = (v2 - v1).perp() / (2 * triangle.area());
        }

        for (auto i : {0, 1, 2})
            for (auto j : {0, 1, 2})
                y[triangle.vertices()[i]] +=
                    alpha * triangle.area() * (grad[i], grad[j]) * x[triangle.vertices()[j]];
    }
}
