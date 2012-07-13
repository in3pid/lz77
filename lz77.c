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
  window_t w;
  char buf[BUFSIZE];
  char *end;
  int length;

  window_init(&w, BUFSIZE);

  while ((length = read(in, buf, BUFSIZE)) > 0) {
    end = buf + length;
    for (char *p = buf; p < end;) {
      match_t match = {0, 0};
      window_match(&w, &match, p, end);
      if (match.length == 0) {
	match_t m = {1, 0};
	write(out, &m, sizeof m);
	write(out, p, 1);
	window_append(&w, *p);
	p++;
      }
      else {
	for (int j = 0; j < match.length; j++) {
	  window_append(&w, *window_distance(&w, match.distance));
	}
	write(out, &match, sizeof match);
	p += match.length;
      }
    }
  }
  window_free(&w);
}

void decompress(int in, int out)
{
  match_t match;
  window_t w;
  int len;

  window_init(&w, BUFSIZE);

  while ((len = read(in, &match, sizeof match)) == sizeof match) {
    if (match.distance == 0) {
      char c;
      read(in, &c, 1);
      write(out, &c, 1);
      window_append(&w, c);
    }
    else {
      for (int i = 0; i < match.length; i++) {
	char c = *window_distance(&w, match.distance);
	write(out, &c, 1);
	window_append(&w, c);
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
  in = open(argv[optind], O_RDONLY);
  out = open(argv[optind+1], O_RDWR);
  if (mode_decompress) decompress(in, out);
  else compress(in, out);
  close(in);
  close(out);
  return 0;
}
