#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef HOTEL_LL
#define HOTEL_LL
typedef struct room{
  int room_num;
  char type[100];
  int vacant;//for doom,1-6;else 0/1;
  int arrival[3];//arrival[0]=date;arrival[1]=month;arrival[2]=year;
  int visitors;
  char *name[6];
  char *id[6];
  int keys[6];
  int breakfast[6];
  int nights[6];
  int price[6];
  struct room *next;
}room_t;

room_t *create(room_t * begin);
#endif
