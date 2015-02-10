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

struct Vec2f_b{
    float x;
    float y;
};


struct Pos_b{
    Pos_b(){}
    Pos_b(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    float x,y,z;
};

struct Pos_i_b{
    int x,y,z;
    Pos_i_b(int x, int y, int z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Pos_i_b operator -(Pos_i_b p)
    {
        return Pos_i_b(x-p.x, y-p.y, z-p.z);
    }
};
#endif
