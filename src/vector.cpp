#include "vector.hpp"

#include <cassert>
#include <utility>

#include "blas.hpp"
#include "matrix.hpp"

using namespace std;

Vector::Vector(size_t size) :
    m_size(size),
    m_values(new double[size]),
    m_should_delete(true)
{
    int n = size, incx = 0, incy = 1;
    double x = 0.0;
    dcopy_(&n, &x, &incx, m_values, &incy);
}

Vector::Vector(const Vector &other) :
    m_size(other.m_size),
    m_values(new double[other.m_size]),
    m_should_delete(true)
{
    int n = size(), incx = 1, incy = 1;
    dcopy_(&n, other.m_values, &incx, m_values, &incy);
}

Vector::Vector(Vector &&other) :
    m_size(other.m_size),
    m_values(other.m_values),
    m_should_delete(other.m_should_delete)
{
    other.m_should_delete = false;
}

Vector::~Vector() {
    if (m_should_delete)
        delete [] m_values;
}

size_t Vector::size() const {
    return m_size;
}

Vector Vector::slice(size_t width, size_t k) const {
    assert (k * width <= size());
    return Vector(width, m_values + k * width);
}

const double &Vector::operator [](size_t i) const {
    return m_values[i];
}

double Vector::operator ,(const Vector &other) const {
    assert(other.size() == size());
    int n = size(), incx = 1, incy = 1;
    return ddot_(&n, m_values, &incx, other.m_values, &incy);
}

Vector::Vector(size_t size, double *values) :
    m_size(size),
    m_values(values),
    m_should_delete(false)
{}

VectorMut::VectorMut(size_t size) :
    Vector(size)
{}

VectorMut::VectorMut(const Vector &other) :
    Vector(other)
{}

VectorMut::VectorMut(VectorMut &&other) :
    Vector(move(other))
{}

VectorMut VectorMut::slice(size_t width, size_t k) {
    assert (k * width <= size());
    return VectorMut(width, m_values + k * width);
}

void VectorMut::swap(VectorMut &other) {
    size_t tmp_size = other.m_size;
    double *tmp_values = other.m_values;
    bool tmp_should_delete = other.m_should_delete;

    other.m_size = m_size;
    other.m_values = m_values;
    other.m_should_delete = m_should_delete;

    m_size = tmp_size;
    m_values = tmp_values;
    m_should_delete = tmp_should_delete;
}

double &VectorMut::operator [](size_t i) {
    return m_values[i];
}

VectorMut &VectorMut::operator =(double value) {
    int n = size(), incx = 0, incy = 1;
    dcopy_(&n, &value, &incx, m_values, &incy);
    return *this;
}

VectorMut &VectorMut::operator =(const Vector &other) {
    assert(other.size() == size());
    int n = size(), incx = 1, incy = 1;
    dcopy_(&n, other.m_values, &incx, m_values, &incy);
    return *this;
}

VectorMut &VectorMut::operator =(const MVProduct &product) {
    *this = 0.0;
    return *this += product;
}

VectorMut &VectorMut::operator +=(const Vector &other) {
    return *this += 1.0 * other;
}

VectorMut &VectorMut::operator +=(const ScaledVec &scaled) {
    assert(scaled.vector().size() == size());
    int n = size(), incx = 1, incy = 1;
    double alpha = scaled.scale();
    daxpy_(&n, &alpha, scaled.vector().m_values, &incx, m_values, &incy);
    return *this;
}

VectorMut &VectorMut::operator +=(const MVProduct &product) {
    product.matrix().apply(*this, 1.0, product.vector());
    return *this;
}

VectorMut &VectorMut::operator -=(const ScaledVec &scaled) {
    assert(scaled.vector().size() == size());
    int n = size(), incx = 1, incy = 1;
    double alpha = -scaled.scale();
    daxpy_(&n, &alpha, scaled.vector().m_values, &incx, m_values, &incy);
    return *this;
}

VectorMut &VectorMut::operator *=(double value) {
    int n = size(), incx = 1;
    dscal_(&n, &value, m_values, &incx);
    return *this;
}

VectorMut::VectorMut(size_t size, double *values) :
    Vector(size, values)
{}

ScaledVec::ScaledVec(double scale, const Vector &vector) :
    m_scale(scale),
    m_vector(vector)
{}

double ScaledVec::scale() const {
    return m_scale;
}

const Vector &ScaledVec::vector() const {
    return m_vector;
}

ScaledVec operator *(double scale, const Vector &vector) {
    return ScaledVec(scale, vector);
}

MVProduct::MVProduct(const Matrix &matrix, const Vector &vector) :
    m_matrix(matrix),
    m_vector(vector)
{}

const Matrix &MVProduct::matrix() const {
    return m_matrix;
}

const Vector &MVProduct::vector() const {
    return m_vector;
}

MVProduct operator *(const Matrix &matrix, const Vector &vector) {
    return MVProduct(matrix, vector);
}
