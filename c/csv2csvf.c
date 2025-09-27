#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "libcsvf.h"

// for counting the number of commas in the first row
static int countChar(char* str, char c) {
  int i = 0;
  char *pch=strchr(str,c);
  while (pch!=NULL) {
    i++;
    pch=strchr(pch+1,c);
  }
  return i;
}

/* THE CONVERTER
 Takes a csv file, and converts to a new csvf file.
*/
void csvToCsvf(char* inputName, char* outputName) {
 FILE* output = fopen(outputName, "w");

 FILE* csv = fopen(inputName, "r");
 if(csv != NULL) {
  char line[1000];
  
  fgets(line,sizeof(line),csv);
  int xd = countChar(line, ',');
  xd++;
  char* first = malloc(sizeof(char)*(2*(xd+2)));
  char* firstSave = first;
  while(xd--) {
   *first = '1';
   first++;
   *first = ',';
   first++;
  }
  *first = '1';
  first++;
  *first = 0;
  
  fputs(firstSave, output);
  free(firstSave);
  
  fputs("\n1\n", output);
  fputs(line, output);
  
  while(fgets(line,sizeof(line),csv) != NULL) {
   fputs("1\n", output);
   fputs(line, output);
  }
  fputs("\n1", output);
  
  fclose(csv);
  fclose(output);
 }
}
