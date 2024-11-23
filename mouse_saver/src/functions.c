#include "functions.h"

#include <stdio.h>
#include <stdlib.h>

void SigIntHandler(int x) {
  if (file) {
    printf("\nSIGINT received. Closing file and exiting safely.\n");
    fclose(file);  // Close the file safely
  }
  exit(EXIT_SUCCESS);  // Exit the program
}
