#ifndef VIEW_H
#define VIEW_H

#include <stdio.h>
#include <stdlib.h>

/* View / Read ID3v2 tag */
void view_tag(const char *filename);

/* Convert syncsafe big-endian size to integer */
//void convert_size1(unsigned int *size);

#endif