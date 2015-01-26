#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "model.h"
#include "geometry.h"
#include <stdio.h>

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
                    else if (strcmp(token[0],"v") == 0)
                    {
                        Pos position;
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
}


