/**
 * @file main.c
 * @author group-9
 * @brief main function that opens the fat image and executes the shell
 *
 */
#include <stdio.h>
#include "utils.h"

FILE* img_file = NULL;

int main(int argc, const char * argv[]) {
  img_file = fopen(argv[1], "rb+");
  if (img_file == NULL) {
    printf("%s image file could not be opened\n", argv[1]);
    return 0;
  }
  fread(&bs, sizeof(bootsector), 1, img_file);
  execute(argv[1], bs, img_file);
  return 0;
}