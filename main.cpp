#include <iostream>
#include "camera.h"
#include "scene.h"
#include "cube.h"
#include "sphere.h"
#include "utils.h"

#include <thread>

using namespace std;

int main()
{

    Scene scene;
    scene.backgroundColor = {0.0f};

    scene.objects.push_back(new Plane({0,-0.5,0}, {0,1,0}));

    scene.objects.push_back(new Sphere({2.5,0.5, -0.5}, 0.5, {1,0,0}));
    scene.objects.push_back(new Sphere({2.5,0.5, -2.5}, 0.5, {0,1,0}));
    scene.objects.push_back(new Sphere({0.5,0.5, -2.5}, 0.5, {0,0,1}));

    Cube *cube = new Cube({1,1,0});
    scene.objects.push_back(cube);

    Light light1;
    light1.color       = {1.0f};
    light1.origin      = Vector3f(2, 2 , 2);
    light1.strength    = 1.0f;
    light1.attenuation = 0.05f;

    Light light2;
    light2.color       = {1.0f};
    light2.origin      = Vector3f(2, 2 , -2);
    light2.strength    = 1.0f;
    light2.attenuation = 1.5f;


    scene.lights.push_back(light1);
    //scene.lights.push_back(light2);

    cout << vertexBuffer << endl <<  *cube << endl;

    double t1, t2, avg=0;
    char buf[256];
    int n=36, i=0;

    float a=360.0/n;

    Camera camera;
    Vector3f origin = {0, 0, 10};

    cout <<  Ry(deg2rad( a )) ;

    for (i=0; i <= n; ++i)
    {
        // new camera position
        camera.cameraToWorld =  Ry(deg2rad( i*a )) ;
        camera.origin = camera.cameraToWorld * origin;

        t1 =ms_time();
        camera.render(scene, 1, 4);

        t2 =ms_time();

        cout << i << " " <<  i*a << " " << camera.origin;
        avg += t2-t1;
        cout << " time in ms: " << t2-t1 << endl;

        sprintf(buf, "%04d.ppm", i);
        camera.frame().save_ppm_bin(buf);
    }
    avg /=n;
    cout << "avg time in ms: " << avg << endl;
    return 0;
}

