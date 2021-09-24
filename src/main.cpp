#include <iostream>
#include <iomanip>
#include <thread>
#include <string>
#include <cstring>
#include <sstream>
#include "raytracer.h"
#include "transformation.h"
#include "resource.h"
#include "material.h"
#include "objectvector.h"
#include "bvh.h"

char	    *xmlscene = NULL;	// xml file with scene description
unsigned    nimages  = 1;       // generates nimages from difterent angles around y-axis
bool        detailedName = false;

void parseArguments(int argc, char **argv);
int main(int argc, char **argv)
{
    parseArguments(argc, argv);

    double time_in_ms_avg=0.0, time_in_ms=0.0;
    float angle = nimages ? 360.0 / nimages : 0.0;

    std::string output(xmlscene);

    //get the name of the file input (scene in xml file)
    output = output.substr(output.find_last_of("/\\")+1);
    output = output.substr(0, output.find_last_of("."));

    Scene scene(xmlscene, std::make_shared<BVH>());
    std::cout << scene << std::endl;

    auto raytracer = RayTracer::Create(scene.renderOptions.illum);
    raytracer->setRenderOptions(scene.renderOptions);
    raytracer->setCameraOptions(scene.cameraOptions);

    Camera &camera = raytracer->getCameraRef();

    const Vector3 from( scene.cameraOptions.getFrom() ), to( scene.cameraOptions.getTo());

    for (unsigned i=0; i < nimages; ++i)
    {
        std::cout << "\n" << i+1 << "/" << nimages << ": at " << std::fixed  << std::setw(6) <<  std::setprecision( 2 ) <<  i*angle << " deg" << std::flush;
        camera.lookAt( Transform::T(to) * Transform::Ry(deg2rad( i*angle )) * Transform::T(-to) * from, to); // rotate around y-axis

        auto start = std::chrono::high_resolution_clock::now();
        raytracer->render(scene);
        auto end = std::chrono::high_resolution_clock::now();

        time_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        std::stringstream ss;
        int ms = (unsigned long long)time_in_ms % 1000;
        int s  = ((unsigned long long)time_in_ms / 1000) % 60;
        int m  = ((unsigned long long)time_in_ms / 1000 / 60) % 60;
        int h  = ((unsigned long long)time_in_ms / 1000 / 60 / 60);
        ss << std::setw(2) << std::setfill('0') << h << "." << std::setw(2) << std::setfill('0') << m << "." << std::setw(2) << std::setfill('0') << s << "." << std::setw(3) << std::setfill('0') << ms;
        std::string time_str = ss.str();
        std::cout << ", Time: " << time_str << std::endl << std::flush;

        std::stringstream ss2;
        ss2 << output << "_" << std::setw(4) << std::setfill('0') << i;
        if (detailedName) { ss2 << "_SPP" << scene.renderOptions.spp << "_T" << time_str; }
        raytracer->getBuffer().write_png(ss2.str().c_str());

        time_in_ms_avg += time_in_ms;
    }

    time_in_ms_avg = time_in_ms_avg / (nimages);
    std::cout << std::endl << "Total Average in ms: " << time_in_ms_avg << std::endl;

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
    if (argc < 2)
    {
        usage(argv);
        exit(0);
    }

    int opt = 1;
    while (opt < argc) {
        if (!strcmp(argv[opt], "-d"))
        {
            detailedName = true;
        }
        else if (!strcmp(argv[opt], "-p"))
        {
            ++opt;
            if (opt < argc)
            {
                nimages = atoi(argv[opt]);
            }
            else
            {
                usage(argv);
                exit(0);
            }
        }
        else
        {
            xmlscene = argv[opt];
        }
        ++opt;
    }
    if (!xmlscene)
    {
        usage(argv);
        exit(0);
    }
}
