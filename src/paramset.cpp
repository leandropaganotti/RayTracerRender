#include "paramset.h"

ParamSet::ParamSet(){}

ParamSet::ParamSet(const ParamSet &other)
{
    *this = other;
}

ParamSet::~ParamSet()
{
    for (auto&& _deleter: deleters)
        _deleter(*this);
}

ParamSet &ParamSet::operator=(const ParamSet &other)
{
    for (auto&& _deleter: deleters)
        _deleter(*this);

    deleters = other.deleters;
    copiers = other.copiers;

    for (auto&& _copier: copiers)
        _copier(*this, other);

    return *this;
}
