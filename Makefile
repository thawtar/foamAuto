TARGET=foamAuto
files := createMesh.cpp createCase.cpp main.cpp
all: createMesh.cpp main.cpp
	g++ -g -Wall -o $(TARGET) createMesh.cpp createCase.cpp main.cpp

clean:
	$(RM) $(TARGET)
