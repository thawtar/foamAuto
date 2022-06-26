#include <stdio.h>
#include <stdlib.h>

int main()
  {
    printf("===================================================\n\n");
    printf("                 Welcome to foamAuto       \n\n");
    printf("===================================================\n\n");
    char* blockMeshHeader="FoamFile \
    { \
        version     2.0; \n \
        format      ascii; \n \
        class       dictionary;\n \
        object      blockMeshDict;\n \
    }\n";
    printf("%s", blockMeshHeader);
  return 0;
  }
