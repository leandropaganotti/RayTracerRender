#pragma once
#include "vector.h"
#include "utils.h"
#include <vector>
#include <string>

void CreateSomplesImage(const std::string &filename, std::vector<Vector2> &samples, size_t width=512, size_t height=512);

class Sampler2d
{
public:
    Sampler2d(){}
    virtual ~Sampler2d(){}
    virtual std::vector<Vector2> operator()(int nsamples) = 0;
protected:
    RNG rng;
};

class Sample2dFilter
{
public:
    Sample2dFilter(){}
    virtual ~Sample2dFilter(){}

    virtual void operator()(std::vector<Vector2> &samples) = 0;
};

class BoxFilter: public Sample2dFilter
{
public:
    void operator()(std::vector<Vector2> &samples)
    {
        for (auto &sample: samples)
        {
            sample.x = sample.x -0.5f;
            sample.y = sample.y -0.5f;
        }
    }
};

class TentFilter: public Sample2dFilter
{
public:
    void operator()(std::vector<Vector2> &samples)
    {
        for (auto &sample: samples)
        {
            float x = sample.x;
            float y = sample.y;

            if(x<0.5f) sample.x = (float)sqrt(2.0*double(x))-1.0f;
            else sample.x = 1.0f-(float)sqrt(2.0-2.0*double(x));

            if(y<0.5f) sample.y = (float)sqrt(2.0*double(y))-1.0f;
            else sample.y = 1.0f-(float)sqrt(2.0-2.0*double(y));
        }
    }
};

class Random: public Sampler2d
{
public:
    std::vector<Vector2> operator()(int nsamples) override
    {
        std::vector<Vector2> samples;
        for (int i=0; i < nsamples; ++i)
        {
            samples.emplace_back(rng(), rng());
        }
        return samples;
    }
};

class Jitter: public Sampler2d
{
public:
    std::vector<Vector2> operator()(int nsamples) override
    {
        std::vector<Vector2> samples(nsamples);
        int sqrt_nsamples = sqrt(nsamples);
        for (int i=0; i < sqrt_nsamples; ++i)
            for (int j=0; j < sqrt_nsamples; ++j)
            {
                float x = (float(j) + rng()) / float(sqrt_nsamples);
                float y = (float(i) + rng()) / float(sqrt_nsamples);
                samples[i*sqrt_nsamples +j].x = x;
                samples[i*sqrt_nsamples +j].y = y;
            }
        return samples;
    }
};


