#ifndef _GEOMETRY_H
#define _GEOMETRY_H

class Vec3f{
public:
    float x;
    float y;
    float z;

    Vec3f(float x, float y, float z);
    Vec3f();

    float norm();
    float scalaire(Vec3f other);
    Vec3f* normalize(float l = 1);
};

struct Pos{
    float x,y,z;
};

struct Pos_i{
    int x,y,z;
};

#endif
