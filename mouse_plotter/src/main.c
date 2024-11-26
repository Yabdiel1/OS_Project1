#include <curses.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "functions.h"
#include "stdlib.h"

int flag = 0;

int main() {
  FILE *file;
  ssize_t nread;

  timer_t timer_id;
  struct sigevent sev;
  struct itimerspec its;

  signal(SIGALRM, TimerHandler);

  sev.sigev_notify = SIGEV_SIGNAL;
  sev.sigev_signo = SIGALRM;
  sev.sigev_value.sival_ptr = &timer_id;

  if (timer_create(CLOCK_REALTIME, &sev, &timer_id) == -1) {
    perror("timer_create");
    exit(1);
  }

  its.it_value.tv_sec = 0;
  its.it_value.tv_nsec = 10 * 1e6;
  its.it_interval.tv_sec = 0;
  its.it_interval.tv_nsec = 10 * 1e6;

  // Start the timer
  if (timer_settime(timer_id, 0, &its, NULL) == -1) {
    perror("timer_settime");
    exit(1);
  }

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
      mvprintw(int_y, int_x, "*");
      refresh();

      flag = 1;
      while (flag) {
      }
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