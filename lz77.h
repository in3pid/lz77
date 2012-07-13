#ifndef LZ77_H
#define LZ77_H

typedef struct match {
  int length;
  int distance;
} match_t;



void compress(int, int);
void decompress(int, int);

#endif
