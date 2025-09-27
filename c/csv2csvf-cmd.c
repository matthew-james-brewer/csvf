#include <stdlib.h>
#include <stdio.h>
#include "libcsvf.h"

int main(int argc, char * argv[]) {
 if(argc != 3) {
  puts("\1n\033[1;31mError: invalid argument number\033[0m\n\n\033[4mCommand usage: csv2csvf file.csv file.csvf\033[0m\n");
  exit(-1);
 }
 csvToCsvf(argv[1], argv[2]);
}
