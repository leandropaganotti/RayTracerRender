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
    scene.bgColor = {0.0f};

    scene.addObject(new Plane({0,0,0}, {0,1,0}));
    scene.addObject(new Sphere({2.5,0.5, -0.5}, 0.5, {1,0,0}));
    scene.addObject(new Sphere({2.5,0.5, -2.5}, 0.5, {0,1,0}));
    scene.addObject(new Sphere({0.5,0.5, -2.5}, 0.5, {0,0,1}));

    scene.addObject(new Sphere({0, 0.5, 2}, 0.5, {1,0,1}));
    scene.addObject(new Sphere({-3.5,0.5, 2}, 0.5, {1,1,0}));
    scene.addObject(new Sphere({3,0.5, 3}, 0.5, {0,1,1}));

    Cube *cube = new Cube({1,1,0});
    scene.addObject(cube);


//    Light light2;
//    light2.color       = {1.0f};
//    light2.origin      = Vector3f(2, 2 , -2);
//    light2.strength    = 1.0f;
//    light2.attenuation = 1.5f;


    scene.addLight(new PointLight(Vector3f(2,2,2)));

    //scene.lights.push_back(new DistantLight(-Vector3f(1,1,-1).normalize(), {1}, 0.03));

    //scene.lights.push_back(light2);

    cout << *cube << endl;

    double t1, t2, avg=0;
    char buf[256];
    int n=36, i=0;

    float a=360.0/n;

    Camera camera;
    Vector3f from(0, 5, 10);
    Vector3f to(0,0,0);

    for (i=0; i <= n; ++i)
    {
        // new camera position


        camera.lookAt(Ry(deg2rad( a )) * from, to);

        t1 =ms_time();
        camera.render(scene, 1, 4);

        t2 =ms_time();

        cout << i << " " <<  i*a << " " << camera.position();
        avg += t2-t1;
        cout << " time in ms: " << t2-t1 << endl << endl;

        sprintf(buf, "%04d.ppm", i);
        camera.frame().save_ppm_bin(buf);
    }
    avg /=n;
    cout << "avg time in ms: " << avg << endl;

    return 0;
}

