#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "hotel-db.h"
//#include "hotel-ll.h"

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

void readperson(room_t *p,FILE *fp,int num){//num decide to deal with which of 6 person
  //TODO: a function reading the seven information of everyperson, different... for first person if double
  //READ NAME
  char first_name[20],second_name[20],buffer[40];
  fscanf(fp,"name%*d = %s %s",first_name,second_name);
  strcpy(buffer,first_name);
  strcat(buffer,second_name);
  p->name[num] = buffer;
  //READ ID-PRICE
  fscanf(fp,"id%*d = %s",p->id[num]);
  fscanf(fp,"arrival%*d = %d/%d/%d",&p->arrival[num][0],&p->arrival[num][1],&p->arrival[num][2]);
  fscanf(fp,"key%*d = %d",&p->keys[num]);
  fscanf(fp,"breakfast = %d",&p->breakfast[num]);
  fscanf(fp,"nights = %d",&p->nights[num]);
  fscanf(fp,"price = %d",&p->price[num]);
}

void read_db(room_t *p,FILE *fp) {
  char str[100];
  char type[100];
  fscanf(fp,"[room %d]",&p->room_num);
      printf("%d\n", p->room_num);
  fscanf(fp,"type = %s",type);
      printf("%s\n",type);

  switch (type[0]*type[1]*type[2]) {
    case (int)'d'*(int)'o'*(int)'r':
    p -> type = 0;
    break;
    case (int)'f'*(int)'a'*(int)'m':
    p -> type = 1;
    break;
    case (int)'s'*(int)'i'*(int)'n':
    p -> type = 2;
    break;
    case (int)'d'*(int)'o'*(int)'u':
    p -> type = 3;
    break;
  }

  //TODO: READ THE FILE BASED ON TYPE
  //case of dorm
  if(p->type == 0){
    fscanf(fp,"type = %d",&p->vacant);
        printf("%d\n", p->vacant);
    fscanf(fp,"visitors = %d",&p->visitors);
        printf("%d\n", p->visitors);
    if (p->visitors!=0) {
      for (size_t i = 0; i < (p->visitors); i++) {
        readperson(p,fp,i);
      }
    }
  }
    // switch (strcmp(head.type)) {
    //   case /* value */:
    // }
}


void hotel_db(){
  FILE *fp=fopen("hotel.db","r");
  room_t head;head.next = NULL;
  room_t *ptr_head=&head;
  room_t *list;
  read_db(ptr_head,fp);
  do {
    list = create(ptr_head);//TODO:A FUNCTION CREATING A NEW LIST, IN LL.C
    read_db(list,fp);
  } while(fp != NULL);
  fclose(fp);
}
