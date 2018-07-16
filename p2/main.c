#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
void initialize(room_t*head, int floor,int dor,int fam,int dou,int si);//EX2
room_t * findroom(int n, room_t* hotel[]);//HOTEL_DB
void find_empty(int type,room_t *hotel[],int *empty,int all,int f);//EX2.5
time_t convert(int year,int month,int day);
char* id_create(char *id);
void print_card(room_t *hotel[],int num,int num_visitor);//ex2
void check_in(room_t *hotel[],time_t t);
void update_db(room_t *hotel[],int flo,int all);
void night_plus(room_t *hotel[],int flo,int all);
void breakfast_plus(room_t *hotel[],int flo,int all);
void price_fresh(room_t *hotel[],int flo,int all);

int main(){
  int date[3]={0};
  int dor=2,fam=2,dou=4,si=4;
  int all=dor+fam+dou+si;
  int days=0;
  srand((unsigned)time(NULL));
  time_t t;
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

  //temporatory, for easier test
  // printf("Please enter the date you want in the form of date/month/year:\n");
  // fscanf(stdin,"%d/%d/%d",&date[0],&date[1],&date[2]);

  date[2]=2018;date[1]=4;date[0]=12;

  t=convert(date[2],date[1],date[0]);

  //printf("t is %d\n", (int)t);
  for (size_t i = 0; i < 5; i++) {
    initialize(hotel[i],i+1,dor,fam,dou,si);
  }
  //readhotel_db(hotel);
  printf("The initialization has ended.\n");
  printf("How many days do you want to demonstrate?\n");
  fscanf(stdin, "%d", &days);
  getchar();
  for (size_t i = 0; i < days; i++) {
    struct tm *p_time=localtime(&t);
    printf("\nA new day comes! Today is %d/%d/%d\n", p_time->tm_mday,p_time->tm_mon+1,p_time->tm_year+1900);
    breakfast_plus(hotel,5,all);
    price_fresh(hotel,5,all);
    int people=0;
    printf("\nPlease decide number of person who will check in today.\n");
    scanf("%d", &people);
    getchar();
    //printf("%d\n", people);
    for (size_t j = 0; j < people; j++) {
      check_in(hotel,t);
      update_db(hotel,5,all);
    }
    printf("\nToday's check in process has ended\n\n");


    printf("Today's affairs have all ended.\n\n");
    night_plus(hotel,5,all);
    t+=24*3600;
  }
  printf("All of the demonstration has ended. Press any button to close the program\n");
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
    printf("\nThe %droom has a type contradict.\n", p->room_num);
    printf("We assume this room to be type %d\nHowever it is actually type %d\n\n", t,p->type);
    printf("This problem is caused by the incorrect set of database.\nPlease ensure room 01-02 are dorm;03-04 are family;05-08 are single;09-12 are double");
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



//FUNCTION READING A SINGLE PERSON
void readperson(room_t *p,FILE *fp,int num){//num decide to deal with which of 6 person
  //READ NAME
  //TODO: 增加程序的鲁棒性能
  char first_name[20],second_name[20],buffer[40];
  fscanf(fp,"name%*d = %s %s\n",first_name,second_name);
  //printf("first_name %s,second_name %s",first_name,second_name);
      strcpy(buffer,first_name);
      strcat(buffer,second_name);
      strcpy(p->name[num],buffer);
      //strcpy(p->name[num],buffer);
  //READ ID-PRICE
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
void initialize(room_t*head, int floor,int dor,int fam,int dou,int si){
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
        head->vacant=6;
        head->visitors=0;
        for (size_t i = 0; i < 6; i++) {
          head->breakfast[i]=0;
          head->nights[i]=0;
          head->nights[i]=0;
          for (size_t m = 0; m < 3; m++) {
            head->arrival[i][m]=0;
          }
          head->name[i]=(char*)malloc(100*sizeof(char));
          head->id[i]=(char*)malloc(100*sizeof(char));
          //head->id[i]=NULL;
        }
      }else{
      buffer=create(head);
      buffer->type = 0;
      buffer->room_num = floor*100+j+1;
      buffer->vacant=6;
      buffer->visitors=0;
      for (size_t i = 0; i < 6; i++) {
        buffer->breakfast[i]=0;
        buffer->nights[i]=0;
        buffer->nights[i]=0;
        buffer->name[i]=(char*)malloc(100*sizeof(char));
        buffer->id[i]=(char*)malloc(100*sizeof(char));
        for (size_t m = 0; m < 3; m++) {
          head->arrival[i][m]=0;
        }
      }
    }
  }
  for (size_t j = 0; j < fam; j++) {
      buffer=create(head);
      buffer->type = 1;
      buffer->room_num = floor*100+j+dor+1;
      buffer->vacant=1;
      buffer->visitors=0;
      for (size_t i = 0; i < 6; i++) {
        buffer->breakfast[i]=0;
        buffer->nights[i]=0;
        buffer->nights[i]=0;
        buffer->name[i]=(char*)malloc(100*sizeof(char));
        buffer->id[i]=(char*)malloc(100*sizeof(char));
        for (size_t m = 0; m < 3; m++) {
          head->arrival[i][m]=0;
        }
      }
    }
  for (size_t j = 0; j < si; j++) {
      buffer=create(head);
      buffer->type = 2;
      buffer->room_num = floor*100+j+dor+fam+1;
      buffer->vacant=1;
      buffer->visitors=0;
      for (size_t i = 0; i < 6; i++) {
        buffer->breakfast[i]=0;
        buffer->nights[i]=0;
        buffer->nights[i]=0;
        buffer->name[i]=(char*)malloc(100*sizeof(char));
        buffer->id[i]=(char*)malloc(100*sizeof(char));
        for (size_t m = 0; m < 3; m++) {
          head->arrival[i][m]=0;
        }
      }
  }
  for (size_t j = 0; j < dou; j++) {
      buffer=create(head);
      buffer->type = 3;
      buffer->room_num = floor*100+j+dor+fam+si+1;
      buffer->vacant=1;
      buffer->visitors=0;
      for (size_t i = 0; i < 6; i++) {
        buffer->breakfast[i]=0;
        buffer->nights[i]=0;
        buffer->nights[i]=0;
        buffer->name[i]=(char*)malloc(100*sizeof(char));
        buffer->id[i]=(char*)malloc(100*sizeof(char));
        for (size_t m = 0; m < 3; m++) {
          head->arrival[i][m]=0;
        }
      }
  }
}


