#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>
#include <vector>

using namespace std;

class Matrix;

template<typename t> struct Vec3
{
    t x,y,z;

    Vec3(){}
    Vec3(t x, t y, t z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vec3 *normalize(float l = 1){
        float n = norm();

        this->x = this->x*l/n;
        this->y = this->y*l/n;
        this->z = this->z*l/n;

        return this;
    }

    float scalaire(Vec3 other){
        return this->x*other.x + this->y * other.y + this->z * other.z;
    }

    float norm(){
        return sqrt(x*x + y*y +z*z);
    }

    Vec3 operator -(Vec3 p){
        return Vec3<t>(this->x - p.x, this->y - p.y, this->z - p.z);
    }

    Vec3 operator +(Vec3 p){
        return Vec3<t>(this->x + p.x, this->y + p.y, this->z + p.z);
    }

    Vec3 operator ^(Vec3<t> p){
         return Vec3<t>(y*p.z-z*p.y, z*p.x-x*p.z, x*p.y-y*p.x);
    }

    Vec3 operator *(float f){
        return Vec3(x*f, y*f, z*f);
    }

    Vec3 operator *(Vec3<t> p ){
        return x*p.x + y*p.y + z*p.z;
    }

    t& operator[] (int i){
        switch (i){
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        }

    }
};

typedef Vec3<int> Vec3i;
typedef Vec3<float> Vec3f;


/////////////////////////////////////////// github.com/ssloy/tinyrenderer //////////////////////////////////////////////////

class Matrix {
    vector<vector<float> > m;
    int rows, cols;
public:
    Matrix(int r=4, int c=4);
    Matrix(Vec3f p);
    int nrows();
    int ncols();
    static Matrix identity(int dimensions);
    vector<float>& operator[](const int i);
    Matrix operator*(const Matrix& a);
    Matrix transpose();
    Matrix inverse();
    friend ostream& operator<<(ostream& s, Matrix& m);
};

#endif //__GEOMETRY_H__
