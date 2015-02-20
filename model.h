#ifndef _MODEL_H
#define _MODEL_H

#include <vector>
#include "tgaimage.h"
#include "geometry.h"

using namespace std;

class Model{
public:
	Model(const char *file);
    void load_texture(std::string filename, const char *suffix, TGAImage &img);

    vector<Vec3f> sommets;
    vector<Vec3f> norms;
    vector<vector<float> > uv;
    vector<vector<vector<int> > > faces;
    TGAImage diffusemap_;
    TGAImage normalmap_;
    TGAImage specularmap_;
};

#endif
