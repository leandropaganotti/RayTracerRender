#include <iostream>
#include <iomanip>
#include <thread>
#include <string>
#include <cstring>
#include <sstream>
#include "transformation.h"
#include "scene.h"
#include "camera.h"

std::string	sceneFileName;	// xml file with scene description
unsigned    nImages  = 1;       // generates nimages from difterent angles around y-axis
bool        detailedName = false;

void parseArguments(int argc, char **argv);

int main(int argc, char **argv)
{
    parseArguments(argc, argv);

    double avgTime=0.0, totalTime=0.0;
    float angle = nImages ? 360.0 / nImages : 0.0;

    //get the name of the file input (scene in xml file)
    std::string outputFileName(sceneFileName);
    outputFileName = outputFileName.substr(outputFileName.find_last_of("/\\")+1);
    outputFileName = outputFileName.substr(0, outputFileName.find_last_of("."));

    Scene scene(sceneFileName);
    if (scene.isEmpty()) {
        std::cout << "Scene does not have any object to render\n";
        exit(0);
    }
    std::cout << scene << std::endl;

    Camera camera;
    camera.setOptions(scene.cameraOptions);
    camera.setRenderOptions(scene.renderOptions);

    const Vector3 from(scene.cameraOptions.getFrom());
    const Vector3 to(scene.cameraOptions.getTo());

    for (unsigned i=0; i < nImages; ++i)
    {
        std::cout << "\n" << i+1 << "/" << nImages << ": at " << std::fixed  << std::setw(6) <<  std::setprecision( 2 ) <<  i*angle << " deg" << std::flush;
        camera.lookAt( Transform::T(to) * Transform::Ry(deg2rad( i*angle )) * Transform::T(-to) * from, to); // rotate around y-axis

        auto startTime = std::chrono::high_resolution_clock::now();
        auto buffer = camera.capture(scene);
        auto endTime = std::chrono::high_resolution_clock::now();

        totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        avgTime += totalTime;

        std::stringstream ss;
        int ms = (unsigned long long)totalTime % 1000;
        int s  = ((unsigned long long)totalTime / 1000) % 60;
        int m  = ((unsigned long long)totalTime / 1000 / 60) % 60;
        int h  = ((unsigned long long)totalTime / 1000 / 60 / 60);
        ss << std::setw(2) << std::setfill('0') << h << "." << std::setw(2) << std::setfill('0') << m << "." << std::setw(2) << std::setfill('0') << s << "." << std::setw(3) << std::setfill('0') << ms;
        std::string totalTimeStr = ss.str();
        std::cout << ", Time: " << totalTimeStr << std::endl << std::flush;

        std::stringstream ss2;
        ss2 << outputFileName << "_" << std::setw(4) << std::setfill('0') << i;
        if (detailedName) { ss2 << "_SPP" << scene.renderOptions.spp << "_T" << totalTimeStr; }

        buffer->write_png(ss2.str().c_str());
    }

    avgTime = avgTime / (nImages);
    std::cout << std::endl << "Total Average in ms: " << avgTime << std::endl;

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
                nImages = atoi(argv[opt]);
            }
            else
            {
                usage(argv);
                exit(0);
            }
        }
        else
        {
            sceneFileName = std::string(argv[opt]);
        }
        ++opt;
    }
    if (sceneFileName.empty())
    {
        usage(argv);
        exit(0);
    }
}
