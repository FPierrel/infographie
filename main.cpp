#include <cmath>
#include <iostream>
#include <climits>

#include "tgaimage.h"
#include "model.h"

using namespace std;

void line(TGAImage &image, int **zbuffer,
          int x1, int y1, int z1,
          int x2, int y2, int z2,
          TGAColor color1 = TGAColor(255, 1))
{
    float t;
    int  y, z;

    bool steep = false ;
    if((steep = abs(y2-y1)>abs(x2-x1))){
        swap(x1, y1);
        swap(x2, y2);
    }
    if(x2<x1) {
        swap(x1, x2);
        swap(y1, y2);
        swap(z1, z2);
    }

    for(int x=x1; x<=x2; x++){
        if (x2==x1) t = 1;
        else t = (x -x1)/(float)(x2-x1);

        y = (1.-t)*y1 + t*y2 ;
        z = (1.-t)*z1 + t*z2;

        if (x > 0 && y > 0 && x < image.get_width() && y < image.get_height() && zbuffer[x][y] < z)
        {
            if(steep)
                image.set(y,x,color1) ;
            else
                image.set(x,y,color1) ;

            zbuffer[x][y] = z;
        }
    }
}

void triangle_plein(TGAImage &image, int **zbuffer,
                                    int x1, int y1, int z1,
                                    int x2, int y2, int z2,
                                    int x3, int y3, int z3,
                                    TGAColor color)
{	
    float t,t2;
    int y, yb;
    int z, zb;

    if (x1 > x2){
        swap(x1, x2);
        swap(y1, y2);
        swap(z1, z2);
	}

    if (x1 > x3){
        swap(x1, x3);
        swap(y1, y3);
        swap(z1, z3);
	}
    		
    if (x2 > x3){
        swap(x2, x3);
        swap(y2, y3);
        swap(z2, z3);
    }
	
    for(int x=x1; x<=x2; x++){
        if(x1 == x2) t = 1;
        else t = (x-x1)/(float)(x2-x1);

        y = y1*(1.-t) + y2*t;
        z = z1*(1.-t) + z2*t;

        if (x1 == x3) t2 = 1;
        else t2 = (x-x1)/(float)(x3-x1);

        yb = y1*(1.-t2) + y3*t2;
        zb = z1*(1.-t2) + z3*t2;

        line(image, zbuffer, x, y, z, x, yb, zb, color);
	}

    for(int x=x2; x<=x3; x++){
        if (x2 == x3) t = 1;
        else t = (x-x2)/(float)(x3-x2);

        y = y2*(1.-t) + y3*t;
        z = z2*(1.-t) + z3*t;

        if (x3 == x1) t2 = 1;
        else t2 = (x-x1)/(float)(x3-x1);

        yb = y1*(1.-t2) + y3*t2;
        zb = z1*(1.-t2) + z3*t2;

        line(image, zbuffer, x, y, z, x, yb, zb, color);
	}	
	
}

void triangle(TGAImage &image, int x1, int y1, int x2, int y2, int x3, int y3)
{
    //line(image,x1,y1,x2,y2);
    //line(image,x2,y2,x3,y3);
    //line(image,x3,y3,x1,y1);
}

void rendu(TGAImage &image, Model model)
{
    vector<vector<int> > face;
    Vec3f lumiere(0,0, -1.);
    Vec3f vn1, vn2, vn3;

    int x1, x2, x3;
    int y1, y2, y3;
    int z1, z2, z3;
    int w = image.get_width();
    int h = image.get_height();
    int zw = (int)(0.4 * w);
    int zh = (int)(0.4 * h);

    int** zbuffer;
    zbuffer = new int*[w];
    for (int i = 0 ; i < w ;i++)
        zbuffer[i] = new int[h];
    for (int i = 0 ; i < w ; i++)
        for (int j = 0 ; j < h ; j++)
            zbuffer[i][j] = INT_MIN;

    float moyenne;
    for(unsigned int i = 0 ; i < model.faces.size() ; i++)
    {
        face = model.faces[i];
        x1 = (int)(model.sommets[face[0][0]-1].x*zw) + w/2;
        x2 = (int)(model.sommets[face[1][0]-1].x*zw) + w/2;
        x3 = (int)(model.sommets[face[2][0]-1].x*zw) + w/2;
        y1 = (int)(model.sommets[face[0][0]-1].y*zh) + h/2;
        y2 = (int)(model.sommets[face[1][0]-1].y*zh) + h/2;
        y3 = (int)(model.sommets[face[2][0]-1].y*zh) + h/2;
        z1 = (int)(model.sommets[face[0][0]-1].z*1024);
        z2 = (int)(model.sommets[face[1][0]-1].z*1024);
        z3 = (int)(model.sommets[face[2][0]-1].z*1024);

        vn1 = model.norms[face[0][2]-1];
        vn2 = model.norms[face[1][2]-1];
        vn3 = model.norms[face[2][2]-1];
        vn1.normalize();
        vn2.normalize();
        vn3.normalize();


        moyenne = (vn1.scalaire(lumiere)+vn2.scalaire(lumiere)+vn3.scalaire(lumiere))/3;

        triangle_plein(image, zbuffer, x1, y1, z1, x2, y2, z2, x3, y3, z3, TGAColor(moyenne*-255,1));
    }

    for (int i = 0 ; i < w ; i++)
        delete[] zbuffer[i];
    delete[] zbuffer;
}

void wireframe(TGAImage &image, Model model)
{
    vector<vector<int> > face;

    int x1, x2, x3;
    int y1, y2, y3;
    int w = image.get_width();
    int h = image.get_height();
    int zw = (int)(0.4 * w);
    int zh = (int)(0.4 * h);
    for(unsigned int i = 0 ; i < model.faces.size() ; i++)
    {
        face = model.faces[i];
        x1 = (int)(model.sommets[face[0][0]-1].x*zw) + w/2;
        x2 = (int)(model.sommets[face[1][0]-1].x*zw) + w/2;
        x3 = (int)(model.sommets[face[2][0]-1].x*zw) + w/2;
        y1 = (int)(model.sommets[face[0][0]-1].y*zh) + h/2;
        y2 = (int)(model.sommets[face[1][0]-1].y*zh) + h/2;
        y3 = (int)(model.sommets[face[2][0]-1].y*zh) + h/2;
        triangle(image, x1, y1, x2, y2, x3, y3);
    }
}


int main()
{
	Model model("head.obj");
    TGAImage image(1000, 1000, 1);

    wireframe(image, model);
    rendu(image,model);
    image.flip_vertically();
	image.write_tga_file("dump.tga");
	return 0;
}
