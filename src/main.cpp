#include <iostream>
#include <thread>
#include <unistd.h>
#include <string>

#include "render.h"

using namespace std;

const char *xmlscene = 0;   // xml file with scene description
unsigned    nthreads = 4;   // number of threads to run
unsigned    nrays    = 1;   // nrays * nrays rays will be cast for each pixel

void parseArguments(int argc, char **argv);

int main(int argc, char **argv)
{
    parseArguments(argc, argv);

    Scene scene;

    scene.load(xmlscene);

    double avg=0;
    char buf[256];
    int n=0, i=0;
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
        render.render(scene, nrays, nthreads);
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
    cout << endl << "Elapsed time in milliseconds (average): " << avg << endl;

    return 0;
}

void parseArguments(int argc, char **argv)
{
    int opt;
    while ((opt = getopt(argc, argv, "r:t:")) != -1) {
        switch (opt) {
        case 'r':
            nrays = atoi(optarg);
            break;
        case 't':
            nthreads = atoi(optarg);
            break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s [-t nthreads] [-r nrays] xmlfile\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

   if (optind >= argc) {
        fprintf(stderr, "Usage: %s [-t nthreads] [-r nrays] xmlfile\n", argv[0]);
        exit(EXIT_FAILURE);
    }
   xmlscene = argv[optind];

   printf("Arguments:\n");
   printf(" xmlfile: %s\n", xmlscene);
   printf(" nrays: %d\n", nrays);
   printf(" nthreads: %d\n", nthreads);
}
