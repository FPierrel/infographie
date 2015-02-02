#include <cmath>
#include <iostream>
#include <climits>

#include "tgaimage.h"
#include "model.h"

using namespace std;

void line(TGAImage &image, int **zbuffer,
          float x1, float y1, float z1,
          float x2, float y2, float z2,
          TGAColor color1 = TGAColor(255, 1))
{
/*    float t, y;
    float z;

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
        else t = (float)(x -x1)/(float)(x2-x1);

        y = (1.-t)*y1 + t*y2 ;
        z = (1.-t)*z1 + t*z2;

        if (x > 0 && y > 0 && x < image.get_width() && y < image.get_height() && zbuffer[x][y] < (int)z*1024)
        {
            if(steep)
                image.set((int)y,x,color1) ;
            else
                image.set(x,(int)y,color1) ;

            zbuffer[x][y] = z;
        }
    }*/
}

void triangle_plein(TGAImage &image, int **zbuffer,Pos pos_1, Pos pos_2, Pos pos_3,
                                    TGAColor color1 = TGAColor(255, 1),
                                    TGAColor color2 = TGAColor(255,1 ),
                                    TGAColor color3 = TGAColor(255, 1))
{
    float min_x = min(pos_1.x, min(pos_2.x, pos_3.x));
    float max_x = max(pos_1.x, max(pos_2.x, pos_3.x));
    float min_y = min(pos_1.y, min(pos_2.y, pos_3.y));
    float max_y = max(pos_1.y, max(pos_2.y, pos_3.y));
    char c;
    float u,v,w;

    int xn, yn, zn;
    float p1, p2, p3;
    int z;

    for (int x = min_x ; x <= max_x ; x++)
        for(int y = min_y ; y <= max_y ; y++)
        {
            p1 = pos_1.x * (pos_2.y-y) + pos_2.x * (y - pos_1.y) + x * (pos_1.y - pos_2.y);
            p2 = pos_2.x * (pos_3.y-y) + pos_3.x * (y - pos_2.y) + x * (pos_2.y - pos_3.y);
            p3 = pos_3.x * (pos_1.y-y) + pos_1.x * (y - pos_3.y) + x * (pos_3.y - pos_1.y);

            if ((p1 > 0 && p2 > 0 && p3 > 0) || (p1 < 0 && p2 < 0 && p3 < 0))
            {
                xn=(pos_2.y-pos_1.y)*(pos_3.z-pos_1.z) - (pos_3.y-pos_1.y)*(pos_2.z-pos_1.z);
                yn=(pos_3.x-pos_1.x)*(pos_2.z-pos_1.z) - (pos_2.x-pos_1.x)*(pos_3.z-pos_1.z);
                zn=(pos_2.x-pos_1.x)*(pos_3.y-pos_1.y) - (pos_3.x-pos_1.x)*(pos_2.y-pos_1.y);

                z = (pos_1.x*xn + pos_1.y*yn + pos_1.z*zn - x*xn - y*yn) / zn;

                if ( z > zbuffer[x][y])
                {
                    u = ((x-pos_3.x)*(pos_2.y-pos_3.y)-(pos_2.x-pos_3.x)*(y-pos_3.y))/
                        ((pos_1.x-pos_3.x)*(pos_2.y-pos_3.y)-(pos_2.x-pos_3.x)*(pos_1.y-pos_3.y));

                    v = ((x-pos_3.x)*(pos_3.y-pos_1.y)-(pos_3.x-pos_1.x)*(y-pos_3.y))/
                        ((pos_1.x-pos_3.x)*(pos_2.y-pos_3.y)-(pos_2.x-pos_3.x)*(pos_1.y-pos_3.y));

                    w = ((x-pos_2.x)*(pos_1.y-pos_2.y)-(pos_1.x-pos_2.x)*(y-pos_2.y))/
                        ((pos_1.x-pos_3.x)*(pos_2.y-pos_3.y)-(pos_2.x-pos_3.x)*(pos_1.y-pos_3.y));

                    c = u*color1.val + v*color2.val + w*color3.val;
                    image.set(x,y,TGAColor(c,1));
                    zbuffer[x][y] = z;
                }
            }
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
    Vec3f lumiere(0.,0., -1.);
    Vec3f vn1, vn2, vn3;
    Pos pos_1, pos_2, pos_3;

    int w = image.get_width();
    int h = image.get_height();
    float zw = (0.5 * w);
    float zh = (0.5 * h);

    int** zbuffer;
    zbuffer = new int*[w];
    for (int i = 0 ; i < w ;i++)
        zbuffer[i] = new int[h];
    for (int i = 0 ; i < w ; i++)
        for (int j = 0 ; j < h ; j++)
            zbuffer[i][j] = INT_MIN;

    float lum1, lum2, lum3;
    for(unsigned int i = 0 ; i < model.faces.size() ; i++)
    {
        face = model.faces[i];
        pos_1.x = (model.sommets[face[0][0]-1].x*zw) + w/2;
        pos_2.x = (model.sommets[face[1][0]-1].x*zw) + w/2;
        pos_3.x = (model.sommets[face[2][0]-1].x*zw) + w/2;
        pos_1.y = (model.sommets[face[0][0]-1].y*zh) + h/2;
        pos_2.y = (model.sommets[face[1][0]-1].y*zh) + h/2;
        pos_3.y = (model.sommets[face[2][0]-1].y*zh) + h/2;
        pos_1.z = (model.sommets[face[0][0]-1].z*2048);
        pos_2.z = (model.sommets[face[1][0]-1].z*2048);
        pos_3.z = (model.sommets[face[2][0]-1].z*2048);

        vn1 = model.norms[face[0][2]-1];
        vn2 = model.norms[face[1][2]-1];
        vn3 = model.norms[face[2][2]-1];
        lumiere.normalize();
        vn1.normalize();
        vn2.normalize();
        vn3.normalize();

        lum1 = -vn1.scalaire(lumiere);
        if (lum1 > 1.0f) lum1= 1.0f;
        else if (lum1 < 0.f) lum1 = 0.0f;

        lum2 = -vn2.scalaire(lumiere);
        if (lum2 > 1.0f) lum2= 1.0f;
        else if (lum2 < 0.f) lum2 = 0.0f;

        lum3 = -vn3.scalaire(lumiere);
        if (lum3 > 1.0f) lum3= 1.0f;
        else if (lum3 < 0.f) lum3 = 0.0f;

        triangle_plein(image, zbuffer, pos_1,pos_2,pos_3, TGAColor(lum1*255,1),
                                                          TGAColor(lum2*255,1),
                                                          TGAColor(lum3*255,1));
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
