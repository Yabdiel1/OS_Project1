#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int abs_x;
int abs_y;
float terminal_x;
float terminal_y;
FILE *file = NULL;

void HandleSigint(int sig) {
  if (file) {
    printf("\nSIGINT received. Closing file and exiting safely.\n");
    fclose(file);  // Close the file safely
  }
  exit(EXIT_SUCCESS);  // Exit the program
}

int main() {
  const char *device = "/dev/input/mice";
  const char *output_file = "mouse_data.dat";

  int fd;
  unsigned char data[3];

  // Set up SIGINT handler
  signal(SIGINT, HandleSigint);

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
  printf("Reading from %s. Move your mouse.", device);

  while (1) {
    int bytes = read(fd, data, sizeof(data));
    if (bytes > 0) {
      int x_movement = (int8_t)data[1];
      abs_x += x_movement;
      int y_movement = (int8_t)data[2];
      abs_y += y_movement;

      if (abs_x <= 0) {
        terminal_x = (512 - abs(abs_x)) / 10.24;
      } else {
        terminal_x = (512 + abs_x) / 10.24;
      }

      if (abs_y < 0) {
        terminal_y = (384 + abs(abs_y)) / 30.72;
      } else {
        terminal_y = (384 - abs_y) / 30.72;
      }

      // Save coordinates to the binary file
      fwrite(&terminal_x, sizeof(float), 1, file);
      fwrite(&terminal_y, sizeof(float), 1, file);

      printf(
          "X: %d, Y: %d (Absolute Position) | X: %f, Y: %f (Terminal "
          "Position)\n",
          abs_x, abs_y, terminal_x, terminal_y);

      // Flush the file buffer to ensure data is written
      fflush(file);
    }
  }
  close(fd);
  return 0;
}