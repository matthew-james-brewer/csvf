#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <wchar.h>
#include <locale.h>
#include "libcsvf.h"

typedef struct {
 int length;
 char** a;
} strArray;

static strArray* split(char* s, char c) {
 char* str = strdup(s);
 char* c2 = malloc(sizeof(char)*2);
 *c2++ = c; *c2-- = 0;
 strtok(str, c2);
 int i = 1;
 while(strtok(NULL, c2)) {
   i++;
 }
 free(str);
 str = strdup(s);
 strArray* result = malloc(sizeof(strArray));
 result->length = i;
 result->a = malloc(sizeof(char*)*i);
 char* spl = strtok(str, c2);
 result->a[0] = strdup(spl);
 int j = 1;
 while((spl = strtok(NULL, c2))) {
   result->a[j] = strdup(spl);
   j++;
 }
 free(str);
 free(c2);
 return result;
}

static void freeStrArray(strArray* arrToFree) {
  for(int i = 0; i < (arrToFree->length); i++) {
    free(arrToFree->a[i]);
  }
  free(arrToFree->a);
  free(arrToFree);
}

// All the vars shared by functions.

static int* cellSize;
static uint8_t* colThick;
static int rowWidth;
static strArray* f0split;
static strArray* f2split;

static wchar_t* horizontal(int len, uint8_t thick) {
  wchar_t* outOG = malloc((len+1)*sizeof(wchar_t));
  wchar_t* out = outOG;
  for(int i = 0; i < len; i++) {
    *out++ = (thick == 1) ? L'─' : L'═';
  }
  *out = 0;
  return outOG;
}

static wchar_t* vertical(strArray* values) {
  wchar_t* outOG = malloc(rowWidth*sizeof(wchar_t));
  wchar_t* out = outOG;
  for(int i = 0; i < (f0split->length); i++) {
    *out++ = (colThick[i] == 1) ? L'│' : L'║';
    if(i != (f0split->length - 1)) {
      int len = strlen(values->a[i]);
      size_t truelen = mbstowcs(out, values->a[i], len);
      out += truelen;
      int difference = cellSize[i] - truelen;
      if(difference != 0) {
        for(int j = 0; j < difference; j++) {
          *out++ = L' ';
        }
      }
    }
  }
  *out = 0;
  return outOG;
}

