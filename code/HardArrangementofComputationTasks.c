/* HardArrangementofComputationTasks.c */

/*headers*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/*Macro Constant*/

/*
 * CUTOFF:
 * when function QuickSort is called,
 * if the size of the array to sort is
 * less than CUTOFF, we use insertion 
 * sort instead  of quick sort.
 */
#define CUTOFF 15

#define SERVERNAMECOUNTS 7 //the length of each server's name.
#define MAXFILENAMECOUNTS 40

#define TRUE 1
#define FALSE 0

/*Macro Functions*/
#define New(type,n) malloc(sizeof(type)*(n))            //alloc a space with specific size
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAXSIZE MAX(sizeof(int*),sizeof(int))

/*
 * lea:
 * Load Effective Address,
 * that is, returning an
 * address of the memory.
 * 
 * lea(base,offset) = &base[offset] 
 * with a specific type, the size 
 * of which is block_size
 */
#define lea(base,offset) ((base)+(offset)*(block_size))

/*
 * record:
 * records the start or 
 * end time of a server
 */
typedef struct record{
    char server[8];
    int time;
}*record;

/*
 * interval:
 * records the start time
 * and end time of a server.
 * we don't save the name of
 * the server.
 */
typedef struct interval{
    int begin,end;
}*interval;

/*
 * times:
 * 
 * times_list: 
 * records all the continuous
 * running times available for 
 * tasks.
 * 
 * times_sum_list:
 * times_sum_list[i] = sum([times_list[i] for i in range(i,len)]) 
 * 
 * len:
 * the length of list
 */
typedef struct times{
    int* times_list;
    int* times_sum_list;
    int len; 
}*times;

/*
 * function CreateIntervals:
 * According to the n server records,
 * we save the start time and end time
 * of each server.
 * 
 * return the array of intervals.
 */
interval* CreateIntervals(record* records,int n);

/*
 * function MergeIntervals:
 * if a interval overlaps another 
 * interval, merge them together.
 * 
 * return: times t, which contains
 * the sorted information of all 
 * merged intervals, which are the
 * continuous running time available
 * for tasks.
 */
times MergeIntervals(interval* intervals,int n);

/*
 * function NewInterval:
 * create a new variable of 
 * type struct interval, and return 
 * its pointer.
 * function arguments are the two 
 * attributes of the struct.
 */
interval NewInterval(int begin,int end);

/*
 * function NewRecord:
 * create a new variable of 
 * type struct record, and return 
 * its pointer.
 * function arguments are the two 
 * attributes of the struct.
 */
record NewRecord(char* server,int time);

/*
 * function read_record:
 * read records from the standard
 * input.
 * n: the number of the records to
 * read 
 * 
 * return the pointer of arrays of records.
 */
record* read_record(int n);

/*
 * function read_query:
 * read a query from the standard
 * input.
 * 
 * return the computing time of
 * the task.
 */
int read_query();

/*
 * function cmp_int:
 * Compare two integers.
 * arguments: bigger, smaller are
 * the addresses of two integers.
 * 
 * If bigger is no less than smaller, return True;
 * else return False.
 */
int cmp_int(void* bigger,void* smaller);

/*
 * function BinarySearch:
 * Find the position of an given element if
 * it is in the array using binary search.
 * 
 * array[]: The function will search in this array.
 * n: the length of the array.
 * key: the integer to find.
 * 
 * return the position of key in the array.
 */
int BinarySearch(int array[],int n,int key);

/*
 * function cmp_start_time:
 * Compare the start time of two intervals.
 * arguments: bigger, smaller are the
 * addresses of pointers of two intervals.
 * 
 * If bigger is no less than smaller, return True;
 * else return False.
 */
int cmp_start_time(void* bigger,void* smaller);

/*
 * function cmp_start_time:
 * Compare the name of two records.
 * arguments: bigger, smaller are the
 * addresses of pointers of two records.
 * 
 * If bigger is no less than smaller, return True;
 * else return False.
 */
int cmp_record(void* bigger,void* smaller);

/*
 * function QuickSort: (like qsort in stdlib.h)
 * Sorting function using
 * quick sort algorithm.
 * When the array size is 
 * too short, using insertion
 * sort algorithm.
 * 
 * array: the array to sort
 * block_size: the size of each element
 * left: the left bound of the array to sort
 * right: the right bound of the array to sort
 * cmp: the method to compare any two elements
 */
void QuickSort(void* array,size_t block_size,int left,int right,int(*cmp)(void* bigger,void* smaller));

/*
 * function median:
 * Given an array, compare the
 * array[left] and array[right]
 * and array[(left+right)/2],and
 * find the second largest one,
 * exchange it with array[right]
 * 
 * array: the array to sort
 * left,right: the first and
 * third elements to compare.
 * 
 * cmp: the method to compare any two elements
 */
void median(unsigned char* head,size_t block_size,int left,int right,int(*cmp)(void* bigger,void* smaller));

