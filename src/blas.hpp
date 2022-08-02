#pragma once

extern "C" {

void dscal_(int *n, double *alpha, double *x, int *incx);
void dcopy_(int *n, double *x, int *incx, double *y, int *incy);
void daxpy_(int *n, double *alpha, double *x, int *incx, double *y, int *incy);
double ddot_(int *n, double *x, int *incx, double *y, int *incy);

}