wchar_t* csvf_char_gen(char* csvf) {
 FILE* fPtr = fopen(csvf, "r");
 if(fPtr != NULL) {
  char line[1000];
  
  int ln = 0; // num lines
  while(fgets(line, sizeof(line), fPtr)) { ln++; }
  rewind(fPtr);
  
  fgets(line, sizeof(line), fPtr);
  char* f0 = strdup(line);
  f0[strlen(f0)-1] = 0; // strip off newlines
  fgets(line, sizeof(line), fPtr);
  char* f1 = strdup(line);
  f1[strlen(f1)-1] = 0;
  fgets(line, sizeof(line), fPtr);
  char* f2 = strdup(line);
  f2[strlen(f2)-1] = 0;
  f0split = split(f0, ',');
  f2split = split(f2, ',');
  cellSize = calloc(f2split->length, sizeof(int)); // needs to be zeroed, so uses calloc
  colThick = malloc(sizeof(uint8_t)*(f0split->length));
  int fg = 1;
  while(fg) {
   strArray* cols = split(line, ',');
   for(int j = 0; j < (cols->length); j++) {
    int len = strlen(cols->a[j]);
    if(len > cellSize[j]) {
     cellSize[j] = len;
    }
   }
   if(fgets(line, sizeof(line), fPtr)) {
    fg = (fgets(line, sizeof(line), fPtr) != 0);
   } else {
    fg = 0;
   }
   freeStrArray(cols);
  }
  rewind(fPtr);
  
  rowWidth = f0split->length; // start with num columns
  for(int i = 0; i < (f2split->length); i++) {
    rowWidth += cellSize[i]; // and add each cell's size
  }
  
  wchar_t* tableOG = malloc(((ln-2)*rowWidth)*2*sizeof(wchar_t)); // allocate exactly enough space for table
  wchar_t* table = tableOG; // mutable pointer; changes as program runs
  
  for(int i = 0; i < (f0split->length); i++) {
    colThick[i] = (uint8_t)atoi(f0split->a[i]);
  }

  uint8_t f1byte = (uint8_t)atoi(f1);
  switch(10*f1byte + colThick[0]) {
   case 11: *table++ = L'┌'; break; // row = 1, col = 1
   case 12: *table++ = L'╓'; break; // row = 1, col = 2
   case 21: *table++ = L'╒'; break; // row = 2, col = 1
   case 22: *table++ = L'╔'; break; // row = 2, col = 2
  }
  wchar_t* h1 = horizontal(cellSize[0], f1byte);
  wcscpy(table, h1);
  table += wcslen(h1);
  free(h1);
  
  if(f0split->length != 2) {
    for(int i = 1; i < (f2split->length); i++) {
      switch(10*f1byte+colThick[i]) {
        case 11: *table++ = L'┬'; break;
        case 12: *table++ = L'╥'; break;
        case 21: *table++ = L'╤'; break;
        case 22: *table++ = L'╦'; break;
      }
      h1 = horizontal(cellSize[i], f1byte);
      wcscpy(table, h1);
      table += wcslen(h1);
      free(h1);
    }
  }
  
  switch(10*f1byte+colThick[f2split->length]) {
    case 11: *table++ = L'┐'; break;
    case 12: *table++ = L'╖'; break;
    case 21: *table++ = L'╕'; break;
    case 22: *table++ = L'╗'; break;
  }
  
  *table++ = L'\n';
  
  h1 = vertical(f2split);
  wcscpy(table, h1);
  table += rowWidth;
  free(h1);
  
  for(int i = 0; i < 4; i++) { fgets(line, sizeof(line), fPtr); }
  
  for(int i = 0; i < ((ln-2)/2-1); i++) {
    *table++ = L'\n';
    uint8_t fibyte = (uint8_t)atoi(line);
    switch(10*fibyte+colThick[0]) {
      case 11: *table++ = L'├'; break;
      case 12: *table++ = L'╟'; break;
      case 21: *table++ = L'╞'; break;
      case 22: *table++ = L'╠'; break;
    }
    h1 = horizontal(cellSize[0], fibyte);
    wcscpy(table, h1);
    table += wcslen(h1);
    free(h1);
    
    for(int j = 1; j < (f2split->length); j++) {
      switch(10*fibyte+colThick[j]) {
        case 11: *table++ = L'┼'; break;
        case 12: *table++ = L'╫'; break;
        case 21: *table++ = L'╪'; break;
        case 22: *table++ = L'╬'; break;
      }
      h1 = horizontal(cellSize[j], fibyte);
      wcscpy(table, h1);
      table += wcslen(h1);
      free(h1);
    }
    switch(10*fibyte+colThick[f2split->length]) {
      case 11: *table++ = L'┤'; break;
      case 12: *table++ = L'╢'; break;
      case 21: *table++ = L'╡'; break;
      case 22: *table++ = L'╣'; break;
    }
    
    fgets(line, sizeof(line), fPtr);
    
    *table++ = L'\n';
    
    line[strlen(line)-1] = 0;
    
    strArray* lineSplit = split(line,',');
    h1 = vertical(lineSplit);
    wcscpy(table, h1);
    table += wcslen(h1);
    free(h1);
    freeStrArray(lineSplit);
    
    fgets(line, sizeof(line), fPtr);
  }
  
  uint8_t flbyte = (uint8_t)atoi(line);
  
  *table++ = L'\n';
  
  switch(10*flbyte+colThick[0]) {
    case 11: *table++ = L'└'; break;
    case 12: *table++ = L'╙'; break;
    case 21: *table++ = L'╘'; break;
    case 22: *table++ = L'╚'; break;
  }
  
  h1 = horizontal(cellSize[0], flbyte);
  wcscpy(table, h1);
  table += wcslen(h1);
  free(h1);
  
  for(int i = 1; i < (f2split->length); i++) {
    switch(10*flbyte+colThick[i]) {
      case 11: *table++ = L'┴'; break;
      case 12: *table++ = L'╨'; break;
      case 21: *table++ = L'╧'; break;
      case 22: *table++ = L'╩'; break;
    }
    h1 = horizontal(cellSize[i], flbyte);
    wcscpy(table, h1);
    table += wcslen(h1);
    free(h1);
  }
  switch(10*flbyte+colThick[f2split->length]) {
    case 11: *table++ = L'┘'; break;
    case 12: *table++ = L'╜'; break;
    case 21: *table++ = L'╛'; break;
    case 22: *table++ = L'╝'; break;
  }
  
  freeStrArray(f0split);
  freeStrArray(f2split);
  free(cellSize);
  free(colThick);
  free(f0);
  free(f1);
  free(f2);
  fclose(fPtr);
  
  *table = 0;

  return tableOG;
 } else {
  perror(""); exit(-1);
 }
}