void check_in(room_t *hotel[],time_t t){
  int all=12,f=5;//temporatory solution as i haven't use this formula in the main function.
  char *first_name[]={"Jacob","Michael","Ethan","Joshua","Alexander","Anthony","William","Christopher","Jayden","Andrew","Joseph","David","Noad","Aiden","James","Ryan","Logan","John","Nathan","Elijah","Christian","Gabriel","Benjamin","Jonathan","Tyler"};
  char blank[2]=" ";
  char *second_name[]={"Adey","Alan","Alban","Albert","Alberts","Albin","Alborn","Albright","Albro","Albury","Alcock","Alcott","Alday","Alden","Alder","Alders"};
  char id[7];
  int num_visitor=0;
  struct tm *ptr_time=localtime(&t);
    //
    // printf("%d\n",ptr_time->tm_mday);
    // printf("%d\n",ptr_time->tm_mon);
    // printf("%d\n",ptr_time->tm_year);
  printf("\n\nA new costomer is checking in...\n\n");
  //TODO:randomly generate the type of room
  int type=(int)((double)rand()/(RAND_MAX+1)*4.0);
  // type=1;//test;
    //printf("type = %d\n", type);
  //randomly generate his or her name The number of people relates to the type of room;
  //write a function returns all available rooms, which is the task of EX2.5
  int empty[1000]={0};
  find_empty(type,hotel,empty,all,f);
  int len_empty=0;//How many room is available;
  for (size_t i = 0; i < 1000; i++) {
    if (empty[i]!=0) {
      len_empty++;
    }else{
      break;
    }
  }
  if (len_empty==0) {
    printf("Sorry, there is no empty room for you.\n");
    return;
  }
  //:Decide the room he or she or they will live in and change the corresponding visitors, vacant, arrival
  int room=empty[(int)((double)rand()/(RAND_MAX+1)*len_empty)];
  room_t *p=findroom(room,hotel);
  ///char first_1[20],first_2[20],first_3[20],second_1[20],second_2[20],second_3[20];//if the type is family, we need 3 person
  switch (type) {
    case 0:
    num_visitor=p->visitors;
    //  printf("num of visitor in this dorm is %d\n", p->visitors);//test
    strcpy(p->name[p->visitors],first_name[(int)((double)rand()/(RAND_MAX+1)*25)]);
    strcat(p->name[p->visitors],blank);
    strcat(p->name[p->visitors],second_name[(int)((double)rand()/(RAND_MAX+1)*25)]);
    //  printf("id is %s\n", id_create(id));//test
    strcpy(id,id_create(id));
    strcpy(p->id[p->visitors],id);
    //  printf("p->id is %s\n", p->id[p->visitors]);//test
    p->arrival[p->visitors][0]=ptr_time->tm_mday;
    p->arrival[p->visitors][1]=ptr_time->tm_mon+1;
    p->arrival[p->visitors][2]=ptr_time->tm_year+1900;
    p->keys[p->visitors]=1;
    p->vacant--;p->visitors++;
    break;
    case 1:
    strcpy(p->name[0],first_name[(int)((double)rand()/(RAND_MAX+1)*25)]);
    strcat(p->name[0],blank);
    strcat(p->name[0],second_name[(int)((double)rand()/(RAND_MAX+1)*16)]);
    strcpy(p->name[1],first_name[(int)((double)rand()/(RAND_MAX+1)*25)]);
    strcat(p->name[1],blank);
    strcat(p->name[1],second_name[(int)((double)rand()/(RAND_MAX+1)*16)]);
    strcpy(p->name[2],first_name[(int)((double)rand()/(RAND_MAX+1)*25)]);
    strcat(p->name[2],blank);
    strcat(p->name[2],second_name[(int)((double)rand()/(RAND_MAX+1)*16)]);
    strcpy(id,id_create(id));
    strcpy(p->id[0],id);
    strcpy(id,id_create(id));
    strcpy(p->id[1],id);
    strcpy(id,id_create(id));
    strcpy(p->id[2],id);
    //printf("id is %s\n", id_create(id));//test
    //  printf("p->id is %s\n", p->id[0]);//test
    p->vacant=0;p->visitors=3;p->keys[0]=1;p->keys[1]=1;p->keys[2]=1;
    for (size_t i = 0; i < 3; i++) {
      p->arrival[i][0]=ptr_time->tm_mday;
      p->arrival[i][1]=ptr_time->tm_mon+1;
      p->arrival[i][2]=ptr_time->tm_year+1900;
    }
    break;
    case 2:
    strcpy(p->name[0],first_name[(int)((double)rand()/(RAND_MAX+1)*25)]);
    strcat(p->name[0],blank);
    strcat(p->name[0],second_name[(int)((double)rand()/(RAND_MAX+1)*16)]);
    strcpy(id,id_create(id));
    strcpy(p->id[0],id);
    //  printf("id is %s\n", id_create(id));//test
    //  printf("p->id is %s\n", p->id[0]);//test
    p->vacant=0;p->visitors=1;
    p->keys[0]=1;
    p->arrival[0][0]=ptr_time->tm_mday;
    p->arrival[0][1]=ptr_time->tm_mon+1;
    p->arrival[0][2]=ptr_time->tm_year+1900;
    break;
    case 3:
    strcpy(p->name[0],first_name[(int)((double)rand()/(RAND_MAX+1)*25)]);
    strcat(p->name[0],blank);
    strcat(p->name[0],second_name[(int)((double)rand()/(RAND_MAX+1)*16)]);
    strcpy(p->name[1],first_name[(int)((double)rand()/(RAND_MAX+1)*25)]);
    strcat(p->name[1],blank);
    strcat(p->name[1],second_name[(int)((double)rand()/(RAND_MAX+1)*16)]);
    strcpy(id,id_create(id));
    strcpy(p->id[0],id);
    strcpy(id,id_create(id));
    strcpy(p->id[1],id);
    //  printf("id is %s\n", id_create(id));//test
    //  printf("p->id is %s\n", p->id[0]);//test
    p->vacant=0;p->visitors=2;
    p->keys[0]=1;p->keys[1]=1;
    for (size_t i = 0; i < 2; i++) {
      p->arrival[i][0]=ptr_time->tm_mday;
      p->arrival[i][1]=ptr_time->tm_mon+1;
      p->arrival[i][2]=ptr_time->tm_year+1900;
    }
    break;
  }
  //TODO:write a function responsible for printing cards.
  printf("The check-in process has ended, here's the costomer's card\n\n");
  print_card(hotel,p->room_num,num_visitor);
}

