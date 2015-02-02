#ifndef _MODEL_H
#define _MODEL_H

#include <vector>
#include "geometry2.h"

using namespace std;

class Model{
public:
	Model(const char *file);

    vector<Pos> sommets;
    vector<Vec3f> norms;
    vector<vector<vector<int> > > faces;
};

#endif
