PROGRAM = render
OBJDIR = build

CXXFLAGS =  -O3 -std=c++14 -Wall -W -fopenmp
CXX = g++
INCPATH = -I./src -I./src/math -I./src/math/shapes -I/usr/include/libxml2
LIBDIRS = -L/usr/lib/x86_64-linux-gnu
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
cylinder.o \
invisibleshape.o \
aabox.o \
aabb.o \
mesh.o \
objparser.o \
shapefactory.o \
xmlparser.o 


CObjects=$(addprefix $(OBJDIR)/,$(OBJECTS))


$(OBJDIR)/%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INCPATH) -c $< -o $@

all: $(PROGRAM)

$(PROGRAM): $(CObjects)
	$(CXX) $(LFLAGS) $(CObjects) -o $@ $(LDFLAGS)

$(CObjects): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	$(RM) -f $(CObjects) $(PROGRAM)