void find_empty(int type,room_t *hotel[],int *empty,int all,int f){
  room_t *p;
  for (size_t i = 0,k=0; i < f; i++) {
    p=hotel[i];
    for (size_t j = 0; j < all; j++) {
      if (p->type==type && p->vacant!=0) {
        empty[k]=p->room_num;k++;
      }
      if (p->next!=NULL) {
        p=p->next;
      }else{
        break;
      }
    }
  }
}

void print_card(room_t *hotel[],int num,int num_visitor){
  room_t*p;p=findroom(num,hotel);
  printf("Costomer Card: \n");
  printf("Arrival: %d/%d/%d\n",p->arrival[num_visitor][0],p->arrival[num_visitor][1],p->arrival[num_visitor][2]);
  // printf("Arrival: num_visitor=%d,p->visitors=%d\n", num_visitor,p->visitors);
  printf("Room Number: %d\n", p->room_num);
  switch (p->type) {
    case 0:
      printf("Room Type: Dormitory\n");
      printf("Number of Visitor: 1\n");
      printf("Name: %s\n", p->name[num_visitor]);
      printf("ID: %s\n", p->id[0]);
      printf("Keys: 1\n");
      break;
    case 1:
      printf("Room Type: Family\n");
      printf("Number of Visitor: 3\n");
      for (size_t i = 0; i < 3; i++) {
      printf("Name: %s\n", p->name[i]);
      printf("ID: %s\n", p->id[i]);
      }
      printf("Keys: 3\n");
      break;
    case 2:
      printf("Room Type: Single\n");
      printf("Number of Visitor: 1\n");
      printf("Name: %s\n", p->name[0]);
      printf("ID: %s\n", p->id[0]);
      printf("Keys: 1\n");
      break;
    case 3:
      printf("Room Type: Double\n");
      printf("Number of Visitor: 2\n");
      for (size_t i = 0; i < 2; i++) {
      printf("Name: %s\n", p->name[i]);
      printf("ID: %s\n", p->id[i]);
      }
      printf("Keys: 2\n");
      break;
    }
    printf("\nThe card has been printed.\nPlease hit Enter to see the following process:\n");
    getchar();
}//END HOTEL_MT
//this is a function in db but used in many places
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
char* id_create(char *id){
  char dic[36]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
//  printf("%d\n", sizeof(dic));
  FILE *fp=fopen("id.db","r+");
  srand((unsigned)time(NULL));
  while (1) {
    for (size_t i = 0; i < 6; i++) {
      int j=(int)((double)rand()/(RAND_MAX+1)*36.0);
      //printf("%d\n",j);
      if (j>35) {
        printf("\n\nGG %d\n\n", j);
      }
      id[i]=dic[j];
      //id[i]=*dic[(int)((double)rand()/(RAND_MAX+1)*36.0)];
    }
     int tf=0;
     char temp[7];
     fseek(fp,0,SEEK_SET);
     while (fscanf(fp,"%s ",temp)!=EOF) {
       if (strncmp(temp, id,6)==0) {
         tf=1;
         break;
       }
     }

    if (tf==0) {
      fseek(fp,0,SEEK_END);
      fprintf(fp, "%s \n", id);
      break;
    }
  }
  fclose(fp);
  return id;
}


