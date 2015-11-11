CC = g++
OBJS = src/main.cpp src/engine.cpp src/object.cpp src/wavefront.cpp src/glsl.cpp
EXEC = a.out 
CFLAGS = -W -Wall -g
LFLAGS = -lSDL2 -lGL -lGLEW


#This is the target that compiles our executable
all : $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) -o $(EXEC)
