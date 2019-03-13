//============================================================================
// @name        : vec2.cpp
// @author      : Thomas Dooms
// @date        : 10/01/19
// @version     : 1.1
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : math classes inspired by GLSL, uses ^ for elementwise multiplications.
//============================================================================

#include "vec4.h"
#include "vec2.h"
#include "math.h"
#include <cstring>
#include <cmath>

/*----------------------------------------*/

Vec2& Vec2::operator+=(const Vec2& v)
{
    data[0] += v[0];   data[1] += v[1];
    return *this;
}
Vec2& Vec2::operator-=(const Vec2& v)
{
    data[0] -= v[0];   data[1] -= v[1];
    return *this;
}
Vec2& Vec2::operator^=(const Vec2& v)
{
    data[0] *= v[0];   data[1] *= v[1];
    return *this;
}
Vec2& Vec2::operator/=(const Vec2& v)
{
    data[0] /= v[0];   data[1] /= v[1];
    return *this;
}

Vec2& Vec2::operator*=(const Mat4& m)
{
    double temp[4];
    vecmatmul(Vec4(*this).data, (double*)&m, temp);
    memcpy(data, temp, 2*sizeof(double));
    return *this;
}
Vec2& Vec2::operator*=(double s)
{
    data[0] *= s;   data[1] *= s;
    return *this;
}
Vec2& Vec2::operator/=(double s)
{
    return *this *= (1.0f/s);
}

/*----------------------------------------*/

Vec2 operator+(const Vec2& a, const Vec2& b)
{
    return { a[0] + b[0], a[1] + b[1]};
}
Vec2 operator-(const Vec2& a, const Vec2& b)
{
    return { a[0] - b[0], a[1] - b[1]};
}
Vec2 operator^(const Vec2& a, const Vec2& b)
{
    return { a[0] * b[0], a[1] * b[1]};
}
Vec2 operator/(const Vec2& a, const Vec2& b)
{
    return { a[0] / b[0], a[1] / b[1]};
}


Vec2 operator-(const Vec2& a)
{
    return {-a[0], -a[1]};
}
Vec2 operator*(const Vec2& a, double s)
{
    return { a[0]*s, a[1]*s};
}
Vec2 operator/(const Vec2& a, double s)
{
    return a * (1.0f/s);
}
Vec2 operator*(double s, const Vec2& a)
{
    return { a[0]*s, a[1]*s};
}
Vec2 operator/(double s, const Vec2& a)
{
    return a * (1.0f/s);
}

/*----------------------------------------*/

double norm(const Vec2& a)
{
    return dot(a, a);
}
double dot(const Vec2& a, const Vec2& b)
{
    return a[0]*b[0] + a[1]*b[1];
}
Vec2 normalize(const Vec2& a)
{
    return a / sqrt(norm(a));
}
Vec2& normalize(Vec2& a)
{
    return a /= sqrt(norm(a));
}

std::ostream& operator<<(std::ostream& stream, const Vec2& a)
{
    return stream << '('<< a[0] << ", " << a[1] << ')';
}

