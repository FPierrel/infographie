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

        return *this;
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



/*
template <class t> struct Vec2 {
    t x, y;
    Vec2<t>() : x(t()), y(t()) {}
    Vec2<t>(t _x, t _y) : x(_x), y(_y) {}
    Vec2<t> operator +(const Vec2<t> &V) const { return Vec2<t>(x+V.x, y+V.y); }
    Vec2<t> operator -(const Vec2<t> &V) const { return Vec2<t>(x-V.x, y-V.y); }
    Vec2<t> operator *(float f)          const { return Vec2<t>(x*f, y*f); }
    t& operator[](const int i) { return i<=0 ? x : y; }
    template <class > friend std::ostream& operator<<(std::ostream& s, Vec2<t>& v);
};

template <class t> struct Vec3 {
    t x, y, z;
    Vec3<t>() : x(t()), y(t()), z(t()) { }
    Vec3<t>(t _x, t _y, t _z) : x(_x), y(_y), z(_z) {}
    Vec3<t>(Matrix m);
    template <class u> Vec3<t>(const Vec3<u> &v);
    Vec3<t> operator ^(const Vec3<t> &v) const { return Vec3<t>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
    Vec3<t> operator +(const Vec3<t> &v) const { return Vec3<t>(x+v.x, y+v.y, z+v.z); }
    Vec3<t> operator -(const Vec3<t> &v) const { return Vec3<t>(x-v.x, y-v.y, z-v.z); }
    Vec3<t> operator *(float f)          const { return Vec3<t>(x*f, y*f, z*f); }
    t       operator *(const Vec3<t> &v) const { return x*v.x + y*v.y + z*v.z; }
    float norm () const { return std::sqrt(x*x+y*y+z*z); }
    float scalaire(Vec3 other) { return this->x*other.x + this->y * other.y + this->z * other.z; }
    Vec3<t> & normalize(t l=1) { *this = (*this)*(l/norm()); return *this; }
    t& operator[](const int i) { return i<=0 ? x : (1==i ? y : z); }
    template <class > friend std::ostream& operator<<(std::ostream& s, Vec3<t>& v);
};

typedef Vec2<float> Vec2f;
typedef Vec2<int>   Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int>   Vec3i;

template <> template <> Vec3<int>::Vec3(const Vec3<float> &v);
template <> template <> Vec3<float>::Vec3(const Vec3<int> &v);


template <class t> std::ostream& operator<<(std::ostream& s, Vec2<t>& v) {
    s << "(" << v.x << ", " << v.y << ")\n";
    return s;
}

template <class t> std::ostream& operator<<(std::ostream& s, Vec3<t>& v) {
    s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
    return s;
}*/

//////////////////////////////////////////////////////////////////////////////////////////////

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
