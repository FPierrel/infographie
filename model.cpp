#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "model.h"
#include <stdio.h>
#include "geometry.h"

using namespace std;

Model::Model(const char *filename)
{
    string line;
	ifstream ifs;
	ifs.open(filename, ifstream::in);
	
	if (ifs.is_open())
	{
        while (getline(ifs, line))
        {
            if (!line.empty())
            {
                if (line.at(0) == 'v')
                {
                    char buf[1024];
                    strcpy(buf,line.c_str());

                    char * token[4];
                    token[0] = strtok(buf," ");

                    if (strcmp(token[0],"vn") == 0)
                    {
                        Vec3f v;
                        v.x = strtof(strtok(0, " "), NULL);
                        v.y = strtof(strtok(0, " "), NULL);
                        v.z = strtof(strtok(0, " "), NULL);
                        this->norms.push_back(v);
                    }
                    else if (strcmp(token[0],"vt") == 0)
                    {
                        vector<float> _uv;
                        _uv.push_back(strtof(strtok(0, " "), NULL));
                        _uv.push_back(strtof(strtok(0, " "), NULL));
                        uv.push_back(_uv);
                    }
                    else if (strcmp(token[0],"v") == 0)
                    {
                        Vec3f position;
                        position.x = strtof(strtok(0, " "), NULL);
                        position.y = strtof(strtok(0, " "), NULL);
                        position.z = strtof(strtok(0, " "), NULL);
                        this->sommets.push_back(position);
                    }
                }
                else if (!line.compare(0,2,"f "))
                {
                    char buf[1024];
                    vector<vector<int> > face;
                    strcpy(buf,line.c_str());
                    strtok(buf, " "); // f
                    for (int i = 0 ; i < 3 ; i++)
                    {
                        vector<int> triplet;
                        for (int j = 0 ; j < 2 ; j++)
                        {
                            triplet.push_back(atoi(strtok(0, "/")));
                        }
                        triplet.push_back(atoi(strtok(0, " ")));
                        face.push_back(triplet);
                    }
                    this->faces.push_back(face);
                }
            }

		}
		ifs.close();
	}
	else
		cout << "Lecture du model impossible" << endl;	

    load_texture(filename, "_diffuse.tga", diffusemap_);
    load_texture(filename, "_nm_tangent.tga",      normalmap_);
    load_texture(filename, "_spec.tga",    specularmap_);
}

void Model::load_texture(std::string filename, const char *suffix, TGAImage &img) {
    std::string texfile(filename);
    size_t dot = texfile.find_last_of(".");
    if (dot!=std::string::npos) {
        texfile = texfile.substr(0,dot) + std::string(suffix);
        std::cerr << "texture file " << texfile << " loading " << (img.read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
        img.flip_vertically();
    }
}

