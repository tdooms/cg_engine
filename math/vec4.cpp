//============================================================================
// @name        : vec4.cpp
// @author      : Thomas Dooms
// @date        : 10/01/19
// @version     : 1.1
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : math classes inspired by GLSL, uses ^ for elementwise multiplications.
//============================================================================

#include "vec4.h"
#include "math.h"
#include <cstring>
#include <cmath>

/*----------------------------------------*/

Vec4& Vec4::operator+=(const Vec4& v)
{
    data[0] += v[0];   data[1] += v[1];   data[2] += v[2];   data[3] += v[3];
    return *this;
}
Vec4& Vec4::operator-=(const Vec4& v)
{
    data[0] -= v[0];   data[1] -= v[1];   data[2] -= v[2];   data[3] -= v[3];
    return *this;
}
Vec4& Vec4::operator^=(const Vec4& v)
{
    data[0] *= v[0];   data[1] *= v[1];   data[2] *= v[2];   data[3] *= v[3];
    return *this;
}
Vec4& Vec4::operator/=(const Vec4& v)
{
    data[0] /= v[0];   data[1] /= v[1];   data[2] /= v[2];   data[3] /= v[3];
    return *this;
}


Vec4& Vec4::operator*=(const Mat4& m)
{
    double temp[4];
    vecmatmul(data, (double*)&m, temp);
    memcpy(data, temp, 4*sizeof(double));
    return *this;
}
Vec4& Vec4::operator*=(double s)
{
    data[0] *= s;   data[1] *= s;   data[2] *= s;   data[3] *= s;
    return *this;
}
Vec4& Vec4::operator/=(double s)
{
    return *this *= (1.0f/s);
}

/*----------------------------------------*/

Vec4 operator+(const Vec4& a, const Vec4& b)
{
    return { a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3] };
}
Vec4 operator-(const Vec4& a, const Vec4& b)
{
    return { a[0] - b[0], a[1] - b[1], a[2] - b[2], a[3] - b[3] };
}
Vec4 operator^(const Vec4& a, const Vec4& b)
{
    return { a[0] * b[0], a[1] * b[1], a[2] * b[2], a[3] * b[3] };
}
Vec4 operator/(const Vec4& a, const Vec4& b)
{
    return { a[0] / b[0], a[1] / b[1], a[2] / b[2], a[3] / b[3] };
}


Vec4 operator-(const Vec4& a)
{
    return {-a[0], -a[1], -a[2], -a[3] };
}
Vec4 operator*(const Vec4& a, double s)
{
    return { a[0]*s, a[1]*s, a[2]*s, a[3]*s };
}
Vec4 operator/(const Vec4& a, double s)
{
    return a * (1.0f/s);
}
Vec4 operator*(double s, const Vec4& a)
{
    return { a[0]*s, a[1]*s, a[2]*s, a[3]*s };
}
Vec4 operator/(double s, const Vec4& a)
{
    return a * (1.0f/s);
}

/*----------------------------------------*/

double norm(const Vec4& a)
{
    return dot(a, a);
}
double dot(const Vec4& a, const Vec4& b)
{
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2] + a[3]*b[3];
}
Vec4 normalize(const Vec4& a)
{
    return a / sqrt(norm(a));
}
Vec4& normalize(Vec4& a)
{
    return a /= sqrt(norm(a));
}

std::ostream& operator<<(std::ostream& stream, const Vec4& a)
{
    return stream << '('<< a[0] << ", " << a[1] << ", " << a[2] << ", " << a[3] << ')';
}
