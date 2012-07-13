#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include "lz77.h"
#include "window.h"

#define BUFSIZE 1024

void compress(int in, int out)
{
  window_t window;
  char buf[BUFSIZE];
  int length = read(in, buf, BUFSIZE);
  window_init(&window, BUFSIZE);
  for (int i = 0; i < length;) {
    match_t match;
    bzero(&match, sizeof match);
    window_match(&window, &match, buf+i, length-i);
    if (match.length == 0) {
      window_append(&window, buf[i]);
      match_t m = {1, 0};
      write(out, &m, sizeof m);
      write(out, buf+i, 1);
      i++;
    }
    else {
      for (int j = 0; j < match.length; j++) {
	char c = window.buffer[window.position-match.length];
	window_append(&window, c);
      }
      write(out, &match, sizeof match);
      i += match.length;
    }
  }
  window_free(&window);
}

void decompress(int in, int out)
{
  match_t match;
  window_t window;
  int len;
  window_init(&window, 1024);
  while ((len = read(in, &match, sizeof match)) > 0) {
    if (match.distance == 0) {
      char c;
      read(in, &c, 1);
      write(out, &c, 1);
      window_append(&window, c);
    }
    else {
      for (int i = 0; i < match.length; i++) {
	char c = window.buffer[window.position-match.length];
	window_append(&window, c);
	write(out, &c, 1);
      }
    }		     
  }
}

void usage(void)
{

}

int main(int argc, char **argv)
{
  int mode_decompress = 0;
  int in, out;
  int opt;
  if (argc < 3) {
    usage();
    return 1;
  }
  while ((opt = getopt(argc, argv, "d")) != -1) {
    switch (opt) {
    case 'd':
      mode_decompress = 1;
      break;
    default:
      usage();
    }
  }
  in = open(argv[1], O_RDONLY);
  out = open(argv[2], O_RDWR);
  if (mode_decompress) decompress(in, out);
  else compress(in, out);
  close(in);
  close(out);
  return 0;
}
