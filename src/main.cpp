#include <iostream>
#include <iomanip>
#include <thread>
#include <string>
#include <cstring>
#include <sstream>
#include "raytracer.h"

#include "shapefactory.h"
#include "objmodel.h"
#include "objparser.h"

using namespace std;

char	    *xmlscene = NULL;	// xml file with scene description
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
    output = output.substr(output.find_last_of("/\\")+1);
    output = output.substr(0, output.find_last_of("."));

    Scene scene;
    scene.grid = 4;
    scene.spp = 8;
    scene.maxDepth = 6;
    scene.raytracer = RayTracerType::Phong;
    scene.addLight( new PointLight({0,4,6}, 1));
    scene.cameraOptions.setFrom({0,4,10});
    scene.cameraOptions.setTo({0,1.5,0});

    scene.addObject(new Object(Shapes::CreatePlane({0,-0.2,0})));

    auto m = Material::Create();
    m->kd = Color::GREEN;
    m->ks = 0.3;
    auto inst = Shapes::CreateEllipsoid();
    auto obj = new Object(inst, m);    
    inst->setTransformation({1,1.7,0.2}, {0,0,0}, {0.3, 1,0.3});
    scene.addObject(obj);

    auto mx = Material::Create();
    mx->type = Material::Type::DIFFUSE;
    mx->kd = {0,0,1};
    mx->R0 = 0.9;
    auto cy = new Object(Shapes::CreateCylinder(), m);
    cy->setTransformation({1.0,0,0.2}, {0}, {0.3, 1.7, 0.3});
    //scene.addObject(cy);


    auto m2 = Material::Create();
    m2->E = 100;
    //m2->kd = {1};
    Object *light = new Object(Shapes::CreateSphere({2,5,0}, 0.5), m2);
    scene.addObject(light);

    auto box = Shapes::CreateBox();
    box->setTransformation({0,0,0}, {0,0,0}, {5, 0.1, 5});
    scene.addObject(new Object(box));



    auto m3 = Material::Create();
    m3->type = Material::Type::TRANSPARENT;
    auto mesh = Shapes::CreateMesh("glass");
    OBJParser::ParseMesh("./obj/glass.obj", mesh);
    Object *model = new Object(Shapes::CreateInstance(mesh), m3);
    model->setTransformation({0,0,0}, {0,0,0}, {0.5});
    scene.addObject(model);

    auto mesh2 = dynamic_pointer_cast<Mesh>(Shapes::Get("glass"));
    Object *model2 = new Object(Shapes::CreateInstance(mesh2), m3);
    model2->setTransformation({-1,0,0}, {0,0,0}, {0.5,0.6, 0.5});
    scene.addObject(model2);


    cout << scene << endl;

    RayTracer raytracer;
    Camera &camera = raytracer.getCamera();
    camera.setOptions(scene.cameraOptions);

    const Vector3 from( scene.cameraOptions.getFrom() ), to( scene.cameraOptions.getTo());

    size_t spp = ceilf(float(scene.spp)/(scene.grid*scene.grid)) * scene.grid * scene.grid;

    for (unsigned i=0; i < nimages; ++i)
    {
        cout << "\n" << i+1 << "/" << nimages << ": at " << std::fixed  << std::setw(6) <<  std::setprecision( 2 ) <<  i*angle << " deg" << flush;
        camera.lookAt( Matrix4::T(to) * Matrix4::Ry(deg2rad( i*angle )) * Matrix4::T(-to) * from, to); // rotate around y-axis

		auto start = std::chrono::high_resolution_clock::now();
        raytracer.render(scene);
		auto end = std::chrono::high_resolution_clock::now();

        time_in_ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        std::stringstream ss;
		int s=time_in_ms/1000, m=s/60, h=m/60;
        ss << std::setw(2) << std::setfill('0') << h << "." << std::setw(2) << std::setfill('0') << m << "." << std::setw(2) << std::setfill('0') << s << "." << std::setw(3) << std::setfill('0') << (unsigned long long)time_in_ms % 1000;
		std::string time_str = ss.str();
        cout << ", Time: " << time_str << endl << flush;
		
		std::stringstream ss2;
		ss2 << output << "_" << std::setw(4) << std::setfill('0') << i;
		if (detailedName) { ss2 << "_SPP" << spp << "_T" << time_str; }
        ss2 << ".ppm";
        raytracer.getBuffer().save_ppm_bin(ss2.str().c_str());

        time_in_ms_avg += time_in_ms;
    }

    time_in_ms_avg = time_in_ms_avg / (nimages);
    cout << endl << "Total Average in ms: " << time_in_ms_avg << endl;

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
