#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <vector>

class ParamSet
{
public:
    ParamSet();
    ParamSet(const ParamSet& other);
    ~ParamSet();

    template<class T>
    void set(const std::string &name, const T &value);

    template<class T>
    T get(const std::string &name);

    ParamSet& operator=(const ParamSet &other);
private:
    template<class T>
    static std::unordered_map <const ParamSet*, std::unordered_map<std::string, T>> params;
    std::vector<std::function<void(const ParamSet &)>> deleters;
    std::vector<std::function<void(const ParamSet &, const ParamSet &)>> copiers;
};

template<class T>
std::unordered_map <const ParamSet*, std::unordered_map<std::string, T>> ParamSet::params;

template<class T>
void ParamSet::set(const std::string &name, const T &value)
{
    if(params<T>.find(this) == params<T>.end())
    {
        deleters.emplace_back([](const ParamSet &_this) {
            params<T>.erase(&_this);
        });
        copiers.emplace_back([](const ParamSet &_this, const ParamSet &other){
            params<T>[&_this] = params<T>[&other];
        });
    }
    params<T>[this][name] = value;
}

template<class T>
T ParamSet::get(const std::string &name)
{
    if(params<T>.find(this) == params<T>.end())
        return T{};
    else if(params<T>[this].find(name) == params<T>[this].end())
        return T{};

    return params<T>[this][name];
}
