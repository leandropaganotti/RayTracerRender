PROGRAM = render
CXXFLAGS =  -O3 -std=c++0x -Wall -W -fopenmp
CXX = g++
INCPATH = -I. -I/usr/include/libxml2
LIBDIRS = -L/usr/local/lib
LIBS = -lpthread -lxml2 
LDFLAGS = $(LIBDIRS) $(LIBS)
LFLAGS  = -fopenmp

OBJECTS = camera.o image.o light.o main.o mesh.o plane.o raytracer.o scene.o sphere.o texture.o transformation.o box.o model.o xmlparser.o objparser.o aabb.o cameraoptions.o material.o

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CXX) $(LFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

$(OBJECTS): %.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) $< -o $@

clean:
	$(RM) -f $(OBJECTS) $(PROGRAM)
