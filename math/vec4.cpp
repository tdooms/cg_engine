//
// Created by Thomas on 10/01/2019.
//

#include "vec4.h"
#include "math.h"
#include <cstring>

/*----------------------------------------*/

vec4& vec4::operator+=(const vec4& v)
{
    data[0] += v[0];   data[1] += v[1];   data[2] += v[2];   data[3] += v[3];
    return *this;
}
vec4& vec4::operator-=(const vec4& v)
{
    data[0] -= v[0];   data[1] -= v[1];   data[2] -= v[2];   data[3] -= v[3];
    return *this;
}
vec4& vec4::operator^=(const vec4& v)
{
    data[0] *= v[0];   data[1] *= v[1];   data[2] *= v[2];   data[3] *= v[3];
    return *this;
}
vec4& vec4::operator/=(const vec4& v)
{
    data[0] /= v[0];   data[1] /= v[1];   data[2] /= v[2];   data[3] /= v[3];
    return *this;
}


vec4& vec4::operator*=(const mat4& m)
{
    double temp[4];
    matvecmul((double*)&m, data, temp);
    memcpy(data, temp, 16);
    return *this;
}
vec4& vec4::operator*=(double s)
{
    data[0] *= s;   data[1] *= s;   data[2] *= s;   data[3] *= s;
    return *this;
}
vec4& vec4::operator/=(double s)
{
    return *this *= (1.0f/s);
}

/*----------------------------------------*/

vec4 operator+(const vec4& a, const vec4& b)
{
    return { a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3] };
}
vec4 operator-(const vec4& a, const vec4& b)
{
    return { a[0] - b[0], a[1] - b[1], a[2] - b[2], a[3] - b[3] };
}
vec4 operator^(const vec4& a, const vec4& b)
{
    return { a[0] * b[0], a[1] * b[1], a[2] * b[2], a[3] * b[3] };
}
vec4 operator/(const vec4& a, const vec4& b)
{
    return { a[0] / b[0], a[1] / b[1], a[2] / b[2], a[3] / b[3] };
}


vec4 operator-(const vec4& a)
{
    return {-a[0], -a[1], -a[2], -a[3] };
}
vec4 operator*(const vec4& a, double s)
{
    return { a[0]*s, a[1]*s, a[2]*s, a[3]*s };
}
vec4 operator/(const vec4& a, double s)
{
    return a * (1.0f/s);
}
vec4 operator*(double s, const vec4& a)
{
    return { a[0]*s, a[1]*s, a[2]*s, a[3]*s };
}
vec4 operator/(double s, const vec4& a)
{
    return a * (1.0f/s);
}

/*----------------------------------------*/

vec4 cross(const vec4& a, const vec4& b)
{
    return {a[1]*b[2]-a[2]*b[1], a[0]*b[2]-a[2]*b[0], a[0]*b[1]-a[1]*b[0], 0};
}
double norm(const vec4& a)
{
    return dot(a, a);
}
double dot(const vec4& a, const vec4& b)
{
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2] + a[3]*b[3];
}
vec4 normalize(const vec4& a)
{
    return a / norm(a);
}
vec4& normalize(vec4& a)
{
    return a /= norm(a);
}


std::ostream& operator<<(std::ostream& stream, const vec4& a)
{
    return stream << '('<< a[0] << ", " << a[1] << ", " << a[2] << ", " << a[3] << ')';
}
