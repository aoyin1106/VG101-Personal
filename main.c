#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct room{
  int room_num;
  int type;//dorm=0;family=1;single=2;double=3;
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

//START HOTEL_DB 我的。h文件include搞不定就只好先放在一起写了
//DECLARE FUNCTIONS HERE
room_t *create(room_t * begin);//HOTEL_LL
void readperson(room_t *p,FILE *fp,int num);//HOTEL_DB
int read_db(FILE *fp,room_t* hotel[]);//HOTEL_DB
void readhotel_db();//HOTEL_DB
void free_hotel(room_t *hotel[],int floor);//main
void initialize(room_t *begin,int floor);//EX2
room_t * findroom(int n, room_t* hotel[]);//HOTEL_DB


int main(){
  room_t *hotel[5];
  room_t head1;head1.next=NULL;
  room_t *ptr_head1=&head1;hotel[0]=ptr_head1;
  room_t head2;head2.next=NULL;
  room_t *ptr_head2=&head2;hotel[1]=ptr_head2;
  room_t head3;head3.next=NULL;
  room_t *ptr_head3=&head3;hotel[2]=ptr_head3;
  room_t head4;head4.next=NULL;
  room_t *ptr_head4=&head4;hotel[3]=ptr_head4;
  room_t head5;head5.next=NULL;
  room_t *ptr_head5=&head5;hotel[4]=ptr_head5;

//  memcpy(hotel,initialize(),5*sizeof(room_t));
  printf("This is the initialization part.\n");
  for (size_t i = 0; i < 5; i++) {
    initialize(hotel[i],i+1);
  }
//test
  room_t *p=hotel[0];
  for (size_t i = 0; i < 12; i++) {
    printf("%d\n", p->type);
    printf("%d\n", p->room_num);
    p=p->next;
  }

  printf("The initialization has ended.\n");



  readhotel_db(hotel);//TODO:change this function based on initialize






//A function used to free all link lists.
  free_hotel(hotel,5);
  return 0;
}

void readhotel_db(room_t *hotel[]){
  printf("Now we start to read hotel.db into initialized structure.\n\n" );
  int i=0;//return of read_db function, used to judge whether fp is all read
  FILE *fp=fopen("hotel.db","r");
  while(i==0) {
    i=read_db(fp,hotel);
    printf("i = %d\n", i);
  }
  fclose(fp);
//  return ptr_head;
  printf("\n\nAll information in the hotel.db has been read over.\n\n" );
}

//FUNCTION READING A ROOM LIST

int read_db(FILE *fp,room_t* hotel[]) {
  char type[100];
  int t;
  room_t buffer;
  room_t *p=&buffer;
  if(fscanf(fp,"[room %d]\n",&p->room_num)==EOF){
    return 1;
  }else{
      p=findroom(p->room_num,hotel);
      printf("p->room_num %d\n", p->room_num);
  fscanf(fp,"type = %s\n",type);
      printf("type %s\n",type);
  switch (type[0]*type[1]*type[2]) {
    case (int)'d'*(int)'o'*(int)'r':
    t = 0;
    break;
    case (int)'f'*(int)'a'*(int)'m':
    t = 1;
    break;
    case (int)'s'*(int)'i'*(int)'n':
    t = 2;
    break;
    case (int)'d'*(int)'o'*(int)'u':
    t = 3;
    break;
  }
  if (t!=p->type) {
    printf("\nSomething impossible happened. The %droom has a type contradict.\n", p->room_num);
    printf("We assume this room to be type %d\nHowever it is actually type %d\n\n", t,p->type);
  }
  printf("p->type %d\n", p->type);
  //case of dorm
  if(p->type == 0){
    fscanf(fp,"vacant = %d\n",&p->vacant);
        printf("p->vacant %d\n", p->vacant);
    fscanf(fp,"visitors = %d\n",&p->visitors);
        printf("p->visitors %d\n", p->visitors);
    if (p->visitors!=0) {
      for (int i = 0; i < (p->visitors); i++) {
        readperson(p,fp,i);
      }
    }
    fscanf(fp,"\n");
  }
  //case of family
  if(p->type == 1){
    fscanf(fp,"vacant = %d\n",&p->vacant);
        printf("p->vacant %d\n", p->vacant);
    p->visitors=0;
    if (p->vacant==0) {
      p->visitors=3;
      for (int i = 0; i < 3; i++) {
        readperson(p,fp,i);
      }
    }
    fscanf(fp,"\n");
  }
  //case of SINGLE
  if(p->type == 2){
    fscanf(fp,"vacant = %d\n",&p->vacant);
        printf("p->vacant %d\n", p->vacant);
    p->visitors=0;
    if (p->vacant==0) {
      p->visitors=1;
      for (int i = 0; i < 1; i++) {
        readperson(p,fp,i);
      }
    }
    fscanf(fp,"\n");
  }
  //case of DOUBLE
  if(p->type == 3){
    fscanf(fp,"vacant = %d\n",&p->vacant);
        printf("p->vacant %d\n", p->vacant);
    p->visitors=0;
    if (p->vacant==0) {
      p->visitors=2;
      for (int i = 0; i < 2; i++) {
        readperson(p,fp,i);
      }
    }
    fscanf(fp,"\n");
  }
return 0;//0 means the function runs well, 1 means there is nothing left to read anymore
}
}

