#ifndef _MODEL_H
#define _MODEL_H

#include <vector>
#include "geometry2.h"
#include "tgaimage.h"
#include "geometry.h"

using namespace std;

class Model{
public:
	Model(const char *file);
    void load_texture(std::string filename, const char *suffix, TGAImage &img);

    vector<Pos_f> sommets;
    vector<Pos_f> norms;
    vector<vector<float> > uv;
    vector<vector<vector<int> > > faces;
    TGAImage diffusemap_;
    TGAImage normalmap_;
    TGAImage specularmap_;
};

#endif
