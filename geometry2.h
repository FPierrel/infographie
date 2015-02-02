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
    Pos(){}
    Pos(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    float x,y,z;
};

struct Pos_i{
    int x,y,z;
    Pos_i(int x, int y, int z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Pos_i operator -(Pos_i p)
    {
        return Pos_i(x-p.x, y-p.y, z-p.z);
    }
};
#endif
