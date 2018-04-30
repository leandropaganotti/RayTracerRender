TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lpthread

SOURCES += main.cpp \
    plane.cpp \
    vertex.cpp \
    triangle.cpp \
    mesh.cpp \
    cube.cpp \
    camera.cpp \
    scene.cpp \
    light.cpp \
    image.cpp \
    utils.cpp \
    ray.cpp \
    raycaster.cpp \
    sphere.cpp

HEADERS += \
    plane.h \
    object.h \
    vector.h \
    vertex.h \
    mesh.h \
    cube.h \
    triangle.h \
    camera.h \
    utils.h \
    scene.h \
    matrix.h \
    light.h \
    image.h \
    ray.h \
    raycaster.h \
    sphere.h

