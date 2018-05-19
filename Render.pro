TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lpthread
LIBS += -L/usr/local/lib -lxml2
INCLUDEPATH = /usr/include/libxml2/

SOURCES += main.cpp \
    plane.cpp \
    mesh.cpp \
    cube.cpp \
    camera.cpp \
    scene.cpp \
    image.cpp \
    sphere.cpp \
    render.cpp \
    xmlparser.cpp \
    light.cpp

HEADERS += \
    plane.h \
    object.h \
    vector.h \
    mesh.h \
    cube.h \
    camera.h \
    utils.h \
    scene.h \
    matrix.h \
    light.h \
    image.h \
    ray.h \
    sphere.h \
    render.h \
    xmlparser.h

