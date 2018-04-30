#ifndef UTILS
#define UTILS

#include "matrix.h"
#include <math.h>
#include <sys/time.h>

# define PI           3.14159265358979323846  /* pi */

typedef struct timeval timestamp;

double ms_time();

float deg2rad(float deg);

float rad2deg(float rad);

Matrix4x4f T(const Vector3f& v);

Matrix4x4f S(const Vector3f& v);

Matrix4x4f Rx(float angle);

Matrix4x4f Ry(float angle);

Matrix4x4f Rz(float angle);

#endif // UTILS
