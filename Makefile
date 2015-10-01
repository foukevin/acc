
all::

### --- CONFIGURATION SECTION ---
# CFLAGS and LDFLAGS are for the users to override from the command line.

CFLAGS = -g -O2 -Wall
LDFLAGS =
ALL_CFLAGS = $(CPPFLAGS) $(CFLAGS)
ALL_LDFLAGS = $(LDFLAGS)
STRIP ?= strip

# Variables

CC = cc
AR = ar
RM = rm -f
INSTALL = install
LIBPNG_CFLAGS = $(shell libpng-config --cflags)
LIBPNG_LIBS = $(shell libpng-config --libs)

### --- END CONFIGURATION SECTION ---

BASIC_CFLAGS = -I.
BASIC_LDFLAGS =

# Guard against environment variables
PROGRAM_OBJS =
MESH_PROGRAM =
MESH_OBJS =
SHADER_PROGRAM =
SHADER_OBJS =

PROGRAM_OBJS += array.o
PROGRAM_OBJS += error.o
PROGRAM_OBJS += hash.o
PROGRAM_OBJS += parse.o
PROGRAM_OBJS += mapfile.o
MESH_OBJS += mesh.o
MESH_OBJS += mesh-obj-read.o
MESH_OBJS += mesh-gl-write.o
SHADER_OBJS += shader.o

# Binary suffix, set to .exe for Windows builds
X =

MESH_PROGRAM = acc-mesh$X
SHADER_PROGRAM = acc-shader$X

MESH_OBJECTS = $(MESH_OBJS) $(PROGRAM_OBJS) acc-mesh.o
SHADER_OBJECTS = $(SHADER_OBJS) $(PROGRAM_OBJS) acc-shader.o

OBJECTS = $(MESH_OBJECTS) $(SHADER_OBJECTS)

# Libraries

EXTLIBS =

BASIC_CFLAGS += $(LIBPNG_CFLAGS)
EXTLIBS += $(LIBPNG_LIBS)

LIBS = $(EXTLIBS)

### Cleaning rules

.PHONY: clean

ALL_CFLAGS += $(BASIC_CFLAGS)
ALL_LDFLAGS += $(BASIC_LDFLAGS)

clean:
	$(RM) $(OBJECTS)

### Build rules

.PHONY: all strip

all:: $(MESH_PROGRAM) $(SHADER_PROGRAM)

strip: $(MESH_PROGRAM)
	$(STRIP) $(STRIP_OPTS) $^

.SUFFIXES:

$(MESH_OBJECTS): %.o: %.c
	$(CC) -o $*.o -c $(ALL_CFLAGS) $(EXTRA_CPPFLAGS) $<

$(MESH_PROGRAM): $(MESH_OBJECTS)
	$(CC) -o $@ $(MESH_OBJECTS) $(ALL_LDFLAGS) $(LIBS)

$(SHADER_OBJECTS): %.o: %.c
	$(CC) -o $*.o -c $(ALL_CFLAGS) $(EXTRA_CPPFLAGS) $<

$(SHADER_PROGRAM): $(SHADER_OBJECTS)
	$(CC) -o $@ $(SHADER_OBJECTS) $(ALL_LDFLAGS) $(LIBS)
