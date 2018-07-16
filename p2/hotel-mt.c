#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct room{
  int room_num;
  int type;//dorm=0;family=1;double=2;single=3;
  int vacant;//for dorm,1-6;else 0/1;
  int arrival[6][3];//arrival[0]=date;arrival[1]=month;arrival[2]=year;
  int visitors;
  char *name[6];
  char *id[6];
  int keys[6];
  int breakfast[6];
  int nights[6];
  int price[6];
  struct room *next;
}room_t;

room_t* initialize(int flo,int dor,int fam,int dou,int sin){//initialize the link list based on the input of user

}
