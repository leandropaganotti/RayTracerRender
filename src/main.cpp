#include <iostream>
#include <iomanip>
#include <thread>
#include <unistd.h>
#include <string>
#include <sstream>
#include "render.h"
#include "transformation.h"

using namespace std;

const char *xmlscene = NULL;// xml file with scene description
unsigned    nimages  = 1;   // generates nimages from difterent angles around y-axis

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

    Render render;
    Camera &camera = render;

    Scene scene;
    scene.load(xmlscene);

    camera.setOptions(scene.cameraOptions);

    const Vector3f from( scene.cameraOptions.getFrom() ), to( scene.cameraOptions.getTo());

    size_t spp = roundf(scene.spp/(4)) * 4;

    cout << "Render parameters:" << endl;
    cout << "- scene: " << xmlscene << endl;
    cout << "- #angles (y-axis): " << nimages << endl;
    cout << "- resolution: " << camera.getWidth() << "x" << camera.getHeight() << endl;
    cout << "- spp: " << spp << endl << endl;

    std::stringstream ss;
    ss << output;
    for (unsigned i=0; i < nimages; ++i)
    {
        cout << "\n" << i+1 << "/" << nimages << ": at " << std::fixed  << std::setw(6) <<  std::setprecision( 2 ) <<  i*angle << "°" << flush;
        camera.lookAt( Transformation::T(to) * Transformation::Ry(deg2rad( i*angle )) * Transformation::T(-to) * from, to); // rotate around y-axis

        auto start = chrono::steady_clock::now();       
        render.render(scene);
        auto end = chrono::steady_clock::now();

        time_in_ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        std::stringstream time_str ;
        time_str << timestamp2string(time_in_ms / 1000, "%T") << ":" << std::setw(3) << std::setfill('0') << (unsigned long long)time_in_ms % 1000;

        cout << ", Time: " << time_str.str() << flush;

        ss << "_IMG" << std::setw(4) << std::setfill('0') << i << "_SPP" << spp << "_T" << time_str.str() << ".ppm";
        render.getImage().save_ppm_bin(ss.str().c_str());

        time_in_ms_avg += time_in_ms;
    }

    time_in_ms_avg = time_in_ms_avg / (nimages);
    cout << ", Average in ms: " << time_in_ms_avg << endl;

    return 0;
}

void usage (char **argv)
{
    fprintf(stderr, "Usage: %s [-p nimages] xmlfile\n\n", argv[0]);
}

void parseArguments(int argc, char **argv)
{
    int opt;
    while ((opt = getopt(argc, argv, "p:")) != -1) {
        switch (opt) {
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
