#pragma once

#include <cstddef>

#include "vector.hpp"

class Matrix {
public:
    virtual size_t width() const = 0;
    virtual size_t height() const = 0;

    virtual void apply(VectorMut &y, double alpha, const Vector &x) const = 0;
};

class ScaledMat : public Matrix {
public:
    ScaledMat(double scale, const Matrix &matrix);

    size_t width() const;
    size_t height() const;

    void apply(VectorMut &y, double alpha, const Vector &x) const;

private:
    double m_scale;
    const Matrix &m_matrix;
};

ScaledMat operator *(double scale, const Matrix &matrix);

class MMSum : public Matrix {
public:
    MMSum(const Matrix &lhs, const Matrix &rhs);

    size_t width() const;
    size_t height() const;

    void apply(VectorMut &y, double alpha, const Vector &x) const;

private:
    const Matrix &m_lhs;
    const Matrix &m_rhs;
};

MMSum operator +(const Matrix &lhs, const Matrix &rhs);

class MMProduct : public Matrix {
public:
    MMProduct(const Matrix &lhs, const Matrix &rhs);

    size_t width() const;
    size_t height() const;

    void apply(VectorMut &y, double alpha, const Vector &x) const;

private:
    const Matrix &m_lhs;
    const Matrix &m_rhs;
    mutable VectorMut m_tmp;
};

MMProduct operator *(const Matrix &lhs, const Matrix &rhs);
