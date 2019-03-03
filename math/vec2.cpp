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

/*----------------------------------------*/

vec2& vec2::operator+=(const vec2& v)
{
    data[0] += v[0];   data[1] += v[1];
    return *this;
}
vec2& vec2::operator-=(const vec2& v)
{
    data[0] -= v[0];   data[1] -= v[1];
    return *this;
}
vec2& vec2::operator^=(const vec2& v)
{
    data[0] *= v[0];   data[1] *= v[1];
    return *this;
}
vec2& vec2::operator/=(const vec2& v)
{
    data[0] /= v[0];   data[1] /= v[1];
    return *this;
}

vec2& vec2::operator*=(const mat4& m)
{
    double temp[4];
    vecmatmul(vec4(*this).data, (double*)&m, temp);
    memcpy(data, temp, 2*sizeof(double));
    return *this;
}
vec2& vec2::operator*=(double s)
{
    data[0] *= s;   data[1] *= s;
    return *this;
}
vec2& vec2::operator/=(double s)
{
    return *this *= (1.0f/s);
}

/*----------------------------------------*/

vec2 operator+(const vec2& a, const vec2& b)
{
    return { a[0] + b[0], a[1] + b[1]};
}
vec2 operator-(const vec2& a, const vec2& b)
{
    return { a[0] - b[0], a[1] - b[1]};
}
vec2 operator^(const vec2& a, const vec2& b)
{
    return { a[0] * b[0], a[1] * b[1]};
}
vec2 operator/(const vec2& a, const vec2& b)
{
    return { a[0] / b[0], a[1] / b[1]};
}


vec2 operator-(const vec2& a)
{
    return {-a[0], -a[1]};
}
vec2 operator*(const vec2& a, double s)
{
    return { a[0]*s, a[1]*s};
}
vec2 operator/(const vec2& a, double s)
{
    return a * (1.0f/s);
}
vec2 operator*(double s, const vec2& a)
{
    return { a[0]*s, a[1]*s};
}
vec2 operator/(double s, const vec2& a)
{
    return a * (1.0f/s);
}

/*----------------------------------------*/

double norm(const vec2& a)
{
    return dot(a, a);
}
double dot(const vec2& a, const vec2& b)
{
    return a[0]*b[0] + a[1]*b[1];
}
vec2 normalize(const vec2& a)
{
    return a / norm(a);
}
vec2& normalize(vec2& a)
{
    return a /= norm(a);
}

std::ostream& operator<<(std::ostream& stream, const vec2& a)
{
    return stream << '('<< a[0] << ", " << a[1] << ')';
}

