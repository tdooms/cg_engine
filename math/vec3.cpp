//============================================================================
// @name        : vec3.cpp
// @author      : Thomas Dooms
// @date        : 10/01/19
// @version     : 1.1
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : math classes inspired by GLSL, uses ^ for elementwise multiplications.
//============================================================================

#include "vec4.h"
#include "vec3.h"
#include "math.h"
#include <cstring>

/*----------------------------------------*/

vec3& vec3::operator+=(const vec3& v)
{
    data[0] += v[0];   data[1] += v[1];   data[2] += v[2];
    return *this;
}
vec3& vec3::operator-=(const vec3& v)
{
    data[0] -= v[0];   data[1] -= v[1];   data[2] -= v[2];
    return *this;
}
vec3& vec3::operator^=(const vec3& v)
{
    data[0] *= v[0];   data[1] *= v[1];   data[2] *= v[2];
    return *this;
}
vec3& vec3::operator/=(const vec3& v)
{
    data[0] /= v[0];   data[1] /= v[1];   data[2] /= v[2];
    return *this;
}

vec3& vec3::operator*=(const mat4& m)
{
    double temp[4];
    vecmatmul(vec4(*this).data, (double*)&m, temp);
    memcpy(data, temp, 3*sizeof(double));
    return *this;
}
vec3& vec3::operator*=(double s)
{
    data[0] *= s;   data[1] *= s;   data[2] *= s;
    return *this;
}
vec3& vec3::operator/=(double s)
{
    return *this *= (1.0f/s);
}

/*----------------------------------------*/

vec3 operator+(const vec3& a, const vec3& b)
{
    return { a[0] + b[0], a[1] + b[1], a[2] + b[2]};
}
vec3 operator-(const vec3& a, const vec3& b)
{
    return { a[0] - b[0], a[1] - b[1], a[2] - b[2]};
}
vec3 operator^(const vec3& a, const vec3& b)
{
    return { a[0] * b[0], a[1] * b[1], a[2] * b[2]};
}
vec3 operator/(const vec3& a, const vec3& b)
{
    return { a[0] / b[0], a[1] / b[1], a[2] / b[2]};
}


vec3 operator-(const vec3& a)
{
    return {-a[0], -a[1], -a[2]};
}
vec3 operator*(const vec3& a, double s)
{
    return { a[0]*s, a[1]*s, a[2]*s};
}
vec3 operator/(const vec3& a, double s)
{
    return a * (1.0f/s);
}
vec3 operator*(double s, const vec3& a)
{
    return { a[0]*s, a[1]*s, a[2]*s};
}
vec3 operator/(double s, const vec3& a)
{
    return a * (1.0f/s);
}

/*----------------------------------------*/

vec3 cross(const vec3& a, const vec3& b)
{
    return {a[1]*b[2]-a[2]*b[1], a[0]*b[2]-a[2]*b[0], a[0]*b[1]-a[1]*b[0]};
}
double norm(const vec3& a)
{
    return dot(a, a);
}
double dot(const vec3& a, const vec3& b)
{
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}
vec3 normalize(const vec3& a)
{
    return a / norm(a);
}
vec3& normalize(vec3& a)
{
    return a /= norm(a);
}

std::ostream& operator<<(std::ostream& stream, const vec3& a)
{
    return stream << '('<< a[0] << ", " << a[1] << ", " << a[2] << ')';
}
