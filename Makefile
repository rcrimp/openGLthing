CC = g++
OBJS = main.cpp engine.cpp object.cpp wavefront.cpp
EXEC = a.out 
CFLAGS = -w
LFLAGS = -lSDL2 -lGL -lGLEW


#This is the target that compiles our executable
all : $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) -o $(EXEC)
