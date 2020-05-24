#pragma once

#include <string>
#include <memory>
#include <map>

class Resource
{
public:
    virtual ~Resource();

    template<class T>
    static std::shared_ptr<T> Create(const std::string &key)
    {
        return T::Create(key);
    }
    template<class T>
    static std::shared_ptr<T> Get(const std::string &key)
    {
        if(resources.find(key) == resources.end())
            return nullptr;

        return std::dynamic_pointer_cast<T>(resources[key]);
    }

protected:
    Resource();

    template<class T>
    static std::shared_ptr<T> Create(const std::string &key, T *raw_ptr)
    {
        const std::string &key_ = key.empty() ? std::to_string(resources.size()) : key;

        auto res = std::shared_ptr<T>(raw_ptr);
        resources[key_] = res;
        resources[key_]->key = key_;
        return res;
    }

    std::string key;

    static std::map<std::string, std::shared_ptr<Resource>> resources;
};
