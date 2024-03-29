//============================================================================
// @name        : mat4.cpp
// @author      : Thomas Dooms
// @date        : 10/01/19
// @version     : 1.1
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : math classes inspired by GLSL, uses ^ for elementwise multiplications.
//============================================================================

#include "mat4.h"
#include "math.h"
#include <cstring>
#include <cmath>
#include <array>

/*----------------------------------------*/

Mat4::Mat4(const Vec4& v0, const Vec4& v1, const Vec4& v2, const Vec4& v3) :
    data{ v0[0], v0[1], v0[2], v0[3],
          v1[0], v1[1], v1[2], v1[3],
          v2[0], v2[1], v2[2], v2[3],
          v3[0], v3[1], v3[2], v3[3] } {}

Mat4::Mat4(const double* f)
{
    memcpy(data, f, 16*sizeof(double));
}

/*----------------------------------------*/

Mat4 Mat4::createUnitMatrix()
{
    return {{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1}};
}
Mat4 Mat4::createTranslationMatrix(double x, double y, double z)
{
    return {{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {x,y,z,1}};
}
Mat4 Mat4::createTranslationMatrix(const Vec3& xyz)
{
    return {{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {xyz[0],xyz[1],xyz[2],1}};
}
Mat4 Mat4::createScalarMatrix(double x, double y, double z)
{
    return {{x,0,0,0}, {0,y,0,0}, {0,0,z,0}, {0,0,0,1}};
}
Mat4 Mat4::createScalarMatrix(double s)
{
    return {{s,0,0,0}, {0,s,0,0}, {0,0,s,0}, {0,0,0,1}};
}

Mat4 Mat4::createZRotationMatrix(double alpha)
{
    double sina = std::sin(alpha);
    double cosa = std::cos(alpha);
    return {{cosa, sina, 0, 0}, {-sina, cosa, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
}
Mat4 Mat4::createXRotationMatrix(double alpha)
{
    double sina = std::sin(alpha);
    double cosa = std::cos(alpha);
    return {{1, 0, 0, 0}, {0, cosa, sina, 0}, {0, -sina, cosa, 0}, {0, 0, 0, 1}};
}
Mat4 Mat4::createYRotationMatrix(double alpha)
{
    double sina = std::sin(alpha);
    double cosa = std::cos(alpha);
    return {{cosa, 0, -sina, 0}, {0, 1, 0, 0}, {sina, 0, cosa, 0}, {0, 0, 0, 1}};
}

Mat4 Mat4::createRotationMatrix(double alphaX, double alphaY, double alphaZ)
{
    return createXRotationMatrix(alphaX) * createYRotationMatrix(alphaY) * createZRotationMatrix(alphaZ);
}

Mat4 Mat4::createTotalTranslationMatrix(const Vec3& translation, double scale, const Vec3& rotation)
{
    return createScalarMatrix(scale) * createRotationMatrix(rotation[0], rotation[1], rotation[2]) * createTranslationMatrix(translation[0], translation[1], translation[2]);
}
Mat4 Mat4::createEyeTransformationMatrix(double x, double y, double z)
{
    double r = sqrt(x*x + y*y + z*z);
    double theta = std::atan2(y,x);
    double phi = std::acos(z/r);
//
//    double sinp = std::sin(phi);    double cosp = std::cos(phi);
//    double sint = std::sin(theta);  double cost = std::cos(theta);
//
//    return {{-sint, -cost*cosp, cost*sinp, 0}, {cost, -sint*cosp, sint*sinp, 0}, {0, sinp, cosp, 0}, {0, 0, -r, 1}};

    return createZRotationMatrix(-M_PI_2-theta) * createXRotationMatrix(-phi) * createTranslationMatrix(0, 0, -r);
}

/*----------------------------------------*/

double Mat4::determinant(const Mat4& m)
{
    return m[0] * det3x3(m.begin(), 0, 0) - m[4] * det3x3(m.begin(), 1, 0) + m[8] * det3x3(m.begin(), 2, 0) - m[12] * det3x3(m.begin(), 3, 0);
}
Mat4 Mat4::adjugate(const Mat4& m)
{
    Mat4 res;
    for(uint32_t i = 0; i < 4; i++)
        for(uint32_t j = 0; j < 4; j++)
            res[4*j+i] = std::pow(-1, i+j) * det3x3(m.begin(), i, j);
    return res;
}
Mat4 Mat4::inverse(const Mat4& m)
{
    return adjugate(m) / determinant(m);
}

/*----------------------------------------*/

Mat4& Mat4::operator+=(const Mat4& m)
{
    for(uint32_t i = 0; i < 16; i++) data[i] += m.data[i];
    return *this;
}
Mat4& Mat4::operator-=(const Mat4& m)
{
    for(uint32_t i = 0; i < 16; i++) data[i] -= m.data[i];
    return *this;
}
Mat4& Mat4::operator^=(const Mat4& m)
{
    for(uint32_t i = 0; i < 16; i++) data[i] *= m.data[i];
    return *this;
}
Mat4& Mat4::operator/=(const Mat4& m)
{
    for(uint32_t i = 0; i < 16; i++) data[i] /= m.data[i];
    return *this;
}
Mat4& Mat4::operator*=(const Mat4& m)
{
    double temp[16];
    matmul(data, m.data, temp);
    memcpy(data, temp, 16*sizeof(double));
    return *this;
}


Mat4& Mat4::operator*=(double s)
{
    for(double item : data) item *= s;
    return *this;
}
Mat4& Mat4::operator/=(double s)
{
    for(double item : data) item /= s;
    return *this;
}

/*----------------------------------------*/

Mat4 operator+(const Mat4& a, const Mat4& b)
{
    double res[16];
    for(uint32_t i = 0; i < 16; i++) res[i] = a.data[i] + b.data[i];
    return res;
}
Mat4 operator-(const Mat4& a, const Mat4& b)
{
    double res[16];
    for(uint32_t i = 0; i < 16; i++) res[i] = a.data[i] - b.data[i];
    return res;
}
Mat4 operator^(const Mat4& a, const Mat4& b)
{
    double res[16];
    for(uint32_t i = 0; i < 16; i++) res[i] = a.data[i] * b.data[i];
    return res;
}
Mat4 operator/(const Mat4& a, const Mat4& b)
{
    double res[16];
    for(uint32_t i = 0; i < 16; i++) res[i] = a.data[i] / b.data[i];
    return res;
}
Mat4 operator*(const Mat4& a, const Mat4& b)
{
    Mat4 res = {};
    matmul(a.data, b.data, res.data);
    return res;
}


Mat4 operator-(const Mat4& a)
{
    Mat4 res = {};
    for(uint32_t i = 0; i < 16; i++) res.data[i] = -a.data[i];
    return res;
}
Mat4 operator*(const Mat4& a, double s)
{
    Mat4 res = {};
    for(uint32_t i = 0; i < 16; i++) res.data[i] = a.data[i]*s;
    return res;
}
Mat4 operator/(const Mat4& a, double s)
{
    return a * (1.0f/s);
}
Mat4 operator*(double s, const Mat4& a)
{
    return a * s;
}
Mat4 operator/(double s, const Mat4& a)
{
    return a * (1.0f/s);
}


Vec4 operator*(const Mat4& a, const Vec4& b)
{
    Vec4 res;
    matvecmul(a.data, b.data, res.data);
    return res;
}

Vec4 operator*(const Vec4& a, const Mat4& b)
{
    Vec4 res;
    vecmatmul(a.data, b.data, res.data);
    return res;
}

/*----------------------------------------*/

std::ostream& operator<<(std::ostream& stream, const Mat4& a)
{
    std::string line[4];
    std::string temp;
    for(int i = 0; i < 4; i++)
    {
        for(uint32_t j = 0; j < 4; j++)
        {
            temp = std::to_string(a[i+4*j]);
            temp.erase(temp.find_last_not_of('0') + 1, std::string::npos);
            if(temp.back() == '.') temp.erase(temp.end() - 1, temp.end());
            line[j] += temp;
        }

        uint32_t max = std::max( std::max(line[0].size(), line[1].size()) , std::max( line[2].size(), line[3].size()) );
        for(uint32_t j = 0; j < 4; j++)
        {
            for (uint32_t k = line[j].size(); k < max; k++) line[j] += ' ';
            if(i!=3) line[j] += ", ";
        }
    }
    stream << line[0] << '\n' << line[1] << '\n' << line[2] << '\n' << line[3] << '\n';
    return stream;
}