CC = g++
SRCS = src/main.cpp src/engine.cpp src/object.cpp src/wavefront.cpp src/glsl.cpp
OBJS = $(SRCS:.c=.o)
TARGET = a.out 
CFLAGS = 
LFLAGS = -lSDL2 -lGL -lGLEW


#This is the target that compiles our executable
$(TARGET) : $(OBJS)
		$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)
