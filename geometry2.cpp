#include "geometry2.h"
#include <cmath>

using namespace std;

Vec3f_b::Vec3f_b()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

Vec3f_b::Vec3f_b(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

float Vec3f_b::norm()
{
    return sqrt(x*x + y*y +z*z);
}

Vec3f_b *Vec3f_b::normalize(float l)
{    
    float n = norm();

    this->x = this->x*l/n;
    this->y = this->y*l/n;
    this->z = this->z*l/n;

    return this;
}

float Vec3f_b::scalaire(Vec3f_b other)
{
    return this->x*other.x + this->y * other.y + this->z * other.z;
}

