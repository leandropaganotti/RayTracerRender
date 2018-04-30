#include <iostream>
#include "camera.h"
#include "scene.h"
#include "cube.h"
#include "sphere.h"
#include "utils.h"

using namespace std;

int main()
{

    Cube *cube = new Cube({1,1,0});

    cout << vertexBuffer << endl <<  *cube << endl;

    Scene scene;

    scene.backgroundColor   = {0.0f};





    scene.objects.push_back(new Plane());
    scene.objects.push_back(new Plane({0,0,-20}, {0,0,1}));

    scene.objects.push_back(new Sphere({2.5,0.5, -0.5}, 0.5, {1,0,0}));
    scene.objects.push_back(new Sphere({2.5,0.5, -2.5}, 0.5, {0,1,0}));
    scene.objects.push_back(new Sphere({0.5,0.5, -2.5}, 0.5, {0,0,1}));
    scene.objects.push_back(cube);

    Light light1;
    light1.color       = {1.0f};
    light1.origin      = Vector3f(2, 2 , 2);
    light1.strength   = 1.0f;
    light1.attenuation = 0.05f;

    Light light2;
    light2.color       = {1.0f};
    light2.origin      = Vector3f(2, 2 , -2);
    light2.strength   = 1.0f;
    light2.attenuation = 1.5f;


    scene.lights.push_back(light1);
   // scene.lights.push_back(light2);

    Camera camera;

    cout << camera << endl;

    double t1, t2;




        char buf[256];
        int n=36, i=0;

        float a=0;

        for (i=0; i < n; ++i)
        {

            Camera camera;
            cout << i << " " <<  a << " ";
            t1 =ms_time();
            camera.cameraToWorld = T({1.5, 1.5, -3}) * Ry(deg2rad( a )) * T({-1.5, -1.5, 3}) * T({1.5, 1.5, 6}) ;
            //camera.cameraToWorld = T({1.5, 1.5, 5}) ;
            camera.render1(scene );
            t2 =ms_time();

            sprintf(buf, "%04d.ppm", i);
            camera.frame().save_ppm_bin(buf);
            a = a + (360.0/n);

            cout << "time in ms: " << t2-t1 << endl;
        }





    return 0;
}

