#ifndef _MODEL_H
#define _MODEL_H

#include <vector>
#include "geometry2.h"

using namespace std;

class Model{
public:
	Model(const char *file);

    vector<Pos_b> sommets;
    vector<Vec3f_b> norms;
    vector<vector<float> > uv;
    vector<vector<vector<int> > > faces;
};

#endif
