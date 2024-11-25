#include <curses.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "stdlib.h"

int main() {
  pid_t child = fork();

  if (child == 0) {
    if (execl("/bin/ln", "ln", "-sf", "../../mouse_saver/build/mouse_data.dat",
              "sym", NULL) == -1) {
      perror("error executing child with sym link creation");
      exit(EXIT_FAILURE);
    }
  } else {
    wait(NULL);
  }

  FILE *file;
  ssize_t nread;

  initscr();
  keypad(stdscr, TRUE);
  noecho();

  curs_set(0);

  char buf[8];
  file = fopen("sym", "rb");
  if (file) {
    while ((nread = fread(buf, 1, sizeof buf, file)) > 0) {
      char x[4];
      char y[4];
      for (int i = 0; i < sizeof(x); i++) {
        x[i] = buf[i];
        y[i] = buf[i + sizeof(y)];
      }
      int int_x = (int)*x;
      int int_y = (int)*y;

      clear();
      mvprintw(0, 0, "%d", int_x);
      mvprintw(0, 2, "%d", int_y);
      mvprintw(int_y, int_x, "*");
      refresh();

      usleep(10000); /* Duerme por 10ms */
    }
    if (ferror(file)) {
      /* deal with error */
    }
    fclose(file);
  }

  getch();
  endwin();

  return 0;
}