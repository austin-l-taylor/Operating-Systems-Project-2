#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX 10240
#define NUM_THREADS  10

int n1,n2;
char *s1,*s2;
FILE *fp;
int countArray[NUM_THREADS]={0};

int readf(FILE *fp)
{
	if((fp=fopen("strings.txt", "r"))==NULL){
		printf("ERROR: can't open string.txt!\n");
		return 0;
	}
	s1=(char *)malloc(sizeof(char)*MAX);
	if(s1==NULL){
		printf("ERROR: Out of memory!\n");
		return -1;
	}
	s2=(char *)malloc(sizeof(char)*MAX);
	if(s1==NULL){
		printf("ERROR: Out of memory\n");
		return -1;
	}
	s1=fgets(s1, MAX, fp);
	s2=fgets(s2, MAX, fp);
	n1=strlen(s1);  
	n2=strlen(s2)-1; 

	if(s1==NULL || s2==NULL || n1<n2)  
		return -1;
	return 0;
}

int num_substring(int t)
{
    int count = 0;
    int start = t * (n1 / NUM_THREADS);
    int end = (t == NUM_THREADS - 1) ? n1 : start + (n1 / NUM_THREADS);
    char *p;

    for (p = s1 + start; p < s1 + end; p++)
    {
        if (strncmp(p, s2, n2) == 0)
            count++;
    }

    return count;
}

void *calSubStringThread(void *threadid){
    long tid = (long)threadid;
    printf("This is thread %ld, ", tid);
    int num = num_substring(tid);
    printf("find num of is: %d\n", num);
    pthread_exit((void*) num);
}

int main(int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    int counts[NUM_THREADS];
    int t, rc;
    int totalNum = 0;

	readf(fp);

	for(t=0; t<NUM_THREADS; t++){
        rc = pthread_create(&threads[t], NULL, calSubStringThread, (void *) (size_t)t);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for(t=0; t<NUM_THREADS; t++){
        pthread_join(threads[t], (void **)&counts[t]);
        totalNum += counts[t];
    }

 	printf("The number of substrings is: %d\n", totalNum);
	return 1;
}
