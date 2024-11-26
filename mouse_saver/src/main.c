#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "functions.h"

FILE *file = NULL;

int main() {
  const char *device = "/dev/input/mice";
  const char *output_file = "mouse_data.dat";

  int fd;
  unsigned char data[3];

  // Set up SIGINT handler
  signal(SIGINT, SigIntHandler);

  fd = open(device, O_RDONLY);
  if (fd == -1) {
    perror("ERROR: Count not open /dev/input/mice");
    return EXIT_FAILURE;
  }

  // Open the binary file for writing
  file = fopen(output_file, "wb");
  if (!file) {
    perror("ERROR: Could not open output file");
    close(fd);
    return EXIT_FAILURE;
  }
  printf("Reading from %s. Move your mouse.\n", device);

  int abs_x = 0;
  int abs_y = 0;
  float terminal_x;
  float terminal_y;

  while (1) {
    int bytes = read(fd, data, sizeof(data));
    if (bytes > 0) {
      int x_movement = (int8_t)data[1];
      abs_x += x_movement;
      int y_movement = (int8_t)data[2];
      abs_y += y_movement;

      // 1024 abs positions / 100 = 10.24
      if (abs_x <= 0) {
        terminal_x = ((MAX_X / 2) - abs(abs_x)) / 10.24;
      } else {
        terminal_x = ((MAX_X / 2) + abs_x) / 10.24;
      }

      // 768 abs positions / 25 = 30.72
      if (abs_y < 0) {
        terminal_y = ((MAX_Y / 2) + abs(abs_y)) / 30.72;
      } else {
        terminal_y = ((MAX_Y / 2) - abs_y) / 30.72;
      }

      int terminal_x_int = round(terminal_x);
      int terminal_y_int = round(terminal_y);

      // Save coordinates to the binary file
      fwrite(&terminal_x_int, sizeof(int), 1, file);
      fwrite(&terminal_y_int, sizeof(int), 1, file);

      printf(
          "X: %d, Y: %d (Absolute Position) | X: %d, Y: %d (Terminal "
          "Position)\n",
          abs_x, abs_y, terminal_x_int, terminal_y_int);

      // Flush the file buffer to ensure data is written
      fflush(file);
    }
  }
  close(fd);
  return 0;
}