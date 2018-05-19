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
	// Hard coded scene
    Scene scene;

    scene.bgColor = 0.0f;

    Plane *plane = new Plane({0,0,0}, {0,1,0}, {0.482, 0.408, 0.933});
    plane->reflectivity = 0.5f;
    scene.addObject(plane);

    Plane *p1 = new Plane({0,0,-20}, {0,0,1}, {0.0, 0.0, 0.0});
    p1->reflectivity = 0.7f;
    p1->k_specular = 0;
    scene.addObject(p1);

    Sphere *sphere = new Sphere({-2.5,1.0, -4.0}, 1.0, {0.824, 0.412, 0.118});
	sphere->reflectivity = 1.0f;
	scene.addObject(sphere);

    Cube *cube = new Cube( Vector3f(1.5, 0.5, -2.5), Vector3f(1, 1.5, 1), Vector3f(0, 30, 0), Vector3f(1,1,0));
    cube->reflectivity = 0.0;
	//cube->shininess = 15;
    //cube->c_specular = Vector3f(1, 0, 0);
    scene.addObject(cube);
	//cout << *cube << endl;

    scene.addObject(new Sphere({3.5,0.5, -0.5}, 0.5, {1,0,0}));
    scene.addObject(new Sphere({2.5,0.5, -3.5}, 0.5, {0,1,0}));
    scene.addObject(new Sphere({1.5,0.5, -5.5}, 0.5, {1,1,0}));
    scene.addObject(new Sphere({0, 0.5, 4}, 0.5, {1,0,1}));
    scene.addObject(new Sphere({-2.5,0.5, -1}, 0.5, {0,0,1}));
    scene.addObject(new Sphere({3.5,0.5, -0.5}, 0.5, {1,0,0}));
    scene.addObject(new Sphere({3,0.5, 3}, 0.5, {0,1,1}));

    scene.addLight(new PointLight(Vector3f(2,3,2), 1, 1.0, 0));

    double avg=0;
    char buf[256];
    int n=36, i=0;
    float a= n?360.0/n:0;

    /**
     * Starts here
     * */

    Scene scene1;

    scene1.load("/home/leandro/projects/Render/scenes/balls.xml");

    Render render;

    Camera &camera = render.getCamera();

    camera.setOptions(scene1.cameraOptions);

    const Vector3f from( scene1.cameraOptions.getFrom() ), to( scene1.cameraOptions.getTo() );

    for (i=0; i <= n; ++i)
    {
        cout << endl << i << " " <<  i*a << " " << render.getCamera().getPosition() << endl;
        auto start = chrono::steady_clock::now();
        camera.lookAt(Ry(deg2rad( i*a )) * from, to);
        render.render(scene1, 1, 4);
        auto end = chrono::steady_clock::now();

        sprintf(buf, "%04d.ppm", i);
        render.getImage().save_ppm_bin(buf);

        cout << "Elapsed time in milliseconds : "
//        		<< chrono::duration_cast<chrono::nanoseconds>(end - start).count()  << " ns" << endl
//				<< chrono::duration_cast<chrono::microseconds>(end - start).count() << " µs" << endl
				<< chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl
//              << chrono::duration_cast<chrono::seconds>(end - start).count() << " sec"
				<< endl;

        avg += chrono::duration_cast<chrono::milliseconds>(end - start).count();
    }

    avg = avg / (n+1);
    cout << endl << "Elapsed average time in milliseconds : " << avg << endl;

    return 0;
}