room_t * findroom(int n, room_t* hotel[]){
  int floor=n/100-1;
  room_t buffer;room_t*p=&buffer;p=hotel[floor];
  // for (size_t i = 0; i < num; i++) {
  //   p=p->next;
  // }
  while (p->room_num!=n) {
      p=p->next;
  }
  return p;
}

//FUNCTION READING A SINGLE PERSON
void readperson(room_t *p,FILE *fp,int num){//num decide to deal with which of 6 person
  //READ NAME
  //TODO: 增加程序的鲁棒性能
  char first_name[20],second_name[20],buffer[40];
  fscanf(fp,"name%*d = %s %s\n",first_name,second_name);
  //printf("first_name %s,second_name %s",first_name,second_name);
      strcpy(buffer,first_name);
      strcat(buffer,second_name);
      p->name[num]=(char*)malloc(100*sizeof(char));
      strcpy(p->name[num],buffer);
      //strcpy(p->name[num],buffer);
  //READ ID-PRICE
  p->id[num]=(char*)malloc(100*sizeof(char));
  fscanf(fp,"id%*d = %s\n",p->id[num]);
//  fscanf(fp,"id%*d = %*s\n");
  fscanf(fp,"arrival%*d = %d/%d/%d\n",&(p->arrival[num][0]),&(p->arrival[num][1]),&(p->arrival[num][2]));
  fscanf(fp,"key%*d = %d\n",&(p->keys[num]));
  fscanf(fp,"breakfast%*d = %d\n",&(p->breakfast[num]));
  fscanf(fp,"nights%*d = %d\n",&(p->nights[num]));
  fscanf(fp,"price%*d = %d\n",&(p->price[num]));

  printf("name = %s",p->name[num]);
//  printf("buffer = %s",buffer);
  printf("\nid = %s",p->id[num]);
  printf("\narrival = %d/%d/%d\n",p->arrival[num][0],p->arrival[num][1],p->arrival[num][2]);//num[0]=date;num[1]=month;num[2]=year
  printf("key = %d\n",p->keys[num]);
  printf("breakfast = %d\n",p->breakfast[num]);
  printf("nights = %d\n",p->nights[num]);
  printf("price = %d\n",p->price[num]);
  //success for the case of type==0
}
//END HOTEL_DB
//START HOTEL_LL
room_t *create(room_t * begin){
  room_t *p = (room_t *)malloc(sizeof(room_t));
  room_t *end;
  end = begin;
  while (1) {
    if (end->next != NULL) {
      end = end->next;
    }else{
      end->next = p;
      p->next=NULL;
      break;
    }
  }
  return p;
}
//ENDHOTEL_LL
//BEGIN HOTEL_MT
//TODO: EX2.1
void initialize(room_t*head, int floor){
  int /*flo=5,*/dor=2,fam=2,dou=4,si=4;
  room_t *buffer;
  //TODO:相应的修改hotel——db函数
  //printf("This is the initialization part.\n");
  //This part is used if we have to decide the structure of hotel based on user input
  //in that case, the db file is preferred to be generated based on this function, i dont like that
  /*
  printf("Please input the number of floor of this hotel.\n");
  scanf("%d", &flo);getchar();
  printf("Please input the number of dormitories of each floor.\n");
  scanf("%d", &dor);getchar();
  printf("Please input the number of family rooms of each floor.\n");
  scanf("%d", &fam);getchar();
  printf("Please input the number of single rooms of each floor.\n");
  scanf("%d", &si);getchar();
  printf("Please input the number of double rooms of each floor.\n");
  scanf("%d", &dou);getchar();
  */
  for (size_t j = 0; j < dor; j++) {
      if (j==0) {//which means it's operating to the first room
        head->type = 0;
        head->room_num = floor*100+j+1;
      }else{
      buffer=create(head);
      buffer->type = 0;
      buffer->room_num = floor*100+j+1;
    }
  for (size_t j = 0; j < fam; j++) {
      buffer=create(head);
      buffer->type = 1;
      buffer->room_num = floor*100+j+dor+1;
  }
  for (size_t j = 0; j < si; j++) {
      buffer=create(head);
      buffer->type = 2;
      buffer->room_num = floor*100+j+dor+fam+1;
  }
  for (size_t j = 0; j < dou; j++) {
      buffer=create(head);
      buffer->type = 3;
      buffer->room_num = floor*100+j+dor+fam+si+1;
  }
}

  // printf("The initialization has ended.\n");
}
//END HOTEL_MT
//free_hotel is a function in the main function
void free_hotel(room_t *hotel[],int floor){
  room_t *buffer;
  for(size_t j;j<floor;j++){
  room_t*ptr_head=hotel[j];
  while (ptr_head != NULL) {
    buffer=ptr_head->next;
    if(ptr_head->visitors!=0){
      for (size_t i = 0; i < ptr_head->visitors; i++) {
          free(ptr_head->name[i]);
          free(ptr_head->id[i]);
      }
    }
    free(ptr_head);
    ptr_head=buffer;
  }
}
}
