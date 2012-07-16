#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
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
      write_match(out, &m);
      if (m.length == 0) {
	write(out, p, 1);
	window_append(&w, *p);
	p++;
      }
      else {
	window_append_match(&w, &m);
	p += m.length;
      }
      window_flush(&w);
    }
  }
  window_free(&w);
}

void decompress(int in, int out)
{
  match_t m;
  window_t w;
  window_init(&w, BUFSIZE);
  while (read_match(in, &m)) { 
    if (m.length == 0) {
      char c;
      read(in, &c, 1);
      window_append(&w, c);
    }
    else {
      window_append_match(&w, &m);
    }
    if (window_should_flush(&w)) {
      write(out, w.start, w.block_size);
      window_flush(&w);
    }
  }
  write(out, w.start, w.cursor-w.start);
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
  out = creat(argv[optind+1], 0644);
  if (mode_decompress) decompress(in, out);
  else compress(in, out);
  close(in);
  close(out);
  return 0;
}
