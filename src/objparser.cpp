#include "objparser.h"
#include <fstream>
#include <vector>
#include "utils.h"
#include <sstream>
#include "mesh.h"

void OBJParser::ParseMesh(const std::string &path, std::shared_ptr<Mesh> &mesh)
{    
    mesh->clear();
    mesh->addVertex({});
    mesh->addNormal({});
    mesh->addFace({});

    std::ifstream ifs (path, std::ifstream::in);

    if (ifs.is_open())
    {
        std::string line;
        while (!ifs.eof())
        {
            std::getline(ifs, line);
            std::vector<std::string> list = split(line, ' ');
            //v 1.250047 0.000000 0.000000
            if (list.size() == 4 && list[0] == "v")
            {
                mesh->addVertex({stof(list[1]), stof(list[2]), stof(list[3])});
            }
            else if (list.size() == 4 && list[0] == "vn")
            {
                mesh->addNormal({stof(list[1]), stof(list[2]), stof(list[3])});
            }
            else if (list.size() == 4 && list[0] == "f")
            {
                std::vector<std::string> list1;
                list1 = split(list[1], '/');
                size_t v1 = stoi(list1[0]);
                size_t nv1 = stoi(list1[2]);

                list1 = split(list[2], '/');
                size_t v2 = stoi(list1[0]);
                size_t nv2 = stoi(list1[2]);

                list1 = split(list[3], '/');
                size_t v3 = stoi(list1[0]);
                size_t nv3 = stoi(list1[2]);

                mesh->addFace(TriangleMesh(mesh.get(), v1, v2, v3, nv1, nv2, nv3));
            }
        }
    }
    else
    {
        std::cerr << "Error opening file: " << path << std::endl << path;
    }
    //std::cout << mesh << std::endl;
    ifs.close();
    mesh->updateAABB();    
}
