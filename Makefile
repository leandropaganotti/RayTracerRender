#
# Compiler flags
#
CXX = g++
CXXFLAGS =  -std=c++14 -Wall -W
VPATH = src/ src/shapes src/aggregates

INCPATH = -I. -I./src -I./src/shapes -I./src/aggregates -I/usr/include/libxml2 -I./libxml/include/libxml2
LIBDIRS = -L/usr/lib/x86_64-linux-gnu -L./libxml/lib
LIBS = -lxml2

LDFLAGS = $(LIBDIRS) $(LIBS)
LFLAGS  = -fopenmp

#
# Project files
#
SRCS = main.cpp \
camera.cpp \
image.cpp \
light.cpp \
plane.cpp \
raytracer.cpp \
scene.cpp \
sphere.cpp \
texture.cpp \
transformation.cpp \
cameraoptions.cpp \
material.cpp \
cylinder.cpp \
invisible.cpp \
box.cpp \
aabb.cpp \
mesh.cpp \
xmlparser.cpp \
objparser.cpp \
bvh.cpp \
pathtracer.cpp \
phong.cpp \
paramset.cpp \
triangle.cpp \
objectvector.cpp \


OBJS=$(SRCS:.cpp=.o )
EXE  = render

#
# Debug build settings
#
DBGDIR = debug
DBGEXE = $(DBGDIR)/$(EXE)
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS))
DBGCFLAGS = -g -O0 -DDEBUG

#
# Release build settings
#
RELDIR = release
RELEXE = $(RELDIR)/$(EXE)
RELOBJS = $(addprefix $(RELDIR)/, $(OBJS))
RELCFLAGS = -O3 -DNDEBUG

.PHONY: all clean debug prep release remake

# Default build
all: prep release

#
# Debug rules
#
debug: $(DBGEXE)

$(DBGEXE): $(DBGOBJS)
	$(CXX) $(DBGOBJS) -o $@ $(LDFLAGS) $(LFLAGS)

$(DBGDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(DBGCFLAGS) $(LFLAGS) $(INCPATH) -c $< -o $@

#
# Release rules
#
release: $(RELEXE)

$(RELEXE): $(RELOBJS)
	$(CXX) $(RELOBJS) -o $@ $(LDFLAGS) $(LFLAGS)

$(RELDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(RELCFLAGS) $(LFLAGS) $(INCPATH) -c $< -o $@

#
# Other rules
#
prep:
	@mkdir -p $(DBGDIR) $(RELDIR)

remake: clean all

clean:
	rm -f $(RELEXE) $(RELOBJS) $(DBGEXE) $(DBGOBJS)


