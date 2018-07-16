#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char* id_create(char *id);

int main(int argc, char const *argv[]) {
  char id[7];
  for (size_t i = 0; i < 10; i++) {
    id_create(id);
  }
  return 0;
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
