#include <iostream>
#include <thread>
#include <unistd.h>
#include <string>

#include "render.h"

using namespace std;

const char *xmlscene = NULL;// xml file with scene description
unsigned    nthreads = 4;   // number of threads to run
unsigned    nrays    = 1;   // nrays * nrays rays will be cast for each pixel
unsigned    panoramic= 0;   // generates 36 images from difterent angles around y-axis

void parseArguments(int argc, char **argv);

int main(int argc, char **argv)
{
    parseArguments(argc, argv);

    Scene scene;

    scene.load(xmlscene);

    double averageTime=0.0, elapsedTime=0.0;
    char buf[256];
    float angle = panoramic ? 360.0 / panoramic : 0.0;

    Render render;

    Camera &camera = render.getCamera();

    camera.setOptions(scene.cameraOptions);

    const Vector3f from( scene.cameraOptions.getFrom() ), to( scene.cameraOptions.getTo());

    for (unsigned i=0; i <= panoramic; ++i)
    {
        cout << endl << i << " " <<  i*angle << " " << endl;
        camera.lookAt( T(to) * Ry(deg2rad( i*angle )) * T(-to) * from, to); // rotate around y-axis

        auto start = chrono::steady_clock::now();       
        render.render(scene, nrays, nthreads);
        auto end = chrono::steady_clock::now();

        elapsedTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "Elapsed time in milliseconds : " << elapsedTime << " ms" << endl;

        sprintf(buf, "%s.%04d.ppm", scene.id.c_str(), i);
        render.getImage().save_ppm_bin(buf);

        averageTime += chrono::duration_cast<chrono::milliseconds>(end - start).count();
    }

    averageTime = averageTime / (panoramic + 1);
    cout << endl << "Elapsed time in milliseconds (average): " << averageTime << "ms" << endl;

    return 0;
}

void usage (char **argv)
{
    fprintf(stderr, "Usage: %s [-t nThreads] [-r nRays] [-p] xmlfile\n\n", argv[0]);
}

void parseArguments(int argc, char **argv)
{
    int opt;
    while ((opt = getopt(argc, argv, "r:t:p")) != -1) {
        switch (opt) {
        case 'r':
            nrays = atoi(optarg);
            break;
        case 't':
            nthreads = atoi(optarg);
            break;
        case 'p':
            panoramic = 36;
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

   printf("Arguments:\n");
   printf(" xmlfile: %s\n", xmlscene);
   printf(" nrays: %d\n", nrays);
   printf(" nthreads: %d\n", nthreads);
}
