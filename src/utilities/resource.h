#pragma once

#include <string>
#include <memory>
#include <map>
#include "paramset.h"
#include <iostream>

template<class T>
using ResourceMap = std::unordered_map<std::string, std::shared_ptr<T>>;

template<class T>
class Resource
{
public:
    static std::shared_ptr<T> Create(const std::string &key)
    {
        std::shared_ptr<T> res = T::Create();
        if (res)
            GetResources()[key] = res;
        return res;
    }

    template <typename ... Args>
    static std::shared_ptr<T> Create(const std::string &key, Args&& ... args)
    {
        std::shared_ptr<T> res = T::Create(args...);
        GetResources()[key] = res;
        return res;
    }

    static std::shared_ptr<T> Create(const std::string &key, const ParamSet &params)
    {
        std::shared_ptr<T> res = T::Create(params);
        if(res)
            GetResources()[key] = res;
        return res;
    }

    static std::shared_ptr<T> Get(const std::string &key)
    {
        if(GetResources().find(key) == GetResources().end())
            return nullptr;

        return GetResources()[key];
    }

    static void Add(const std::string &key, std::shared_ptr<T> &res)
    {
        if(res)
            GetResources()[key] = res;
    }

protected:
    Resource() = delete;

    static ResourceMap<T>& GetResources()
    {
        static ResourceMap<T> *ans = new ResourceMap<T>();
        return *ans;
    }
};
