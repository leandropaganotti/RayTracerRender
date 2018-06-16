#include <iostream>
#include <iomanip>
#include <thread>
#include <unistd.h>
#include <string>

#include "render.h"

using namespace std;

const char *xmlscene = NULL;// xml file with scene description
unsigned    nrays    = 1;   // nrays * nrays rays will be cast for each pixel
unsigned    nimages  = 1;   // generates nimages from difterent angles around y-axis

void parseArguments(int argc, char **argv);

int main(int argc, char **argv)
{
    parseArguments(argc, argv);

    Scene scene;

    scene.load(xmlscene);

    double averageTime=0.0, elapsedTime=0.0;
    char buf[256];
    float angle = nimages ? 360.0 / nimages : 0.0;

    Render render;

    Camera &camera = render.getCamera();

    camera.setOptions(scene.cameraOptions);

    const Vector3f from( scene.cameraOptions.getFrom() ), to( scene.cameraOptions.getTo());

    cout << "Render parameters:" << endl;
    cout << "- scene file: " << xmlscene << endl;
    cout << "- # of image around y-axis: " << nimages << endl;
    cout << "- image output resolution: " << camera.getWidth() << "x" << camera.getHeight() << endl;
    cout << "- # of threads: " << camera.getHeight() << endl;

    for (unsigned i=0; i < nimages; ++i)
    {
        cout << "\r" << i+1 << "/" << nimages << ": at " << std::fixed  << std::setw(6) <<  std::setprecision( 2 ) <<  i*angle << "° => ";
        camera.lookAt( T(to) * Ry(deg2rad( i*angle )) * T(-to) * from, to); // rotate around y-axis

        auto start = chrono::steady_clock::now();       
        render.render(scene, nrays, camera.getHeight());
        auto end = chrono::steady_clock::now();

        elapsedTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "Time: " << elapsedTime << " ms" << flush;

        sprintf(buf, "%s_%04d.ppm", scene.name.c_str(), i);
        render.getImage().save_ppm_bin(buf);

        averageTime += chrono::duration_cast<chrono::milliseconds>(end - start).count();
    }

    averageTime = averageTime / (nimages);
    cout << endl << "Average time: " << averageTime << " ms" << endl;

    return 0;
}

void usage (char **argv)
{
    fprintf(stderr, "Usage: %s [-r nrays] [-i nimages] xmlfile\n\n", argv[0]);
}

void parseArguments(int argc, char **argv)
{
    int opt;
    while ((opt = getopt(argc, argv, "r:i:")) != -1) {
        switch (opt) {
        case 'r':
            nrays = atoi(optarg);
            break;
        case 'i':
            nimages = atoi(optarg);
            break;
        default: /* '?' */
            usage(argv);
            exit(EXIT_FAILURE);
        }
    }

   if (optind >= argc) {
        usage(argv);
        exit(EXIT_FAILURE);
    }
   xmlscene = argv[optind];
}
