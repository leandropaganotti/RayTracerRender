PROGRAM = render
CXXFLAGS =  -O3 -std=c++0x -Wall -W -fopenmp
CXX = g++
#INCPATH = -I. -I/usr/include/libxml2
#LIBDIRS = -L/usr/local/lib
INCPATH = -I. -IC:\Users\lpaganotti\RayTracerRender\libxml\include\libxml2
LIBDIRS = -LC:\Users\lpaganotti\RayTracerRender\libxml\lib
LIBS = -lpthread -lxml2
LDFLAGS = $(LIBDIRS) $(LIBS)
LFLAGS  = -fopenmp

OBJECTS = main.o \
camera.o \
image.o \
light.o \
plane.o \
raytracer.o \
scene.o \
sphere.o \
texture.o \
transformation.o \
cameraoptions.o \
material.o \
shape.o \

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CXX) $(LFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

$(OBJECTS): %.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) $< -o $@

clean:
	$(RM) -f $(OBJECTS) $(PROGRAM)