/*
 * function swap:
 * exchange the two elements of
 * an array.
 * 
 * array: the array of any type
 * block_size: the size of each element
 * a,b: the positions of two elements to swap
 */
void swap(unsigned char* array,size_t block_size,int a,int b);

/*
 * function InsertionSort:
 * Sorting function using
 * insertion sort algorithm.
 * 
 * array: the array to sort
 * block_size: the size of each element
 * n:the length of the array
 * cmp: the method to compare any two elements
 */
void InsertionSort(void* array,size_t block_size,int n,int(*cmp)(void* bigger,void* smaller));

/*
 * function process_query:
 * Given a bances of queries, 
 * generate a text file with
 * output in it, that is, the
 * max start points for the task
 * of each query.
 * 
 * t: the time information 
 * len: the number of continuous integers
 * queries: the queries put in 
 * filename: the file to put the output
 */
void process_query(times t,int len,int queries,char filename[]);

/*
 * function process_query:
 * show a the content of a file
 * 
 * filename: the name of file
 */
void cat(char filename[]);

/* 
 * function DestoryIntervals:
 * Destory the interval struct
 * arrays. n is its length.
 */
void DestoryIntervals(interval* intervals,int n);

/* 
 * function DestoryRecords:
 * Destory the record struct
 * arrays. n is its length.
 */
void DestoryRecords(record* records,int n);

int main(int argc, char* argv[]){
    //n: the number of server records
    //k: the number of queries
    int n,k;

    char* filename ="output";

    if(argc>1 && !strcmp("-f",argv[1])){
        if(argc<3 || argv[2][0]=='-'){puts("usage: arrange -f filename");exit(0);}
        filename=argv[2];
        argc-=2;
        argv+=2;
    }

    scanf("%d%d",&n,&k);

    record* records=read_record(n);

    interval* intervals=CreateIntervals(records,n/2);

    DestoryRecords(records,n);

    times t=MergeIntervals(intervals,n/2);

    DestoryIntervals(intervals,n/2);

    process_query(t,t->len,k,"output");

    if(argc>1 && !strcmp("-q",argv[1]));// be quiet
    else
        cat("output");
    
    printf("\n");
    system("pause");
    return 0;
}

void InsertionSort(void* array,size_t block_size,int n,int(*cmp)(void* bigger,void* smaller)){
    int i,j;
    unsigned char* head= array;// let the pointer of array be the byte type
    unsigned char  tmp[MAXSIZE];
    for(i=1;i<n;i++){
        memcpy(tmp,lea(head,i),block_size);// tmp=head[i]
        for(j=i-1;j>=0;j--){
            if((*cmp)(lea(head,j),tmp)){   //if head[j] > tmp
                memcpy(lea(head,j+1),lea(head,j),block_size); //head[j+1]=head[j]
            }
            else break;
        }
        memcpy(lea(head,j+1),tmp,block_size); //head[j+1]=tmp
    }
}

void swap(unsigned char* array,size_t block_size,int a,int b){// swap array[a] and array[b]
    unsigned char tmp[MAXSIZE];
    memcpy(tmp,lea(array,a),block_size);
    memcpy(lea(array,a),lea(array,b),block_size);
    memcpy(lea(array,b),tmp,block_size);
}

void median(unsigned char* head,size_t block_size,int left,int right,int(*cmp)(void* bigger,void* smaller)){
    int mid=(left+right)/2;
    if((*cmp)(lea(head,left),lea(head,right))){// if head[left]>=head[right]
        if((*cmp)(lea(head,mid),lea(head,right))) // if head[mid]>=head[right]
            if((*cmp)(lea(head,left),lea(head,mid))) // if head[left]>=head[mid]
                swap(head,block_size,right,mid);
            else
                swap(head,block_size,right,left);
    }
    else
        if((*cmp)(lea(head,right),lea(head,mid))) // if head[right]>=head[mid]
            if((*cmp)(lea(head,mid),lea(head,left))) // if head[mid]>=head[left]
                swap(head,block_size,right,mid);
            else 
                swap(head,block_size,right,left);
}

void QuickSort(void* array,size_t block_size,int left,int right,int(*cmp)(void* bigger,void* smaller)){
    unsigned char* head=array;
    if(right<left+CUTOFF)// array is too short. use insetion sort.
        InsertionSort(lea(head,left),block_size,right-left+1,cmp);
    else{
        //the element to divide the array into left part and right part is put in the right temporarily.
        int i=left,j=left;//i: the left bound of right part. j: the working pointer
        median(head,block_size,left,right,cmp);//find the middle one and put it in the right.
        while(j<right){
            if((*cmp)(lea(head,right),lea(head,j))){//if head[right]>=head[j]
                swap(head,block_size,i,j);
                i++;
            }
            j++;
        }
        swap(head,block_size,i,right);

        QuickSort(array,block_size,left,i-1,cmp);
        QuickSort(array,block_size,i+1,right,cmp);
    }
}

