#pragma once

#include <string>
#include <memory>
#include <map>
#include "paramset.h"

template<class T>
class Resource
{
public:
    static std::shared_ptr<T> Create(const std::string &key, const ParamSet &params)
    {
        std::shared_ptr<T> res = T::Create(params);
        if(res)
            resources[key] = res;
        return res;
    }
    static std::shared_ptr<T> Create(const std::string &key)
    {
        std::shared_ptr<T> res = T::Create();
        if (res)
            resources[key] = res;
        return res;
    }
    static std::shared_ptr<T> Get(const std::string &key)
    {
        if(resources.find(key) == resources.end())
            return nullptr;

        return resources[key];
    }
    static void Add(const std::string &key, std::shared_ptr<T> &res)
    {
        resources[key] = res;
    }

protected:
    Resource() = delete;
    static std::map<std::string, std::shared_ptr<T>> resources;
};

template<class T>
std::map<std::string, std::shared_ptr<T>> Resource<T>::resources;
