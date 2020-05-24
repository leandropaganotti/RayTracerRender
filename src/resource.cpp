#include "resource.h"
#include <map>
#include <memory>

std::map<std::string, std::shared_ptr<Resource>> Resource::resources;

Resource::~Resource(){}

Resource::Resource(){}
