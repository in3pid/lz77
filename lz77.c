#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "lz77.h"
#include "window.h"

#define BUFSIZE 1024

void compress(int in, int out)
{
  match_t m;
  window_t w;
  char *p;
  char *end;
  char buf[BUFSIZE];
  int length;

  window_init(&w, BUFSIZE);

  while ((length = read(in, buf, BUFSIZE)) > 0) {
    p = buf;
    end = buf+length;
    while (p < end) {
      window_match(&w, &m, p, end);
      write(out, &m, sizeof m);
      if (m.length == 0) {
	write(out, p, 1);
	window_append(&w, *p);
	p++;
      }
      else {
	window_append_match(&w, &m);
	p += m.length;
      }
    }
  }
  window_free(&w);
}

void decompress(int in, int out)
{
  match_t m;
  window_t w;
  int len;
  char c;
  const char *p;

  window_init(&w, BUFSIZE);

  while ((len = read(in, &m, sizeof m)) == sizeof m) {
    if (m.length == 0) {
      read(in, &c, 1);
      write(out, &c, 1);
      window_append(&w, c);
    }
    else {
     p = window_distance(&w, m.distance);
     write(out, p, m.length);
     window_append_match(&w, &m);
    }
  }
  window_free(&w);
}

void usage(void)
{
  printf("usage\n");
}

int main(int argc, char **argv)
{
  int mode_decompress = 0;
  int in, out;
  int opt;
  while ((opt = getopt(argc, argv, "d")) != -1) {
    switch (opt) {
    case 'd':
      mode_decompress = 1;
      break;
    default:
      usage();
      return 1;
    }
  }
  if (argc < optind+2) {
    usage();
    return 1;
  }
  in = open(argv[optind], O_RDONLY);
  out = open(argv[optind+1], O_RDWR);
  if (mode_decompress) decompress(in, out);
  else compress(in, out);
  close(in);
  close(out);
  return 0;
}
