TARGET=foamAuto
files := createMesh.cpp main.cpp
all: createMesh.cpp main.cpp
	g++ -g -Wall -o $(TARGET) createMesh.cpp main.cpp

clean:
	$(RM) $(TARGET)
