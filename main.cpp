#include <cmath>
#include <iostream>
#include <climits>

#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

//http://habrahabr.ru/post/248723/

using namespace std;

int depth = 2048;

Matrix viewport(int x, int y, int w, int h)
{
    Matrix Viewport = Matrix::identity(4);
    Viewport[0][3] = x+w/2.f;
    Viewport[1][3] = y+h/2.f;
    Viewport[2][3] = depth/2.f;
    Viewport[0][0] = w/2.f;
    Viewport[1][1] = h/2.f;
    Viewport[2][2] = depth/2.f;

    return Viewport;
}

Matrix lookat(Vec3f eye, Vec3f center, Vec3f up){
    Vec3f z = (*(eye-center).normalize());
    Vec3f x = (*(up^z).normalize());
    Vec3f y = (*(z^x).normalize());
    Matrix res = Matrix::identity(4);
    for (int i = 0 ; i < 3 ; i++){
        res[0][i] = x[i];
        res[1][i] = y[i];
        res[2][i] = z[i];
        res[i][3] = -center[i];
    }
    return res;
}

void triangle_plein(TGAImage &image, Model *model,int **zbuffer,Vec3f pos_1, Vec3f pos_2, Vec3f pos_3,
                                    vector<float> uv1,
                                    vector<float> uv2,
                                    vector<float> uv3,
                                    TGAColor color1 = TGAColor(255, 1),
                                    TGAColor color2 = TGAColor(255, 1 ),
                                    TGAColor color3 = TGAColor(255, 1))
{
    float min_x = min(pos_1.x, min(pos_2.x, pos_3.x));
    float max_x = max(pos_1.x, max(pos_2.x, pos_3.x));
    float min_y = min(pos_1.y, min(pos_2.y, pos_3.y));
    float max_y = max(pos_1.y, max(pos_2.y, pos_3.y));
    float u,v,w;

    int xn, yn, zn;
    float p1, p2, p3;
    int z;

    for (int x = min_x ; x <= max_x ; x++)
        for(int y = min_y ; y <= max_y ; y++)
            if (x>0 && x < image.get_width() && y > 0 && y < image.get_height())
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

                    float xt = u*uv1[0] + v*uv2[0] + w*uv3[0];
                    float yt = u*uv1[1] + v*uv2[1] + w*uv3[1];

                    image.set(x,y,model->diffusemap_.get(xt*model->diffusemap_.get_width(),
                                                         yt*model->diffusemap_.get_height()));
                    zbuffer[x][y] = z;
                }
            }
        }
}

void rendu(TGAImage &image, Model model)
{
    int w = image.get_width();
    int h = image.get_height();
    float lum1, lum2, lum3;

    vector<vector<int> > face;
    Vec3f lumiere(0.f,0.f,-1.f);
    Vec3f vn1, vn2, vn3;
    vector<float> uv1, uv2, uv3;
    Vec3f pos_1, pos_2, pos_3;

    Vec3f eye(0,0,1);
    Vec3f center(0,0,0);
    Vec3f up(0,1,0);

    Matrix ModelView(lookat(eye,center,Vec3f(0,1,0)));
    Matrix Projection = Matrix::identity(4);
    Matrix ViewPort   = viewport(w/8, h/8, w*3/4, h*3/4);
    Projection[2][3] = -1.f/(eye-center).norm();

    int** zbuffer;
    zbuffer = new int*[w];
    for (int i = 0 ; i < w ;i++)
        zbuffer[i] = new int[h];
    for (int i = 0 ; i < w ; i++)
        for (int j = 0 ; j < h ; j++)
            zbuffer[i][j] = INT_MIN;


    for(unsigned int i = 0 ; i < model.faces.size() ; i++)
    {
        face = model.faces[i];

        Matrix facette(4,4);
        facette[0][0] = model.sommets[face[0][0]-1].x + 1;
        facette[1][0] = model.sommets[face[0][0]-1].y + 1;
        facette[2][0] = model.sommets[face[0][0]-1].z;
        facette[0][1] = model.sommets[face[1][0]-1].x + 1;
        facette[1][1] = model.sommets[face[1][0]-1].y + 1;
        facette[2][1] = model.sommets[face[1][0]-1].z;
        facette[0][2] = model.sommets[face[2][0]-1].x + 1;
        facette[1][2] = model.sommets[face[2][0]-1].y + 1;
        facette[2][2] = model.sommets[face[2][0]-1].z;

        facette = ViewPort*Projection*ModelView*facette;

        pos_1.x = facette[0][0];
        pos_2.x = facette[0][1];
        pos_3.x = facette[0][2];
        pos_1.y = facette[1][0];
        pos_2.y = facette[1][1];
        pos_3.y = facette[1][2];
        pos_1.z = facette[2][0];
        pos_2.z = facette[2][1];
        pos_3.z = facette[2][2];

        uv1 = model.uv[face[0][1]-1];
        uv2 = model.uv[face[1][1]-1];
        uv3 = model.uv[face[2][1]-1];

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

        triangle_plein(image, &model, zbuffer, pos_1,pos_2,pos_3, uv1, uv2, uv3,
                                                          TGAColor  (lum1*255,1),
                                                          TGAColor(lum2*255,1),
                                                          TGAColor(lum3*255,1));
    }

    for (int i = 0 ; i < w ; i++)
        delete[] zbuffer[i];
    delete[] zbuffer;
}

int main()
{
    Model model("african_head.obj");
    TGAImage image(1024, 1024, TGAImage::RGB);

    rendu(image,model);
    image.flip_vertically();
    image.write_tga_file("dump.tga");
    return 0;
}
