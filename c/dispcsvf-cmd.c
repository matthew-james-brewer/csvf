#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include "libcsvf.h"

int main(int argc, char * argv[]) {
  if(argc != 2) {
    puts("\1n\033[1;31mError: invalid argument number\033[0m\n\n\033[4mCommand usage: dispcsvf file.csvf\033[0m\n");
    exit(-1);
  }

  setlocale(LC_ALL, "");

  wchar_t* tab = csvf_char_gen(argv[1]);

  wprintf(L"%ls\n",tab);
  free(tab);

  return 0;
}
