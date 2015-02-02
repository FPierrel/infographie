#include <cmath>
#include <iostream>
#include <climits>

#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include "geometry2.h"

using namespace std;

Matrix ModelView;
Matrix Viewport;
Matrix Projection;

void viewport(int x, int y, int w, int h)
{
    Viewport = Matrix::identity();
    Viewport[0][3] = x+w/2.f;
    Viewport[1][3] = y+h/2.f;
    Viewport[2][3] = 1.f;
    Viewport[0][0] = w/2.f;
    Viewport[1][1] = h/2.f;
    Viewport[2][2] = 0;
}

void projection(float coeff){
    Projection = Matrix::identity();
    Projection[3][2] = coeff;
}

void lookat(Vec3f eye, Vec3f center, Vec3f up){
    Vec3f z = (eye-center).normalize();
    Vec3f x = cross(up,z).normalize();
    Vec3f y = cross(z,x).normalize();
    ModelView = Matrix::identity();
    for (int i = 0 ; i < 3 ; i++){
        ModelView[0][i] = x[i];
        ModelView[1][i] = y[i];
        ModelView[2][i] = z[i];
        ModelView[i][3] = -center[i];
    }
}

void triangle_plein(TGAImage &image, int **zbuffer,Pos_b pos_1, Pos_b pos_2, Pos_b pos_3,
                                    TGAColor color1 = TGAColor(255, 1),
                                    TGAColor color2 = TGAColor(255, 1 ),
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
                    //Calcul coordonée barycentric
                    // (http://imagine.inrialpes.fr/people/Damien.Rohmer/documents/teaching/11_1spring_cpe/4eti_mso_synthese/tp/tp_2_projectif.pdf)
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
    Vec3f_b lumiere(0.f,0.f,-1.f);
    Vec3f_b vn1, vn2, vn3;
    Pos_b pos_1, pos_2, pos_3;

    Vec3f eye(1,1,3);
    Vec3f center(0,0,0);
    Vec3f up(0,1,0);

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
