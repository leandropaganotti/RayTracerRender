#include <iostream>
#include <iomanip>
#include <thread>
#include <unistd.h>
#include <string>
#include <sstream>
#include "raytracer.h"
#include "transformation.h"

using namespace std;

const char *xmlscene = NULL;// xml file with scene description
unsigned    nimages  = 1;   // generates nimages from difterent angles around y-axis
bool        detailedName = false;

void parseArguments(int argc, char **argv);

int main(int argc, char **argv)
{
    parseArguments(argc, argv);

    double time_in_ms_avg=0.0, time_in_ms=0.0;
    float angle = nimages ? 360.0 / nimages : 0.0;

    std::string output(xmlscene);

    //get the name of the file input (scene in xml file)
    output = output.substr(output.find_last_of("/")+1);
    output = output.substr(0, output.find_last_of("."));

    Scene scene(xmlscene);
    cout << scene << endl;

    RayTracer raytracer;
    Camera &camera = raytracer.getCamera();
    camera.setOptions(scene.cameraOptions);

    const Vector3 from( scene.cameraOptions.getFrom() ), to( scene.cameraOptions.getTo());

    size_t spp = roundf(scene.spp/(scene.grid*scene.grid)) * scene.grid * scene.grid;

    for (unsigned i=0; i < nimages; ++i)
    {
        cout << "\n" << i+1 << "/" << nimages << ": at " << std::fixed  << std::setw(6) <<  std::setprecision( 2 ) <<  i*angle << "°" << flush;
        camera.lookAt( Matrix4::T(to) * Matrix4::Ry(deg2rad( i*angle )) * Matrix4::T(-to) * from, to); // rotate around y-axis

        auto start = chrono::steady_clock::now();
        raytracer.render(scene);
        auto end = chrono::steady_clock::now();

        time_in_ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        std::stringstream time_str ;
        time_str << timestamp2string(time_in_ms / 1000, "%T") << ":" << std::setw(3) << std::setfill('0') << (unsigned long long)time_in_ms % 1000;

        cout << ", Time: " << time_str.str() << flush;

        std::stringstream ss;
        ss << output;
        ss << "_IMG" << std::setw(4) << std::setfill('0') << i;
        if (detailedName) ss << "_SPP" << spp << "_T" << time_str.str();
        ss << ".ppm";
        raytracer.getBuffer().save_ppm_bin(ss.str().c_str());

        time_in_ms_avg += time_in_ms;
    }

    time_in_ms_avg = time_in_ms_avg / (nimages);
    cout << ", Average in ms: " << time_in_ms_avg << endl;

    return 0;
}

void usage (char **argv)
{
    fprintf(stderr, "Usage: %s OPTIONS xmlfile\n\n", argv[0]);
    //[-p nimages] [-d]
    fprintf(stderr, "OPTIONS:\n");
    fprintf(stderr, "\t-p nimages : number of images to generate around y-axis\n");
    fprintf(stderr, "\t-d : detailed file name output with SPP and time for image creation\n");
}

void parseArguments(int argc, char **argv)
{
    int opt;
    while ((opt = getopt(argc, argv, "dp:")) != -1) {
        switch (opt) {
        case 'd':
            detailedName = true;
            break;
        case 'p':
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