//this is a function either in main.c or time-related FUNCTIONS
void update_db(room_t *hotel[],int flo,int all){
  FILE *fp=fopen("hotel.db","w");
  if(fp == NULL){
    printf("Fail to open database.\n");
    return;
  }
  //fseek(fp,0,SEEK_SET);
  room_t *current;
  for (size_t j = 0; j < flo; j++) {
    current=hotel[j];
    for (size_t k = 0; k < all; k++) {
          fprintf(fp, "[room %d]\n", current->room_num);
          switch (current->type) {
            case 0:
            fprintf(fp, "type = dormitory\n");
            break;
            case 1:
            fprintf(fp, "type = family\n");
            break;
            case 2:
            fprintf(fp, "type = single\n");
            break;
            case 3:
            fprintf(fp, "type = double\n");
            break;
          }
          fprintf(fp, "vacant = %d\n", current->vacant);
          if (current->visitors!=0) {
              if (current->type==0) {
                fprintf(fp, "visitors = %d\n", current->visitors);
              }
              for (int i = 0; i < current->visitors; i++) {
                fprintf(fp, "name%d = %s\n", i+1, current->name[i]);
                fprintf(fp, "id%d = %s\n", i+1,current->id[i]);
                fprintf(fp, "arrival%d = %d/%d/%d\n", i+1,current->arrival[i][0],current->arrival[i][1],current->arrival[i][2]);
                fprintf(fp, "key%d = 1\n",i+1);
                fprintf(fp, "breakfast%d = %d\n", i+1,current->breakfast[i]);
                fprintf(fp, "nights%d = %d\n", i+1 ,current->nights[i] );
                fprintf(fp, "price%d = %d\n", i+1,current->price[i]);
              }
          }
          fprintf(fp, "\n");
      current=current->next;
    }
  }
  fclose(fp);
}


