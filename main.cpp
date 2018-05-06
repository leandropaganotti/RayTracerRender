#include <iostream>
#include "render.h"
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
    cout << *cube << endl;

    scene.addLight(new PointLight(Vector3f(2,2,2)));


    double avg=0;
    char buf[256];
    int n=36, i=0;

    float a=360.0/n;

    Vector3f from(0, 5, 10);
    Vector3f to(0,0,0);

    Render render;

    for (i=0; i <= n; ++i)
    {
        render.camera().lookAt(Ry(deg2rad( a )) * from, to);

        cout << endl << i << " " <<  i*a << " " << render.camera().position() << endl;

        auto start = chrono::steady_clock::now();
        render.render(scene, 640, 480);
        auto end = chrono::steady_clock::now();

        sprintf(buf, "%04d.ppm", i);
        render.frame().save_ppm_bin(buf);

//        cout << "Elapsed time in nanoseconds : "
//            << chrono::duration_cast<chrono::nanoseconds>(end - start).count()
//            << " ns" << endl;

//        cout << "Elapsed time in microseconds : "
//            << chrono::duration_cast<chrono::microseconds>(end - start).count()
//            << " µs" << endl;

        cout << "Elapsed time in milliseconds : "
            << chrono::duration_cast<chrono::milliseconds>(end - start).count()
            << " ms" << endl;

//        cout << "Elapsed time in seconds : "
//            << chrono::duration_cast<chrono::seconds>(end - start).count()
//            << " sec" << endl;

        avg += chrono::duration_cast<chrono::milliseconds>(end - start).count();
    }

    avg = avg / (n+1);
    cout << endl << "Elapsed average time in milliseconds : " << avg << endl;

    return 0;
}
