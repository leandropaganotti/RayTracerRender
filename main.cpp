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

    scene.load("/home/leandro/projects/Render/scenes/ballsOnTheMirror.xml");

    double avg=0;
    char buf[256];
    int n=36, i=0;
    float a= n?360.0/n:0;    

    Render render;

    Camera &camera = render.getCamera();

    camera.setOptions(scene.cameraOptions);

    const Vector3f from( scene.cameraOptions.getFrom() ), to( scene.cameraOptions.getTo());

    for (i=0; i <= n; ++i)
    {
        cout << endl << i << " " <<  i*a << " " << endl;
        auto start = chrono::steady_clock::now();
        camera.lookAt(Ry(deg2rad( i*a )) * from, to);
        render.render(scene, 6, 4);
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