//this is a function either in main.c or time-related FUNCTIONS
time_t convert(int year,int month,int day){
  struct tm input={0};
  input.tm_mday=day;
  input.tm_year=year-1900;
  input.tm_mon=month-1;
  return mktime(&input);
}
//free_hotel is a function in the main function
void free_hotel(room_t *hotel[],int floor){
  room_t *buffer;
  for(size_t j;j<floor;j++){
  room_t*ptr_head=hotel[j];
  while (ptr_head != NULL) {
    buffer=ptr_head->next;
    if(ptr_head->visitors!=0){
      for (size_t i = 0; i < 6; i++) {
          free(ptr_head->name[i]);
          free(ptr_head->id[i]);
      }
    }
    free(ptr_head);
    ptr_head=buffer;
  }
}
}

void night_plus(room_t *hotel[],int flo,int all){
  room_t *current;
  for (size_t i = 0; i < flo; i++) {
    current=hotel[i];
    for (size_t j = 0; j < all; j++) {
      for (size_t k = 0; k < current->visitors; k++) {
          current->nights[k]++;
      }
      current=current->next;
    }
  }
}

void breakfast_plus(room_t *hotel[],int flo,int all){
  room_t *current;
  int plus=0;int people;
  printf("\nIt's breakfast time.\n");
  for (size_t i = 0; i < flo; i++) {
    current=hotel[i];
    for (size_t j = 0; j < all; j++) {
      for (size_t k = 0; k < current->visitors; k++) {
          plus= rand();
          if (plus<= RAND_MAX/2) {
            plus=0;//this value of plus is exactly not used in the judgement, just want to show 0=not plus breakfast and 1=show
          }else{
            plus=1;
            printf("%s people orders a breakfast\n", current->name[k]);
            current->breakfast[k]++;
            people++;
          }
      }
      current=current->next;
    }
  }
  printf("The breakfast ends.\n%d ordered breakfast.\n",people);
}


void price_fresh(room_t *hotel[],int flo,int all){
  //rules of charging: dorm:70/night;family:300/night;single:120/night;double:210/night;breakfast:15/time;
  room_t *current;
  for (size_t i = 0; i < flo; i++) {
    current=hotel[i];
    for (size_t j = 0; j < all; j++) {
          switch (current->type) {
            case 0:
            for (size_t k = 0; k < current->visitors; k++) {
              current->price[k]=70*current->nights[k]+15*current->breakfast[k];
            }
            break;
            case 1:
            current->price[0]=300*current->nights[0]+15*(current->breakfast[0]+current->breakfast[1]+current->breakfast[2]);
            break;
            case 2:
            current->price[0]=120*current->nights[0]+15*current->breakfast[0];
            break;
            case 3:
            current->price[0]=210*current->nights[0]+15*(current->breakfast[0]+current->breakfast[1]);
            break;
      }
      current=current->next;
    }
  }
}
