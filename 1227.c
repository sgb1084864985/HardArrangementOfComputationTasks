#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXSTRCOUNTS 7
#define CUTOFF 15

#define SERVERNAMECOUNTS 7 //the length of each server's name.

#define TRUE 1
#define FALSE 0

/*Macro Functions*/
#define New(type,n) malloc(sizeof(type)*(n))            //alloc a space with specific size
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAXSIZE MAX(sizeof(int*),sizeof(int))

#define lea(base,offset) ((base)+(offset)*(block_size))

typedef struct record{
    char server[MAXSTRCOUNTS+1];
    int time;
}*record;

void swap(unsigned char* array,size_t block_size,int a,int b){// swap array[a] and array[b]
    unsigned char tmp[MAXSIZE];
    memcpy(tmp,lea(array,a),block_size);
    memcpy(lea(array,a),lea(array,b),block_size);
    memcpy(lea(array,b),tmp,block_size);
}

record NewRecord(char* server,int time){
    record r = New(struct record,1);
    assert(r);
    strcpy(r->server,server);
    r->time=time;
    return r;
}

void random_string(char s[],int n){
    static const char table[]="0123456789abcdefghijklmnopqrstuvwxyz";
    int i;
    for(i=0;i<n;i++) s[i]=table[rand()%(sizeof(table)/sizeof(char)-1)];
    s[i]='\0';
}

void seconds_to_24h(int n,char s[]){
    int hh,mm,ss;
    ss=n%60;
    n/=60;
    mm=n%60;
    hh=n/60;
    sprintf(s,"%02d:%02d:%02d",hh,mm,ss);
}

record* random_records(int n){
    record* records = New(record,2*n);
    assert(records);
    char server[MAXSTRCOUNTS+1]; // "+1" is for '\0'
    int t1,t2,i;

    for(i=0;i<n;i++){
        random_string(server,MAXSTRCOUNTS);
        t1=rand()%(24*3600);
        t2=rand()%(24*3600);
        records[2*i]=NewRecord(server,t1);
        records[2*i+1]=NewRecord(server,t2);
    }

    return records;
}

void disarrange_record(record* records,int n){
    int i=0;
    int j;
    for(i=0;i<n;i++){
        j=rand()%(n-i)+i;
        swap((unsigned char*)records,sizeof(record),i,j);
    }
}

void output(record* records,int n,int k){
    printf("%d %d",n,k);
    char t_24h[MAXSTRCOUNTS+1];
    while(n--){
        seconds_to_24h(records[n]->time,t_24h);
        printf("\n%s %s",records[n]->server,t_24h);
    }
    while(k--){
        n=rand()%(24*3600);
        seconds_to_24h(n,t_24h);
        printf("\n%s",t_24h);
    }
}

int main(int argc, char* argv[]){
    int n=12,k=7;
    srand((unsigned)time(NULL));
    if(argc>1) n=atoi(argv[1]);
    if(argc>2) k=atoi(argv[2]);
    record* records=random_records(n/2);
    disarrange_record(records,n);
    output(records,n,k);
    return 0;
}