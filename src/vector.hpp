#pragma once

#include <cstddef>

class Matrix;
class ScaledVec;
class MVProduct;

class Vector {
public:
    Vector(size_t size);
    Vector(const Vector &other);
    Vector(Vector &&other);
    virtual ~Vector();

    size_t size() const;

    Vector slice(size_t width, size_t k) const;

    const double &operator [](size_t i) const;

    double operator ,(const Vector &other) const;

private:
    friend class VectorMut;

    Vector(size_t size, double *values);

    size_t m_size;
    double *m_values;
    bool m_should_delete;
};

class VectorMut : public Vector {
public:
    VectorMut(size_t size);
    VectorMut(const Vector &other);
    VectorMut(VectorMut &&other);

    VectorMut slice(size_t width, size_t k);

    void swap(VectorMut &other);

    double &operator [](size_t i);

    VectorMut &operator =(double value);
    VectorMut &operator =(const Vector &other);
    VectorMut &operator =(const MVProduct &product);

    VectorMut &operator +=(const Vector &other);
    VectorMut &operator +=(const ScaledVec &scaled);
    VectorMut &operator +=(const MVProduct &product);

    VectorMut &operator -=(const ScaledVec &scaled);

    VectorMut &operator *=(double value);

private:
    VectorMut(size_t size, double *values);
};

class ScaledVec {
public:
    ScaledVec(double scale, const Vector &vector);

    double scale() const;
    const Vector &vector() const;

private:
    double m_scale;
    const Vector &m_vector;
};

ScaledVec operator *(double scale, const Vector &vector);

class MVProduct {
public:
    MVProduct(const Matrix &matrix, const Vector &vector);

    const Matrix &matrix() const;
    const Vector &vector() const;

private:
    const Matrix &m_matrix;
    const Vector &m_vector;
};

MVProduct operator *(const Matrix &matrix, const Vector &vector);
