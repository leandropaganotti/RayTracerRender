#include "objparser.h"
#include "triangle.h"
#include "mesh.h"

#include <fstream>
#include <vector>
#include <sstream>

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string& s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string& s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string& s)
{
    std::string trimed = rtrim(ltrim(s));
    size_t i=0;
    int count=0;
    while(s[i]!='\0')
    {
        if(isspace(trimed[i]))
        {
            if (count == 1)
                trimed.erase(i, 1);
            else
                ++count;
        }
        else
            count = 0;


        ++i;
    }

    return trimed;
}

std::vector<std::string> split(const std::string &s, char delim) {
  std::stringstream ss(s);
  std::string item;
  std::vector<std::string> elems;
  while (std::getline(ss, item, delim)) {
    elems.push_back(std::move(item));
  }
  return elems;
}

std::shared_ptr<Mesh> OBJParser::ParseMesh(const std::string &filepath)
{
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

    std::ifstream ifs (filepath, std::ifstream::in);

    if (!ifs.is_open())
    {
        std::cerr << "Error opening file: " << filepath << std::endl;
        return nullptr;
    }

    mesh->addVertex({});
    mesh->addNormal({});

    std::string line;
    while (!ifs.eof())
    {

        std::getline(ifs, line);
        line = trim(line);
        //std::cout << line << std::endl;
        std::vector<std::string> list = split(line, ' ');

        if (list.size() == 0) continue;

        if (list.size() == 4 && list[0] == "v")
        {
            mesh->addVertex({stof(list[1]), stof(list[2]), stof(list[3])});
        }
        else if (list.size() == 4 && list[0] == "vn")
        {
            mesh->addNormal({stof(list[1]), stof(list[2]), stof(list[3])});
        }
        else if (list.size() >= 4 && list[0] == "f")
        {
            std::vector<std::string> list1;
            list1 = split(list[1], '/');
            size_t v1 = stoull(list1[0]);
            size_t nv1 = stoull(list1[2]);

            list1 = split(list[2], '/');
            size_t v2 = stoull(list1[0]);
            size_t nv2 = stoull(list1[2]);

            list1 = split(list[3], '/');
            size_t v3 = stoull(list1[0]);
            size_t nv3 = stoull(list1[2]);

            std::shared_ptr<MeshTriangle> face = std::make_shared<MeshTriangle>(mesh.get(), v1, v2, v3, nv1, nv2, nv3);
            mesh->addFace(face);

            if(list.size() == 5)
            {
                list1 = split(list[4], '/');
                size_t v4 = stoull(list1[0]);
                size_t nv4 = stoull(list1[2]);

                face = std::make_shared<MeshTriangle>(mesh.get(), v1, v3, v4, nv1, nv3, nv4);
                mesh->addFace(face);
            }
        }
    }

    //std::cout << *mesh << std::endl;
    ifs.close();
    mesh->buildAggregate();
    return mesh;
}
