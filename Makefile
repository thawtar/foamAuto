TARGET=foamAuto
all: createMesh.cpp main.cpp
   gcc -g -Wall -o $(TARGET) createMesh.cpp main.cpp

clean:
   $(RM) $(TARGET)