int cmp_record(void* bigger,void* smaller){
    //first compare two records by name(ascii order), then by time(late>early).
    record a=*(record*)bigger,b=*(record*)smaller;
    int flag=strcmp(a->server,b->server);
    if(flag>0 || flag==0 && a->time>b->time)
        return TRUE;
    else
        return FALSE;
}

int cmp_start_time(void* bigger,void* smaller){
    interval a=*(interval*)bigger,b=*(interval*)smaller;
    if(a->begin>=b->begin)
        return TRUE;
    else
        return FALSE;
}

int cmp_int(void* bigger,void* smaller){
    return *(int*)bigger >= *(int*)smaller;
}

record NewRecord(char* server,int time){
    record r = New(struct record,1);
    assert(r);
    strcpy(r->server,server);
    r->time=time;
    return r;
}

interval NewInterval(int begin,int end){
    interval intrvl=New(struct interval,1);
    assert(intrvl);
    intrvl->begin=begin;
    intrvl->end=end;
    return intrvl;
}

record* read_record(int n){
    record* records = New(record,n);
    assert(records);
    char server[SERVERNAMECOUNTS+1]; // "+1" is for '\0'
    int time,hh,mm,ss;
    while(n--){// n is declining, being the counter
        scanf("%s %d:%d:%d",server,&hh,&mm,&ss);
        time=hh*3600+mm*60+ss;
        records[n]=NewRecord(server,time);
    }
    return records;
}

interval* CreateIntervals(record* records,int n){
    // sort the records by name and time,
    // and select each two be the start time
    // and end time of a interval.
    // then sort the intervals by their start time 
    int i=0;
    interval* intervals = New(interval,n);
    assert(intervals);
    QuickSort(records,sizeof(record),0,2*n-1,cmp_record);
    for(i=0;i<n;i++){
        intervals[i]=NewInterval(records[2*i]->time,records[2*i+1]->time);
    }
    QuickSort(intervals,sizeof(interval),0,n-1,cmp_start_time);
    return intervals;
}

times MergeIntervals(interval* intervals,int n){

    times t=New(struct times,1);
    assert(t);
    t->len=0;
    t->times_list=New(int,n);
    t->times_sum_list=New(int,n);
    assert( t->times_list && t->times_sum_list);
    int begin=intervals[0]->begin,end=intervals[0]->end,i;

    // there's intervals A and B
    // If their time order is like following,
    // Astart --> Bstart --> Aend --> Bend
    // then we can merge them together.
    for(i=1;i<n;i++){
        // begin: the start time of current merged(or not yet) interval
        // end: the end time of current merged(or not yet) interval
        if(intervals[i]->begin>=end){//it is the case: Astart --> Aend --> Bstart, so they can't merge together
            t->times_list[t->len++]=end-begin;
            begin=intervals[i]->begin;
            end=intervals[i]->end;
        }
        else{
            end=MAX(intervals[i]->end,end);// make end-begin more larger
        }
    }
    t->times_list[t->len++]=end-begin;

    QuickSort(t->times_list,sizeof(int),0,t->len-1,cmp_int);

    //the following routines is to get all starting points for a certain query.
    t->times_sum_list[t->len-1]=t->times_list[t->len-1];
    for(i=t->len-2;i>=0;i--){
        t->times_sum_list[i]=t->times_sum_list[i+1]+t->times_list[i];
    }
    return t;
}

int BinarySearch(int array[],int n,int key){
    int i=0,j=n-1;
    int mid;
    while(i<=j){
        mid=(i+j)/2;
        if(array[mid]<key)
            i=mid+1;
        else
            j=mid-1;
    }
    return i;
}

int read_query(){
    int hh,mm,ss;
    scanf(" %d:%d:%d",&hh,&mm,&ss);
    return hh*3600+mm*60+ss;
}

void DestoryRecords(record* records,int n){
    while(n--) free(records[n]);
    free(records);
}
void DestoryIntervals(interval* intervals,int n){
    while(n--) free(intervals[n]);
    free(intervals);
}

void cat(char filename[]){
    FILE* fp=fopen(filename,"r");
    assert(fp);
    char s[30];
    while(fgets(s,30,fp)) printf("%s",s);
    fclose(fp);
}

void process_query(times t,int len,int querys,char filename[]){
    FILE*fp=fopen(filename,"w");
    if(!fp){
        printf("\ncan't open the file %s\n",filename);
        exit(0);
    }
    int time,i;
    fprintf(fp,"%d",t->times_list[len-1]);
    while(querys--){
        time=read_query();
        if(time>t->times_list[len-1]) fprintf(fp,"\n0");// we cannot run this task
        else{
            i=BinarySearch(t->times_list,len,time);
            fprintf(fp,"\n%d",t->times_sum_list[i]-(len-i)*(time-1));// calculate the all staring points for a certain task
        }
    }
    fclose(fp);
}