#include "model.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <algorithm>

std::vector<std::string> split(const std::string &s, char delim) {
  std::stringstream ss(s);
  std::string item;
  std::vector<std::string> elems;
  while (std::getline(ss, item, delim)) {
    //elems.push_back(item);
    elems.push_back(std::move(item)); // if C++11 (based on comment from @mchiasson)
  }
  return std::move(elems);
}

//std::vector<std::string> split(const std::string& str, int delimiter(int) = ::isspace){
//  std::vector<std::string> result;
//  auto e=str.end();
//  auto i=str.begin();
//  while(i!=e){
//    i=std::find_if_not(i,e, delimiter);
//    if(i==e) break;
//    auto j=find_if(i,e, delimiter);
//    result.push_back(std::string(i,j));
//    i=j;
//  }
//  return result;
//}

Model::Model()
{
    addVertex({});
    addNormal({});
}

void Model::load(std::string path)
{
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
                addVertex({stof(list[1]), stof(list[2]), stof(list[3])});
            }
            else if (list.size() == 4 && list[0] == "vn")
            {
                addNormal({stof(list[1]), stof(list[2]), stof(list[3])});
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


                addFace(Face(v1, v2, v3, nv1, nv2, nv3));
            }
        }
    }
    else
    {
        std::cerr << "Error opening file " << path;
    }
    //std::cout << mesh << std::endl;
    ifs.close();
}
