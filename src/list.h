#ifndef LIST_H
#define LIST_H

#include <map>
#include <string>
#include <memory>

template < class T>
class List {
public:
    List() = default;

    std::shared_ptr<T> add(std::shared_ptr<T> item, const std::string &name="")
    {
        return list[name=="" ? std::to_string(list.size()+1) : name] = item;
    }
    std::shared_ptr<T> get(const std::string &name)
    {
        auto item = list.find(name);
        return item == list.end() ? nullptr : item->second;
    }
protected:
    std::map<std::string, std::shared_ptr<T>> list;
};

#endif // LIST_H
