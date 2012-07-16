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

void write_tab(int f, char *tab, int n)
{
  match_t a = match_pack(0, n);
  write(f, &a, sizeof a);
  write(f, tab, n);
}

int read_tab(int f, char *tab, int n)
{
  return read(f, tab, n) == n;
}

void compress(int in, int out)
{
  match_t m;
  window_t w;
  char *p;
  char *end;
  char buf[BUFSIZE];
  int length;
  char tab[64];
  int n = 0;

  window_init(&w, BUFSIZE);

  while ((length = read(in, buf, BUFSIZE)) > 0) {
    p = buf;
    end = buf+length;
    while (p < end) {
      m = window_match(&w, p, end);
      if (match_length(m) <= 1) {
	if (n >= match_length_max) {
	  write_tab(out, tab, match_length_max);
	  n = 0;
	}
	tab[n++] = *p;
	window_append(&w, *p);
	p++;
      }
      else {
	if (n) {
	  write_tab(out, tab, n);
	  n = 0;
	}
	write(out, &m, sizeof m);
	window_append_match(&w, m);
	p += match_length(m);
      }
      window_flush(&w);
    }
  }
  if (n) write_tab(out, tab, n);
  window_free(&w);
}

void decompress(int in, int out)
{
  match_t m;
  window_t w;
  window_init(&w, BUFSIZE);
  while (read(in, &m, sizeof m) == sizeof m) { 
    if (match_distance(m) == 0) {
      int n = match_length(m);
      char tab[64];
      if (!read_tab(in, tab, n)) {
	printf("illegal state\n");
      }
      window_append_tab(&w, tab, n);
    }
    else {
      window_append_match(&w, m);
    }
    if (window_should_flush(&w)) {
      write(out, w.start, w.block_size);
      window_flush(&w);
    }
  }
  if (0 < w.cursor-w.start) write(out, w.start, w.cursor-w.start);
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
