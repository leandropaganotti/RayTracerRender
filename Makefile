PROGRAM = render
OBJDIR = build

CXXFLAGS =  -O3 -std=c++0x -Wall -W -fopenmp
CXX = g++
#INCPATH = -I./src -I./src/math -I./src/math/shapes -I/usr/include/libxml2
#LIBDIRS = -L/usr/local/lib
INCPATH = -I./src -I./src/math -I./src/math/shapes -IC:\Users\lpaganotti\RayTracerRender\libxml\include\libxml2
LIBDIRS = -LC:\Users\lpaganotti\RayTracerRender\libxml\lib
LIBS = -lpthread -lxml2
LDFLAGS = $(LIBDIRS) $(LIBS)
LFLAGS  = -fopenmp
VPATH = src/ src/math src/math/shapes

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
shapefactory.o \
invisibleshape.o \
box.o \
aabb.o \
mesh.o \
objparser.o


CObjects=$(addprefix $(OBJDIR)/,$(OBJECTS))


$(OBJDIR)/%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INCPATH) -c $< -o $@

all: $(PROGRAM)

$(PROGRAM): $(CObjects)
	$(CXX) $(LFLAGS) $(LDFLAGS) $(CObjects) -o $@

$(CObjects): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	$(RM) -f $(CObjects) $(PROGRAM)

