#include "matrix.hpp"

#include <cassert>

using namespace std;

ScaledMat::ScaledMat(double scale, const Matrix &matrix) :
    m_scale(scale),
    m_matrix(matrix)
{}

size_t ScaledMat::width() const {
    return m_matrix.width();
}

size_t ScaledMat::height() const {
    return m_matrix.height();
}

void ScaledMat::apply(VectorMut &y, double alpha, const Vector &x) const {
    m_matrix.apply(y, m_scale * alpha, x);
}

ScaledMat operator *(double scale, const Matrix &matrix) {
    return ScaledMat(scale, matrix);
}

MMSum::MMSum(const Matrix &lhs, const Matrix &rhs) :
    m_lhs(lhs),
    m_rhs(rhs)
{
    assert(lhs.width() == rhs.width());
    assert(lhs.height() == rhs.height());
}

size_t MMSum::width() const {
    return m_lhs.width();
}

size_t MMSum::height() const {
    return m_lhs.height();
}

void MMSum::apply(VectorMut &y, double alpha, const Vector &x) const {
    m_lhs.apply(y, alpha, x);
    m_rhs.apply(y, alpha, x);
}

MMSum operator +(const Matrix &lhs, const Matrix &rhs) {
    return MMSum(lhs, rhs);
}

MMProduct::MMProduct(const Matrix &lhs, const Matrix &rhs) :
    m_lhs(lhs),
    m_rhs(rhs),
    m_tmp(rhs.height())
{
    assert(lhs.width() == rhs.height());
}

size_t MMProduct::width() const {
    return m_rhs.width();
}

size_t MMProduct::height() const {
    return m_lhs.height();
}

void MMProduct::apply(VectorMut &y, double alpha, const Vector &x) const {
    m_tmp = m_rhs * x;
    y += alpha * m_lhs * m_tmp;
}

MMProduct operator *(const Matrix &lhs, const Matrix &rhs) {
    return MMProduct(lhs, rhs);
}
