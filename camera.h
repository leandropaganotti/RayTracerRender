#ifndef CAMERA_H
#define CAMERA_H

#include "utils.h"
#include "vector.h"
#include "matrix.h"

#define FOV 	40
#define WIDTH 	640
#define HEIGHT 	480

class CameraOptions
{
	friend class Camera;

	Vector3f from;
	Vector3f to;
	float fov;
	size_t width;
	size_t height;

	float aspectRatio;

public:
	CameraOptions(const Vector3f& from={0}, const Vector3f& to={0,0,-1}, float fov=FOV, size_t width=WIDTH, size_t height=HEIGHT):
		from(from), to(to), fov(deg2rad(fov)), width(width), height(height), aspectRatio(float(width)/height)
	{}
	CameraOptions(const Vector3f& from, const Vector3f& to, size_t width, size_t height):
		from(from), to(to), fov(deg2rad(FOV)), width(width), height(height), aspectRatio(float(width)/height)
	{}

	friend std::ostream &operator <<(std::ostream &os, const CameraOptions &opt);
};

class Camera
{
    CameraOptions 	options;
    Matrix4x4f 		cameraToWorld;
public:

    Camera() = default;

    void lookAt(const Vector3f &from, const Vector3f &to, const Vector3f& up={0.0f,1.0f,0.0f});

    void setResolution(size_t width, size_t height);
    void setFov(float fov);
    void setOptions(const CameraOptions& options);

    Vector3f getRayDirection(float i, float j) const;
    const Vector3f& getPosition() const;
    const CameraOptions& getOptions() const;

    friend std::ostream& operator << (std::ostream& os, const Camera& cam);
};

inline
Vector3f Camera::getRayDirection(float i, float j) const
{
    float Px = (2.0f * ((j) / options.width) - 1.0f) * tan(options.fov / 2.0f ) * options.aspectRatio;
    float Py = (1.0f - 2.0f * ((i) / options.height)) * tan(options.fov / 2.0f);
    Vector3f dir = (cameraToWorld * Vector3f(Px, Py, -1.0f)) - options.from;
    printf("%f %f\n", Px, Py);
    return dir.normalize();
}

#endif // CAMERA_H
