#ifndef MOUSE_SAVER_FUNCTIONS_H_
#define MOUSE_SAVER_FUNCTIONS_H_
#define MAX_X 1024
#define MAX_Y 768

#include <stdio.h>

extern FILE* file;

/// @brief Will close file used for writing terminal position coordinates
/// @return Nothing
void SigIntHandler(int x);

#endif  // MOUSE_SAVER_FUNCTIONS_H_