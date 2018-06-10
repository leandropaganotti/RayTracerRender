PROGRAM = render
CXXFLAGS =  -O3 -std=c++0x -Wall -W
CXX = g++
INCPATH = -I. -I/usr/include/libxml2
LIBDIRS = -L/usr/local/lib
LIBS = -lpthread -lxml2 
LDFLAGS = $(LIBDIRS) $(LIBS)

OBJECTS = camera.o cube.o image.o light.o main.o mesh.o plane.o render.o scene.o sphere.o xmlparser.o model.o box.o

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)

$(OBJECTS): %.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) $< -o $@

clean:
	$(RM) -f $(OBJECTS) $(PROGRAM)
