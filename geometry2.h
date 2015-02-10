#ifndef _GEOMETRY_H
#define _GEOMETRY_H

class Vec3f_b{
public:
    float x;
    float y;
    float z;

    Vec3f_b(float x, float y, float z);
    Vec3f_b();

    float norm();
    float scalaire(Vec3f_b other);
    Vec3f_b* normalize(float l = 1);
};

template<class t> struct Pos
{
    Pos(){}
    Pos(t x, t y, t z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Pos operator -(Pos p){
        return Pos<t>(this->x - p.x, this->y - p.y, this->z - p.z);
    }

    Pos operator +(Pos p){
        return Pos<t>(this->x + p.x, this->y + p.y, this->z + p.z);
    }

    t x,y,z;
};

typedef Pos<int> Pos_i;
typedef Pos<float> Pos_f;
#endif
